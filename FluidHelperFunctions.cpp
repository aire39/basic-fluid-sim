//
// Created by sean on 6/28/2022.
//

#include <iostream>
#include <algorithm>

#include "FluidHelperFunctions.h"
#include "FluidUtility.h"

namespace FluidHelperFunctions
{
    FluidDataStructures::FluidCube * FluidCubeCreate3D(int32_t size, const float diffusion, const float viscosity, const float dt)
    {
        auto *fluid_cube = new FluidDataStructures::FluidCube;

        int32_t N = size;

        fluid_cube->size = N;
        fluid_cube->dt = dt;
        fluid_cube->diff = diffusion;
        fluid_cube->visc = viscosity;

        fluid_cube->s = new float[N * N * N];
        fluid_cube->density = new float [N * N * N];

        fluid_cube->v_x = new float [N * N * N];
        fluid_cube->v_y = new float [N * N * N];
        fluid_cube->v_z = new float [N * N * N];

        fluid_cube->v_x0 = new float [N * N * N];
        fluid_cube->v_y0 = new float [N * N * N];
        fluid_cube->v_z0 = new float [N * N * N];

        for (size_t i=0; i< (N*N*N); i++)
        {
            fluid_cube->s[i] = 0.0f;
            fluid_cube->density[i] = 0.0f;

            fluid_cube->v_x[i] = 0.0f;
            fluid_cube->v_y[i] = 0.0f;
            fluid_cube->v_z[i] = 0.0f;

            fluid_cube->v_x0[i] = 0.0f;
            fluid_cube->v_y0[i] = 0.0f;
            fluid_cube->v_z0[i] = 0.0f;
        }

        return fluid_cube;
    }

    FluidDataStructures::FluidCube *FluidCubeCreate2D(int32_t size, const float diffusion, const float viscosity, const float dt)
    {
        auto *fluid_cube = new FluidDataStructures::FluidCube;

        int32_t N = size;

        fluid_cube->size = N;
        fluid_cube->dt = dt;
        fluid_cube->diff = diffusion;
        fluid_cube->visc = viscosity;

        fluid_cube->s = new float[N * N];
        fluid_cube->density = new float [N * N];

        fluid_cube->v_x = new float [N * N];
        fluid_cube->v_y = new float [N * N];
        fluid_cube->v_z = nullptr;

        fluid_cube->v_x0 = new float [N * N];
        fluid_cube->v_y0 = new float [N * N];
        fluid_cube->v_z0 = nullptr;

        for (size_t i=0; i< (N*N); i++)
        {
            fluid_cube->s[i] = 0.0f;
            fluid_cube->density[i] = 0.0f;

            fluid_cube->v_x[i] = 0.0f;
            fluid_cube->v_y[i] = 0.0f;

            fluid_cube->v_x0[i] = 0.0f;
            fluid_cube->v_y0[i] = 0.0f;
        }

        return fluid_cube;
    }

    void FluidAddDensity(FluidDataStructures::FluidCube * fluid_cube, int32_t x, int32_t y, int32_t z, float amount)
    {
        if (fluid_cube)
        {
            int32_t N = fluid_cube->size;

            if (fluid_cube->v_z0)
            {
                fluid_cube->density[FluidUtility::IX(x, y, z, N)] += amount;
            }
            else
            {
                fluid_cube->density[FluidUtility::IX(x, y, 0, N)] += amount;
            }
        }
        else
        {
            std::cerr << "invalid FluidCube object!" << std::endl;
        }
    }

    void FluidAddDensity(FluidDataStructures::FluidCube * fluid_cube, int32_t x, int32_t y, int32_t z, int32_t scale, float amount)
    {
        if (fluid_cube)
        {
            int32_t N = fluid_cube->size;

            if (fluid_cube->v_z0)
            {
                fluid_cube->density[FluidUtility::IX(x, y, z, N)] += amount;
            }
            else
            {
                for (int32_t i=0; i<scale; i++)
                {
                    for (int32_t j=0; j<scale; j++)
                    {
                        fluid_cube->density[FluidUtility::IX(x + j - (scale / 2), y + i - (scale / 2), N)] += amount;
                    }
                }
            }
        }
        else
        {
            std::cerr << "invalid FluidCube object!" << std::endl;
        }
    }

    void FluidAddVelocity(FluidDataStructures::FluidCube * fluid_cube, int32_t x, int32_t y, int32_t z, float amount_x, float amount_y, float amount_z)
    {
        if (fluid_cube)
        {
            int32_t N = fluid_cube->size;
            int32_t index = FluidUtility::IX(x, y, z, N);

            if (fluid_cube->v_z)
            {
                fluid_cube->v_z[index] += amount_z;
            }
            else
            {
                index = FluidUtility::IX(x, y, N);
            }

            fluid_cube->v_x[index] += amount_x;
            fluid_cube->v_y[index] += amount_y;
        }
        else
        {
            std::cerr << "invalid FluidCube object!" << std::endl;
        }
    }

    void FluidAddVelocity(FluidDataStructures::FluidCube *fluid_cube, int32_t x, int32_t y, int32_t z, int32_t scale, float amount_x, float amount_y, float amount_z)
    {
        if (fluid_cube)
        {
            int32_t N = fluid_cube->size;

            for (size_t i=0; i<scale; i++)
            {
                for (size_t j=0; j<scale; j++)
                {
                    int32_t index = FluidUtility::IX(x + j - (scale / 2), y + i - (scale / 2), N);

                    if (fluid_cube->v_z)
                    {
                        fluid_cube->v_z[index] += amount_z;
                    }
                    else
                    {
                        index = FluidUtility::IX(x, y, 0, N);
                    }

                    fluid_cube->v_x[index] += amount_x;
                    fluid_cube->v_y[index] += amount_y;
                }
            }
        }
        else
        {
            std::cerr << "invalid FluidCube object!" << std::endl;
        }
    }

    void FluidCubeFree(FluidDataStructures::FluidCube * fluid_cube)
    {
        delete [] fluid_cube->v_x;
        delete [] fluid_cube->v_y;

        if (fluid_cube->v_z)
        {
            delete[] fluid_cube->v_z;
        }

        delete [] fluid_cube->v_x0;
        delete [] fluid_cube->v_y0;

        if (fluid_cube->v_z0)
        {
            delete[] fluid_cube->v_z0;
        }

        delete [] fluid_cube->density;
        delete [] fluid_cube->s;

        delete fluid_cube;

        fluid_cube = nullptr;
    }
}