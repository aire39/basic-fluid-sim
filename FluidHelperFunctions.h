#pragma once

#include <cstdint>
#include "FluidDataStructures.h"

namespace FluidHelperFunctions
{
    FluidDataStructures::FluidCube *FluidCubeCreate3D(int32_t size, float diffusion, float viscosity, float dt);
    FluidDataStructures::FluidCube *FluidCubeCreate2D(int32_t size, float diffusion, float viscosity, float dt);

    void FluidAddDensity(FluidDataStructures::FluidCube *fluid_cube, int32_t x, int32_t y, int32_t z, float amount);
    void FluidAddDensity(FluidDataStructures::FluidCube *fluid_cube, int32_t x, int32_t y, int32_t z, int32_t scale, float amount);
    void FluidAddVelocity(FluidDataStructures::FluidCube *fluid_cube, int32_t x, int32_t y, int32_t z, float amount_x, float amount_y, float amount_z);
    void FluidAddVelocity(FluidDataStructures::FluidCube *fluid_cube, int32_t x, int32_t y, int32_t z, int32_t scale, float amount_x, float amount_y, float amount_z);

    void FluidCubeFree(FluidDataStructures::FluidCube* fluid_cube);
}
