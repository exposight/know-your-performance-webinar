
#include "profiler.hpp"
#include "scoped_profiler_block.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
    void doLittleWork()
    {
        SCOPED_PROFILER_FUNCTION();
        std::this_thread::sleep_for(30ms);
    }
    void doMoreWork()
    {
        SCOPED_PROFILER_FUNCTION();
        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    std::cout << "Starting\n";

    Profiler::start("perf_dump.json");

    for (int i = 0; i < 5; ++i)
    {
        ScopedProfilerBlock profiler_block("outer_loop");

        for (int j = 0; j < 5; ++j)
        {
            SCOPED_PROFILER_BLOCK("inner_loop");
            doLittleWork();
            doLittleWork();
        }
        doMoreWork();
    }

    std::cout << "Finished!\n";
}