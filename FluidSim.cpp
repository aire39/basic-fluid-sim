//
// Created by sean on 6/28/2022.
//

#include "FluidSim.h"
#include "FluidUtility.h"

#include <cmath>
#include <algorithm>

namespace
{
    constexpr int32_t iterations = 10;
}

namespace FluidSim
{
    void SetBounds3D(int32_t b, float *x, int32_t N)
    {
        for (int32_t i = 1; i<(N-1); i++)
        {
            for (int32_t j=1; j<(N-1); j++)
            {
                x[FluidUtility::IX(j, i, 0, N)] = (b == 3) ? -x[FluidUtility::IX(j, i, 1, N)] : x[FluidUtility::IX(j, i, 1, N)];
                x[FluidUtility::IX(j, i, (N-1), N)] = (b == 3) ? -x[FluidUtility::IX(j, i, (N-2), N)] : x[FluidUtility::IX(j, i, (N-2), N)];
            }
        }

        for (int32_t i = 1; i<(N-1); i++)
        {
            for (int32_t j=1; j<(N-1); j++)
            {
                x[FluidUtility::IX(j, 0, i, N)] = (b == 2) ? -x[FluidUtility::IX(j, 1, i, N)] : x[FluidUtility::IX(j, 1, i, N)];
                x[FluidUtility::IX(j, (N-1), i, N)] = (b == 2) ? -x[FluidUtility::IX(j, (N-2), i, N)] : x[FluidUtility::IX(j, (N-2), i, N)];
            }
        }

        for (int32_t i = 1; i<(N-1); i++)
        {
            for (int32_t j=1; j<(N-1); j++)
            {
                x[FluidUtility::IX(0, i, j, N)] = (b == 1) ? -x[FluidUtility::IX(1, i, j, N)] : x[FluidUtility::IX(1, i, j, N)];
                x[FluidUtility::IX((N-1), i, j, N)] = (b == 1) ? -x[FluidUtility::IX((N-2), i, j, N)] : x[FluidUtility::IX((N-2), i, j, N)];
            }
        }

        constexpr float c = 0.33f;

        x[FluidUtility::IX(0, 0, 0, N)] = c * ( x[FluidUtility::IX(1, 0, 0, N)]
                                                       + x[FluidUtility::IX(0, 1, 0, N)]
                                                       + x[FluidUtility::IX(0, 0, 1, N)]);

        x[FluidUtility::IX(0, (N-1), 0, N)] = c * ( x[FluidUtility::IX(1, (N-1), 0, N)]
                                                           + x[FluidUtility::IX(0, (N-2), 0, N)]
                                                           + x[FluidUtility::IX(0, (N-1), 1, N)]);

        x[FluidUtility::IX(0, 0, (N-1), N)] = c * ( x[FluidUtility::IX(1, 0, (N-1), N)]
                                                           + x[FluidUtility::IX(0, 1, (N-1), N)]
                                                           + x[FluidUtility::IX(0, 0, N, N)]);

        x[FluidUtility::IX(0, (N-1), (N-1), N)] = c * ( x[FluidUtility::IX(1, (N-1), (N-1), N)]
                                                               + x[FluidUtility::IX(0, (N-2), (N-1), N)]
                                                               + x[FluidUtility::IX(0, (N-1), (N-2), N)]);

        x[FluidUtility::IX((N-1), 0, 0, N)] = c * ( x[FluidUtility::IX((N-2), 0, 0, N)]
                                                           + x[FluidUtility::IX((N-1), 1, 0, N)]
                                                           + x[FluidUtility::IX((N-1), 0, 1, N)]);

        x[FluidUtility::IX((N-1), (N-1), 0, N)] = c * ( x[FluidUtility::IX((N-2), (N-1), 0, N)]
                                                               + x[FluidUtility::IX((N-1), (N-2), 0, N)]
                                                               + x[FluidUtility::IX((N-1), (N-1), 1, N)]);

        x[FluidUtility::IX((N-1), 0, (N-1), N)] = c * ( x[FluidUtility::IX((N-2), 0, (N-1), N)]
                                                               + x[FluidUtility::IX((N-1), 1, (N-1), N)]
                                                               + x[FluidUtility::IX((N-1), 0, (N-2), N)]);

        x[FluidUtility::IX((N-1), (N-1), (N-1), N)] = c * ( x[FluidUtility::IX((N-2),(N-1), (N-1), N)]
                                                                   + x[FluidUtility::IX((N-1), (N-2), (N-1), N)]
                                                                   + x[FluidUtility::IX((N-1), (N-1), (N-2), N)]);

    }

    void SetBounds2D(int32_t b, float *x, int32_t N)
    {
        for (int32_t j=1; j<(N-1); j++)
        {
            x[FluidUtility::IX(j, 0, N)]     = (b == 2) ? -x[FluidUtility::IX(j, 1, N)] : x[FluidUtility::IX(j, 1, N)];
            x[FluidUtility::IX(j, (N-1), N)] = (b == 2) ? -x[FluidUtility::IX(j, (N-2), N)] : x[FluidUtility::IX(j, (N-2), N)];
        }

        for (int32_t i = 1; i<(N-1); i++)
        {
            x[FluidUtility::IX(0, i, N)]     = (b == 1) ? -x[FluidUtility::IX(1, i, N)] : x[FluidUtility::IX(1, i, N)];
            x[FluidUtility::IX((N-1), i, N)] = (b == 1) ? -x[FluidUtility::IX((N-2), i, N)] : x[FluidUtility::IX((N-2), i, N)];
        }

        constexpr float c = 0.5f;

        x[FluidUtility::IX(0, 0, N)] = c * ( x[FluidUtility::IX(1, 0, N)]
                                                 + x[FluidUtility::IX(0, 1, N)]);

        x[FluidUtility::IX(0, (N-1), N)] = c * ( x[FluidUtility::IX(1, (N-1), N)]
                                                     + x[FluidUtility::IX(0, (N-2), N)]);

        x[FluidUtility::IX((N-1), 0, N)] = c * ( x[FluidUtility::IX((N-2), 0, N)]
                                                     + x[FluidUtility::IX((N-1), 1, N)]);

        x[FluidUtility::IX((N-1), (N-1), N)] = c * ( x[FluidUtility::IX((N-2), (N-1), N)]
                                                         + x[FluidUtility::IX((N-1), (N-2), N)]);

    }

    void LinearSolver3D(int32_t b, float *x, const float *x0, float a, float c, int32_t iter, int N)
    {
        float c_recip = 1.0f / c;

        for (int32_t k = 0; k<iter; k++)
        {
            for (int32_t m = 1; m<(N-1); m++)
            {
                for (int32_t j = 1; j<(N-1); j++)
                {
                    for (int32_t i = 1; i<(N-1); i++)
                    {
                        x[FluidUtility::IX(i, j, m, N)] =
                            ( x0[FluidUtility::IX(i,j,m, N)]
                                + a * ( x[FluidUtility::IX(i+1, j  , m  , N)]
                                      + x[FluidUtility::IX(i-1, j  , m  , N)]
                                      + x[FluidUtility::IX(i  , j+1, m  , N)]
                                      + x[FluidUtility::IX(i  , j-1, m  , N)]
                                      + x[FluidUtility::IX(i  , j  , m+1, N)]
                                      + x[FluidUtility::IX(i  , j  , m-1, N)]
                                      )
                            ) * c_recip;
                    }
                }
            }

            SetBounds3D(b, x, N);
        }
    }

    void LinearSolver2D(int32_t b, float *x, const float *x0, float a, float c, int32_t iter, int N)
    {
        const float c_recip = 1.0f / c;

        for (int32_t k = 0; k<iter; k++)
        {
            for (int32_t i = 1; i<(N-1); i++)
            {
                for (int32_t j = 1; j<(N-1); j++)
                {
                    x[FluidUtility::IX(j, i, N)] =
                            ( x0[FluidUtility::IX(j, i, N)]
                                + a * ( x[FluidUtility::IX(j+1, i  , N)]
                                      + x[FluidUtility::IX(j-1, i  , N)]
                                      + x[FluidUtility::IX(j  , i+1, N)]
                                      + x[FluidUtility::IX(j  , i-1, N)]
                                      )) * c_recip;
                }
            }

            SetBounds2D(b, x, N);
        }
    }

    void Diffuse3D(int32_t b, float *x, float *x0, float diff, float dt, int32_t iter, int32_t N)
    {
        float a = dt * diff * static_cast<float>(N-2) * static_cast<float>(N-2);
        LinearSolver3D(b, x, x0, a, 1 + 6 * a, iter, N);
    }

    void Diffuse2D(int32_t b, float *x, float *x0, float diff, float dt, int32_t iter, int32_t N)
    {
        float a = dt * diff * static_cast<float>(N-2) * static_cast<float>(N-2);
        LinearSolver2D(b, x, x0, a, 1 + 6 * a, iter, N);
    }

    void Project3D(float *veloc_x, float *veloc_y, float *veloc_z, float *p, float *div, int32_t iter, int32_t N)
    {
        constexpr float c = 0.5f;

        for (int32_t k = 1; k<(N-1); k++)
        {
            for (int32_t i = 1; i<(N-1); i++)
            {
                for (int32_t j = 1; j<(N-1); j++)
                {
                    div[FluidUtility::IX(j, i, k, N)] = -c * ( veloc_x[FluidUtility::IX((j+1), i, k, N)]
                                                                      - veloc_x[FluidUtility::IX((j-1), i, k, N)]
                                                                      + veloc_y[FluidUtility::IX(j, (i+1), k, N)]
                                                                      - veloc_y[FluidUtility::IX(j, (i-1), k, N)]
                                                                      + veloc_z[FluidUtility::IX(j, i, (k+1), N)]
                                                                      - veloc_z[FluidUtility::IX(j, i, (k-1), N)]
                                                                      ) / static_cast<float>(N);

                    p[FluidUtility::IX(j, i, k, N)] = 0;
                }
            }
        }

        SetBounds3D(0, div, N);
        SetBounds3D(0, p, N);
        LinearSolver3D(0, p, div, 1, 6, iter, N);

        for (int32_t k = 1; k<(N-1); k++)
        {
            for (int32_t i = 1; i<(N-1); i++)
            {
                for (int32_t j = 1; j<(N-1); j++)
                {
                    veloc_x[FluidUtility::IX(j, i, k, N)] -= c * ( p[FluidUtility::IX((j+1), i, k, N)]
                                                                          - p[FluidUtility::IX((j-1), i, k, N)]) * static_cast<float>(N);

                    veloc_y[FluidUtility::IX(j, i, k, N)] -= c * ( p[FluidUtility::IX(j, (i+1), k, N)]
                                                                          - p[FluidUtility::IX(j, (i-1), k, N)]) * static_cast<float>(N);

                    veloc_z[FluidUtility::IX(j, i, k, N)] -= c * ( p[FluidUtility::IX(j, i, (k+1), N)]
                                                                          - p[FluidUtility::IX(j, i, (k-1), N)]) * static_cast<float>(N);
                }
            }
        }

        SetBounds3D(1, veloc_x, N);
        SetBounds3D(2, veloc_y, N);
        SetBounds3D(3, veloc_z, N);
    }

    void Project2D(float *veloc_x, float *veloc_y, float *p, float *div, int32_t iter, int32_t N)
    {
        constexpr float c = 0.5f;

        for (int32_t i = 1; i<(N-1); i++)
        {
            for (int32_t j = 1; j<(N-1); j++)
            {
                div[FluidUtility::IX(j, i, N)] = -c * ( veloc_x[FluidUtility::IX((j+1), i, N)]
                                                            - veloc_x[FluidUtility::IX((j-1), i, N)]
                                                            + veloc_y[FluidUtility::IX(j, (i+1), N)]
                                                            - veloc_y[FluidUtility::IX(j, (i-1), N)]
                                                            ) / static_cast<float>(N);

                p[FluidUtility::IX(j, i, N)] = 0.0f;
            }
        }

        SetBounds2D(0, div, N);
        SetBounds2D(0, p, N);
        LinearSolver2D(0, p, div, 1, 6, iter, N);

        for (int32_t i = 1; i<(N-1); i++)
        {
            for (int32_t j = 1; j<(N-1); j++)
            {
                veloc_x[FluidUtility::IX(j, i, N)] -= c * ( p[FluidUtility::IX((j+1), i, N)]
                                                                - p[FluidUtility::IX((j-1), i, N)])
                                                                * static_cast<float>(N);

                veloc_y[FluidUtility::IX(j, i, N)] -= c * ( p[FluidUtility::IX(j, (i+1), N)]
                                                                - p[FluidUtility::IX(j, (i-1), N)])
                                                                * static_cast<float>(N);
            }
        }

        SetBounds2D(1, veloc_x, N);
        SetBounds2D(2, veloc_y, N);
    }

    void Advect3D(int32_t b, float *d, const float *d0, const float *veloc_x, const float *veloc_y, const float *veloc_z, float dt, int32_t N)
    {
        float i_0, i_1, j_0, j_1, k_0, k_1;

        auto dt_x = dt * static_cast<float>(N - 2);
        auto dt_y = dt * static_cast<float>(N - 2);
        auto dt_z = dt * static_cast<float>(N - 2);

        float s_0, s_1, t_0, t_1, u_0, u_1;
        float tmp_1, tmp_2, tmp_3, x, y, z;

        auto n_float = static_cast<float>(N-2);
        float i_float, j_float, k_float;

        int32_t i, j, k;

        constexpr float bias = 0.5f;

        for (k = 1, k_float = 1.0f; k < (N-1); k++)
        {
            for (i = 1, i_float = 1.0f; i < (N-1); i++)
            {
                for (j = 1, j_float = 1.0f; j < (N-1); j++)
                {
                    tmp_1 = dt_x * veloc_x[FluidUtility::IX(j, i, k, N)];
                    tmp_2 = dt_y * veloc_y[FluidUtility::IX(j, i, k, N)];
                    tmp_3 = dt_z * veloc_z[FluidUtility::IX(j, i, k, N)];

                    x = j_float - tmp_1;
                    y = i_float - tmp_2;
                    z = k_float - tmp_3;

                    x = std::max(bias, x);
                    x = std::min(n_float + bias, x);

                    j_0 = std::floor(x);
                    j_1 = j_0 + bias;

                    y = std::max(bias, y);
                    y = std::min(n_float + bias, y);

                    i_0 = std::floor(y);
                    i_1 = i_0 + bias;

                    z = std::max(bias, z);
                    z = std::min(n_float + bias, z);

                    k_0 = std::floor(z);
                    k_1 = k_0 + bias;

                    s_1 = (x - j_0);
                    s_0 = 1.0f - s_1;

                    t_1 = (y - i_0);
                    t_0 = 1.0f - t_1;

                    u_1 = (z - k_0);
                    u_0 = 1.0f - u_1;

                    auto i_0_j = static_cast<int32_t>(i_0);
                    auto i_1_j = static_cast<int32_t>(i_1);
                    auto j_0_j = static_cast<int32_t>(j_0);
                    auto j_1_j = static_cast<int32_t>(j_1);
                    auto k_0_j = static_cast<int32_t>(k_0);
                    auto k_1_j = static_cast<int32_t>(k_1);

                    d[FluidUtility::IX(j, i, k, N)] = s_0 * ( t_0 * ( u_0 * d0[FluidUtility::IX(j_0_j, i_0_j, k_0_j, N)]
                                                                             + u_1 * d0[FluidUtility::IX(j_0_j, i_0_j, k_1_j, N)]))
                                                                   + ( t_1 * ( u_0 * d0[FluidUtility::IX(j_0_j, i_1_j, k_0_j, N)]
                                                                             + u_1 * d0[FluidUtility::IX(j_0_j, i_1_j, k_1_j, N)]))
                                                             + s_1 * ( t_0 * ( u_0 * d0[FluidUtility::IX(j_1_j, i_0_j, k_0_j, N)]
                                                                             + u_1 * d0[FluidUtility::IX(j_1_j, i_0_j, k_1_j, N)]))
                                                                   + ( t_1 * ( u_0 * d0[FluidUtility::IX(j_1_j, i_1_j, k_0_j, N)]
                                                                             + u_1 * d0[FluidUtility::IX(j_1_j, i_1_j, k_1_j, N)]));


                    j_float++;
                }
                i_float++;
            }
            k_float++;
        }

        SetBounds3D(b, d, N);
    }

    void Advect2D(int32_t b, float *d, const float *d0, const float *veloc_x, const float *veloc_y, float dt, int32_t N)
    {
        float i_0, i_1, j_0, j_1;

        auto dt_x = dt * static_cast<float>(N - 2);
        auto dt_y = dt * static_cast<float>(N - 2);

        float s_0, s_1, t_0, t_1;
        float tmp_1, tmp_2, x, y;

        auto n_float = static_cast<float>(N - 2);

        constexpr float min_value = 0.5f;

        for (int32_t i = 1, i_float = 1; i < (N-1); i++, i_float++)
        {
            for (int32_t j = 1, j_float = 1; j < (N-1); j++, j_float++)
            {
                tmp_1 = dt_x * veloc_x[FluidUtility::IX(j, i, N)];
                tmp_2 = dt_y * veloc_y[FluidUtility::IX(j, i, N)];

                x = static_cast<float>(j_float) - tmp_1;
                y = static_cast<float>(i_float) - tmp_2;

                x = std::clamp(x, min_value, (n_float + min_value));
                j_0 = std::floor(x);
                j_1 = (j_0 + 1.0f);

                y = std::clamp(y, min_value, (n_float + min_value));
                i_0 = std::floor(y);
                i_1 = (i_0 + 1.0f);

                s_1 = (x - j_0);
                s_0 = (1.0f - s_1);

                t_1 = (y - i_0);
                t_0 = (1.0f - t_1);

                auto j_0_j = static_cast<int32_t>(j_0);
                auto j_1_j = static_cast<int32_t>(j_1);
                auto i_0_j = static_cast<int32_t>(i_0);
                auto i_1_j = static_cast<int32_t>(i_1);

                d[FluidUtility::IX(j, i, N)] =
                        s_0 * ( t_0 * d0[FluidUtility::IX(j_0_j, i_0_j, N)] +  t_1 * d0[FluidUtility::IX(j_0_j, i_1_j, N)]) +
                        s_1 * ( t_0 * d0[FluidUtility::IX(j_1_j, i_0_j, N)] +  t_1 * d0[FluidUtility::IX(j_1_j, i_1_j, N)]);

            }
        }

        SetBounds2D(b, d, N);
    }

    [[maybe_unused]]void FluidCubeStep3D(FluidDataStructures::FluidCube * fluid_cube)
    {
        FluidDataStructures::FluidCube fluid_data = *fluid_cube;

        Diffuse3D(1, fluid_data.v_x0, fluid_data.v_x, fluid_data.visc, fluid_data.dt, iterations, fluid_data.size);
        Diffuse3D(2, fluid_data.v_y0, fluid_data.v_y, fluid_data.visc, fluid_data.dt, iterations, fluid_data.size);
        Diffuse3D(3, fluid_data.v_z0, fluid_data.v_z, fluid_data.visc, fluid_data.dt, iterations, fluid_data.size);

        Project3D(fluid_data.v_x0, fluid_data.v_y0, fluid_data.v_z0, fluid_data.v_x, fluid_data.v_y, iterations, fluid_data.size);

        Advect3D(1, fluid_data.v_x, fluid_data.v_x0, fluid_data.v_x0, fluid_data.v_y0, fluid_data.v_z0, fluid_data.dt, fluid_data.size);
        Advect3D(2, fluid_data.v_y, fluid_data.v_y0, fluid_data.v_x0, fluid_data.v_y0, fluid_data.v_z0, fluid_data.dt, fluid_data.size);
        Advect3D(3, fluid_data.v_z, fluid_data.v_z0, fluid_data.v_x0, fluid_data.v_y0, fluid_data.v_z0, fluid_data.dt, fluid_data.size);

        Project3D(fluid_data.v_x, fluid_data.v_y, fluid_data.v_z, fluid_data.v_x0, fluid_data.v_y0, iterations, fluid_data.size);

        Diffuse3D(0, fluid_data.s, fluid_data.density, fluid_data.diff, fluid_data.dt, iterations, fluid_data.size);
        Advect3D(0, fluid_data.density, fluid_data.s, fluid_data.v_x, fluid_data.v_y, fluid_data.v_z, fluid_data.dt, fluid_data.size);
    }

    void FluidCubeStep2D(FluidDataStructures::FluidCube * fluid_cube)
    {
        FluidDataStructures::FluidCube fluid_data = *fluid_cube;

        Diffuse2D(1, fluid_data.v_x0, fluid_data.v_x, fluid_data.visc, fluid_data.dt, iterations, fluid_data.size);
        Diffuse2D(2, fluid_data.v_y0, fluid_data.v_y, fluid_data.visc, fluid_data.dt, iterations, fluid_data.size);

        Project2D(fluid_data.v_x0, fluid_data.v_y0, fluid_data.v_x, fluid_data.v_y, iterations, fluid_data.size);

        Advect2D(1, fluid_data.v_x, fluid_data.v_x0, fluid_data.v_x0, fluid_data.v_y0, fluid_data.dt, fluid_data.size);
        Advect2D(2, fluid_data.v_y, fluid_data.v_y0, fluid_data.v_x0, fluid_data.v_y0, fluid_data.dt, fluid_data.size);

        Project2D(fluid_data.v_x, fluid_data.v_y, fluid_data.v_x0, fluid_data.v_y0, iterations, fluid_data.size);

        Diffuse2D(0, fluid_data.s, fluid_data.density, fluid_data.diff, fluid_data.dt, iterations, fluid_data.size);
        Advect2D(0, fluid_data.density, fluid_data.s, fluid_data.v_x, fluid_data.v_y, fluid_data.dt, fluid_data.size);
    }
}
