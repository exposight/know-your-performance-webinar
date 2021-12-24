#include "scoped_metric.hpp"

#include "metric_agent.hpp"

ScopedMetric::ScopedMetric(const char* const name)
    : m_name(name)
    , m_started_at(std::chrono::steady_clock::now())
{
}

ScopedMetric::~ScopedMetric()
{
    const auto duration = std::chrono::steady_clock::now() - m_started_at;
    const auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    MetricAgent::get().record(m_name, duration_ms);
}
