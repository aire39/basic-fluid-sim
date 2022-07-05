#pragma once

#include <cstdint>
#include "FluidDataStructures.h"

namespace FluidSim
{
    void SetBounds3D(int32_t b, float *x, int32_t N);
    void SetBounds2D(int32_t b, float *x, int32_t N);

    void LinearSolver3D(int32_t b, float *x, const float *x0, float a, float c, int32_t iter, int N);
    void LinearSolver2D(int32_t b, float *x, const float *x0, float a, float c, int32_t iter, int N);

    void Diffuse3D(int32_t b, float *x, float *x0, float diff, float dt, int32_t iter, int32_t N);
    void Diffuse2D(int32_t b, float *x, float *x0, float diff, float dt, int32_t iter, int32_t N);

    void Project3D(float *veloc_x, float *veloc_y, float *veloc_z, float *p, float *div, int32_t iter, int32_t N);
    void Project2D(float *veloc_x, float *veloc_y, float *p, float *div, int32_t iter, int32_t N);

    void Advect3D(int32_t b, float *d, const float *d0, const float *veloc_x, const float *veloc_y, const float *veloc_z, float dt, int32_t N);
    void Advect2D(int32_t b, float *d, const float *d0, const float *veloc_x, const float *veloc_y, float dt, int32_t N);

    [[maybe_unused]]void FluidCubeStep3D(FluidDataStructures::FluidCube * fluid_cube);
    void FluidCubeStep2D(FluidDataStructures::FluidCube * fluid_cube);
}
