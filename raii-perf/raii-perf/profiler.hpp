#pragma once

#include "profiler_block_data.hpp"

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>

class Profiler
{
public:
    ~Profiler();

    static Profiler* getProfiler();
    static void start(const char* dump_file_name);

    ProfilerBlockData startBlock(const char* const block_name);
    void endBlock(ProfilerBlockData block_data);

private:
    // Should be singleton, not constructible externally and not copyable.
    Profiler(const char* dump_file_name);
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    void BlockDumpWorker();

    std::mutex m_mutex;
    std::vector<ProfilerBlockData> m_blocks_to_dump;

    std::condition_variable m_new_blocks_signal;
    std::thread m_thread;
    bool m_stopping_thread = false;

    std::ofstream m_dump_stream;
};