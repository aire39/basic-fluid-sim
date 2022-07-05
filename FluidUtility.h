#pragma once

#include <cstdint>
#include <algorithm>

namespace FluidUtility
{
    constexpr int32_t IX (const int32_t &x, const int32_t &y, const int32_t &N)
    {
        return std::clamp(x + y * N, 0, (N*N)-1);
    }

    constexpr int32_t IX (const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &N)
    {
        return std::clamp(((x + y * N) + (z * N * N)), 0, (N*N*N)-1);
    }
}
