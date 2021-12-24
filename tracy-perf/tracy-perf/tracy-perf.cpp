
#include <Tracy.hpp>

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
    void doLittleWork()
    {
        ZoneScoped;
        std::this_thread::sleep_for(30ms);
    }
    void doMoreWork()
    {
        ZoneScoped;
        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    std::cout << "Starting\n";

    for (int i = 0; i < 5; ++i)
    {
        ZoneNamedN(OuterLoop, "outer loop", true);
        for (int j = 0; j < 5; ++j)
        {
            ZoneNamedN(InnerLoop, "inner loop", true);
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

    std::cout << "Finished!\n";
}