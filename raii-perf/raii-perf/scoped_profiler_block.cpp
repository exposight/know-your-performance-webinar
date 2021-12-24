#include "scoped_profiler_block.hpp"

#include "profiler.hpp"

ScopedProfilerBlock::ScopedProfilerBlock(const char* const name)
{
    if (auto profiler = Profiler::getProfiler())
    {
        m_impl = profiler->startBlock(name);
    }
}

ScopedProfilerBlock::~ScopedProfilerBlock()
{
    if (auto profiler = Profiler::getProfiler())
    {
        profiler->endBlock(std::move(*m_impl));
    }
}