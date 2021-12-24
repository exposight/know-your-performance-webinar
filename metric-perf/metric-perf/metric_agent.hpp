#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class MetricAgent
{
public:
    static MetricAgent& get();

    void record(const char* name, std::chrono::milliseconds duration);

private:
    // Should be singleton, not constructible externally and not copyable.
    MetricAgent();
    MetricAgent(const MetricAgent&) = delete;
    MetricAgent& operator=(const MetricAgent&) = delete;

    ~MetricAgent();

    void PrintWorker();

    std::mutex m_mutex;
    std::unordered_map<std::string, std::vector<std::chrono::milliseconds>> m_records_to_dump;

    std::condition_variable m_new_records_signal;
    std::thread m_thread;
    bool m_stopping_thread = false;
};