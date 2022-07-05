#pragma once

#include <cstdint>

namespace FluidDataStructures
{
    struct FluidCube
    {
        int32_t size = 0; //
        float dt = 0.0f; // delta time
        float diff = 0.0f; // differential
        float visc = 0.0f; // viscosity

        float *s = nullptr;
        float *density = nullptr;

        float *v_x = nullptr;
        float *v_y = nullptr;
        float *v_z = nullptr;

        float *v_x0 = nullptr;
        float *v_y0 = nullptr;
        float *v_z0 = nullptr;
    };
}