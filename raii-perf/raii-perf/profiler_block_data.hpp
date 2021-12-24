#pragma once

#include <chrono>

struct ProfilerBlockData
{
    const char* block_name;
    std::chrono::steady_clock::time_point start_timepoint;
    std::chrono::steady_clock::time_point stop_timepoint;
};