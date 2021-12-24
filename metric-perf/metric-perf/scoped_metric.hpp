#pragma once

#include <chrono>

class ScopedMetric
{
public:
    ScopedMetric(const char* const name);
    ~ScopedMetric();

    ScopedMetric(const ScopedMetric&) = delete;
    ScopedMetric(ScopedMetric&&) = delete;

private:
    const char* const m_name;
    const std::chrono::steady_clock::time_point m_started_at;
};