#pragma once

#include "profiler_block_data.hpp"

#include <optional>

class ScopedProfilerBlock
{
public:
    ScopedProfilerBlock(const char* const name);
    ~ScopedProfilerBlock();

private:
    std::optional<ProfilerBlockData> m_impl;
};

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define SCOPED_PROFILER_BLOCK(name) ScopedProfilerBlock TOKENPASTE2(profiler_block_, __LINE__)(name);

#define SCOPED_PROFILER_FUNCTION() SCOPED_PROFILER_BLOCK(__func__)