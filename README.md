# Code from "Know your performance" webinar
Here you can find the code from the webinar. MS Visual Studio 2019 solution and project files are here as well for ease. Note that some of them have custom compiler definitions set and include/link inputs added. C++ standard is set to 17.

## Simple hand-crafted instrumentation
Code is under the [raii-perf folder](raii-perf). Implementation uses the [RapidJSON](https://github.com/Tencent/rapidjson/) open-source header-only library for writing the data to file. In MS Visual Studio, it was attached as a NuGet package.

## EasyProfiler instrumentation
Code is under the [easy-perf folder](easy-perf). The [EasyProfiler](https://github.com/yse/easy_profiler) release was downloaded an unpacked to that folder in a 'easy_profiler-v2.1.0-msvc14-win64' sub-directory (use https://github.com/yse/easy_profiler/releases/download/v2.1.0/easy_profiler-v2.1.0-msvc14-win64.zip link to get that content).

## TracyProfiler instrumentation
Code is under the [tracy-perf folder](tracy-perf). The [Tracy Profiler](https://github.com/wolfpld/tracy) source code was downloaded an unpacked to that folder in a 'tracy-0.7.8' sub-directory.

## Metrics instrumentation
Code is under the [metric-perf folder](metric-perf).

## Watchdog
Code is under the [wd folder](wd).
