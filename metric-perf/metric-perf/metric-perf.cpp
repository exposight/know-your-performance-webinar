
#include "scoped_metric.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
    void doLittleWork()
    {
        ScopedMetric metric(__func__);
        std::this_thread::sleep_for(30ms);
    }
    void doMoreWork()
    {
        ScopedMetric metric(__func__);
        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    std::cout << "Starting\n";

    for (int i = 0; i < 5; ++i)
    {
        ScopedMetric metric_outer("outer_loop");

        for (int j = 0; j < 5; ++j)
        {
            ScopedMetric metric_inner("inner_loop");
            doLittleWork();
            doLittleWork();
        }
        doMoreWork();
    }

    std::cout << "Finished!\n";
}