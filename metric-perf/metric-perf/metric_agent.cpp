#include "metric_agent.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

MetricAgent& MetricAgent::get()
{
    static MetricAgent agent;
    return agent;
}

void MetricAgent::record(const char* name, std::chrono::milliseconds duration)
{
    {
        std::lock_guard lock(m_mutex);
        m_records_to_dump[name].push_back(duration);
    }
    m_new_records_signal.notify_one();
}

MetricAgent::MetricAgent()
{
    m_thread = std::thread(&MetricAgent::PrintWorker, this);
}

MetricAgent::~MetricAgent()
{
    {
        std::lock_guard lock(m_mutex);
        m_stopping_thread = true;
    }
    m_new_records_signal.notify_one();
    m_thread.join();
}

void MetricAgent::PrintWorker()
{
    constexpr auto dump_interval = std::chrono::seconds(1);

    decltype(m_records_to_dump) records_to_dump;

    using DoublePrecisionDurationMs = std::chrono::duration<double, std::milli>;

    struct RecordStats
    {
        std::string_view name;
        std::size_t count;
        std::chrono::milliseconds min_duration;
        DoublePrecisionDurationMs mean_duration;
        std::chrono::milliseconds max_duration;
    };
    std::vector<RecordStats> stats;

    auto last_dump_at = std::chrono::steady_clock::time_point();

    bool stopped = false;
    while (!stopped)
    {
        {
            std::unique_lock lock(m_mutex);

            m_new_records_signal.wait(lock,
                [this]() { return !m_records_to_dump.empty() || m_stopping_thread; });

            stopped = m_stopping_thread;

            const auto now = std::chrono::steady_clock::now();
            if (now - last_dump_at < dump_interval)
            {
                continue;
            }

            last_dump_at = now;

            m_records_to_dump.swap(records_to_dump);
            m_records_to_dump.clear();
        }

        stats.clear();
        for (const auto& record_with_durations: records_to_dump)
        {
            const auto& [name, durations] = record_with_durations;
            const auto [min, max] = std::minmax_element(durations.begin(), durations.end());
            const auto durations_sum = std::accumulate(durations.begin(), durations.end(),
                std::chrono::microseconds::zero());
            const auto mean_duration =
                std::chrono::duration_cast<DoublePrecisionDurationMs>(durations_sum) / durations.size();
            stats.push_back({ name, durations.size(), *min, mean_duration, *max });
        }

        std::sort(stats.begin(), stats.end(),
            [](const auto& lhs, const auto rhs) { return lhs.name < rhs.name; });

        std::cout << "Performance stats start:\n";
        for (const auto& stat : stats)
        {
            std::cout
                << " Record " << stat.name 
                << " count:" << stat.count
                << " min:" << stat.min_duration.count()
                << "ms max:" << stat.max_duration.count()
                << "ms mean:" << stat.mean_duration.count()
                << "ms\n";
        }
        std::cout << "Performance stats dumped" << std::endl;
    }
}
