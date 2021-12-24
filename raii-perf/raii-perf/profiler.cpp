#include "profiler.hpp"

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

namespace {
    constexpr auto InvalidTimepoint = std::chrono::steady_clock::time_point();

    std::unique_ptr<Profiler> s_profiler;
}

Profiler* Profiler::getProfiler()
{
    return s_profiler.get();
}

void Profiler::start(const char* dump_file_name)
{
    s_profiler.reset(new Profiler(dump_file_name));
}

ProfilerBlockData Profiler::startBlock(const char* const block_name)
{
    ProfilerBlockData block{ block_name, std::chrono::steady_clock::now(), InvalidTimepoint };

    {
        std::lock_guard lock(m_mutex);
        m_blocks_to_dump.push_back(block);
    }
    m_new_blocks_signal.notify_one();

    return block;
}

void Profiler::endBlock(ProfilerBlockData block_data)
{
    block_data.stop_timepoint = std::chrono::steady_clock::now();

    {
        std::lock_guard lock(m_mutex);
        m_blocks_to_dump.push_back(std::move(block_data));
    }
    m_new_blocks_signal.notify_one();
}

Profiler::Profiler(const char* dump_file_name)
{
    m_dump_stream.open(dump_file_name);
    m_thread = std::thread(&Profiler::BlockDumpWorker, this);
}

Profiler::~Profiler()
{
    {
        std::lock_guard lock(m_mutex);
        m_stopping_thread = true;
    }
    m_new_blocks_signal.notify_one();
    m_thread.join();
}

void Profiler::BlockDumpWorker()
{
    rapidjson::OStreamWrapper stream(m_dump_stream);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> json_writer(stream);
    json_writer.StartArray();

    std::vector<ProfilerBlockData> blocks_to_dump;

    while (true)
    {
        {
            std::unique_lock lock(m_mutex);
            m_new_blocks_signal.wait(lock,
                [this]() { return !m_blocks_to_dump.empty() || m_stopping_thread; });

            if (m_stopping_thread)
                break;

            m_blocks_to_dump.swap(blocks_to_dump);
            m_blocks_to_dump.clear();
        }
        for (auto& block : blocks_to_dump)
        {
            if (block.stop_timepoint == InvalidTimepoint)
            {
                json_writer.StartObject();
                json_writer.Key("name");
                json_writer.String(block.block_name);
                json_writer.Key("nested");
                json_writer.StartArray();
            }
            else
            {
                const auto duration = block.stop_timepoint - block.start_timepoint;
                using DoublePrecisionDurationMs = std::chrono::duration<double, std::milli>;
                const auto duration_ms = std::chrono::duration_cast<DoublePrecisionDurationMs>(duration);
                json_writer.EndArray();
                json_writer.Key("duration_ms");
                json_writer.Double(duration_ms.count());
                json_writer.EndObject();
            }
        }
    }

    json_writer.EndArray();
}
