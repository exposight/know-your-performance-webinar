
#include "watchdog.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
    void doLittleWork()
    {
        std::this_thread::sleep_for(30ms);
    }
    void doMoreWork()
    {
        Watchdog watchdog(1s,
            []() { std::cout << "doMoreWork timed out!\n"; });
        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    std::cout << "Starting\n";

    {
        Watchdog watchdog(2s,
            []() { std::cout << "Everything timed out!\n"; });

        for (int i = 0; i < 5; ++i)
        {
            std::cout << " Iteration " << i << "\n";

            Watchdog watchdog(1s,
                []() { std::cout << "Outer loop timed out!\n"; });

            for (int j = 0; j < 50; ++j)
            {
                doLittleWork();
            }

            doMoreWork();
        }
    }

    std::cout << "Finished!\n";
}