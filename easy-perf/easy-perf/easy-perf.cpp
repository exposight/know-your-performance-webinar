
#define BUILD_WITH_EASY_PROFILER

#include <easy/profiler.h>
#include <easy/arbitrary_value.h>

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
    void doLittleWork()
    {
        EASY_FUNCTION();
        std::this_thread::sleep_for(30ms);
    }
    void doMoreWork()
    {
        EASY_FUNCTION();
        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    std::cout << "Starting\n";

    EASY_PROFILER_ENABLE;

    for (int i = 0; i < 5; ++i)
    {
        EASY_BLOCK("outer_loop");
        EASY_VALUE("iteration", i);

        for (int j = 0; j < 5; ++j)
        {
            EASY_BLOCK("inner_loop");
            EASY_VALUE("iteration", j);
            doLittleWork();
            doLittleWork();
        }
        doMoreWork();

        if (i == 3)
        {
            doMoreWork();
            doMoreWork();
        }
    }

    profiler::dumpBlocksToFile("test_profile.prof");

    std::cout << "Finished!\n";
}