#include <string>
#include <thread>
#include <chrono>
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <mutex>

#include <windows.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>

#include "FluidDataStructures.h"
#include "FluidHelperFunctions.h"
#include "FluidSim.h"

#include "DrawColors.h"
#include "DrawRect.h"

constexpr int32_t FLUID_SIZE = 160; // size of fluid domain represented as NxN
constexpr int32_t SCALE = 4; // size of pixel dye/density

std::mutex draw_mutex;
void fluid_update(FluidDataStructures::FluidCube * fluid, const sf::RenderWindow * window, const bool * enable_auto_fluid = nullptr, const bool * enable_pause = nullptr);
void auto_fluid(FluidDataStructures::FluidCube * fluid, const float & dt);

int main()
{
    // initialize and display window using sfml

    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    sf::VideoMode video_mode (FLUID_SIZE * SCALE, FLUID_SIZE * SCALE);
    std::string window_str_title = "Fluid Simulation";

    sf::RenderWindow window (video_mode, window_str_title, sf::Style::Titlebar | sf::Style::Close, context_settings);
    window.setFramerateLimit(60);

    // create text to draw

    bool show_text = true;

    sf::Text txt_control_pause;
    sf::Text txt_control_auto_fluid;
    sf::Text txt_control_display_type;
    sf::Font font;

    //if (!font.loadFromFile("Pixellettersfull-BnJ5.ttf"))
    //if (!font.loadFromFile("ArianaVioleta-dz2K.ttf"))
    if (!font.loadFromFile("ToThePointRegular-n9y4.ttf"))
    {
        show_text = false;
    }
    else
    {
        txt_control_pause.setFont(font);
        txt_control_auto_fluid.setFont(font);
        txt_control_display_type.setFont(font);

        txt_control_pause.setString("Press P key to pause/unpause");
        txt_control_auto_fluid.setString("Press A key to start/stop auto add fluid or left-click and hold drag mouse");
        txt_control_display_type.setString("Press B key to display fluid or vector field");

        txt_control_pause.setFillColor(sf::Color(sf::Color::White));
        txt_control_auto_fluid.setFillColor(sf::Color(sf::Color::White));
        txt_control_display_type.setFillColor(sf::Color(sf::Color::White));

        constexpr uint32_t txt_size = 22;
        txt_control_pause.setCharacterSize(txt_size);
        txt_control_auto_fluid.setCharacterSize(txt_size);
        txt_control_display_type.setCharacterSize(txt_size);

        txt_control_pause.setStyle(sf::Text::Bold);
        txt_control_auto_fluid.setStyle(sf::Text::Bold);
        txt_control_display_type.setStyle(sf::Text::Bold);

        txt_control_pause.setPosition(2, 0);
        txt_control_auto_fluid.setPosition(2, 24);
        txt_control_display_type.setPosition(2, 48);
    }

    // create sprite to draw

    DrawRect sprite(FLUID_SIZE * SCALE, FLUID_SIZE * SCALE, draw_colors::color_gray);

    // create fluid

    constexpr int32_t fluid_size = FLUID_SIZE;
    constexpr float fluid_diffusion = 2.0f / static_cast<float>((FLUID_SIZE*SCALE)*(FLUID_SIZE*SCALE));
    constexpr float fluid_viscosity = 2.0f / static_cast<float>((FLUID_SIZE*SCALE)*(FLUID_SIZE*SCALE));
    constexpr float fluid_dt = 1.0f / 60.0f;

    FluidDataStructures::FluidCube * fluid = FluidHelperFunctions::FluidCubeCreate2D(fluid_size, fluid_diffusion, fluid_viscosity, fluid_dt);

    // setup window interaction variables

    int32_t mouse_x = 0; // current mouse x location (relative to the window)
    int32_t mouse_y = 0; // current mouse y location (relative to the window)
    int32_t p_mouse_x; // previous mouse x location (relative to the window)
    int32_t p_mouse_y; // previous mouse y location (relative to the window)
    float mouse_x_dt; // mouse movement x
    float mouse_y_dt; // mouse movement y

    bool ignore_click = true; // ignore 1st click to avoid adding velocity when mouse is not moving
    bool view_velocity = false; // view velocity or density (density is the default)
    bool auto_fluid = false;
    bool pause_fluid = false;

    std::thread fluid_thread = std::thread(fluid_update, fluid, &window, &auto_fluid, &pause_fluid); // run fluid simulation thread

    // window/drawing loop

    while (window.isOpen())
    {
        sf::Event event {};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::B)
                {
                    view_velocity ^= true;
                }

                if (event.key.code == sf::Keyboard::A)
                {
                    auto_fluid ^= true;
                }

                if (event.key.code == sf::Keyboard::P)
                {
                    pause_fluid ^= true;
                }

                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            p_mouse_x = mouse_x;
            p_mouse_y = mouse_y;

            mouse_x = sf::Mouse::getPosition(window).x;
            mouse_y = sf::Mouse::getPosition(window).y;

            if (ignore_click) // ignore the 1st click to avoid getting a false dt value
            {
                mouse_x_dt = 0;
                mouse_y_dt = 0;
                ignore_click = false;
            }
            else
            {
                constexpr float dt_strength_factor = 1.0f; // increase the strength/amount of velocity from the mouse movement
                mouse_x_dt = static_cast<float>(mouse_x - p_mouse_x) * dt_strength_factor;
                mouse_y_dt = static_cast<float>(mouse_y - p_mouse_y) * dt_strength_factor;
            }

            // add density/dye and velocity to fluid

            if (!pause_fluid)
            {
                FluidHelperFunctions::FluidAddDensity(fluid, (mouse_x / SCALE), (mouse_y / SCALE), 0, 2, 4.0f);
                FluidHelperFunctions::FluidAddVelocity(fluid, (mouse_x / SCALE), (mouse_y / SCALE), 0, 2, static_cast<float>(mouse_x_dt), static_cast<float>(mouse_y_dt), 0.0f);
            }
        }
        else
        {
            ignore_click = true;
            mouse_x = 0;
            mouse_y = 0;
        }

        // set image to display either density or velocity

        {
            std::unique_lock<std::mutex> lock(draw_mutex);

            for (int32_t i = 0; i < (FLUID_SIZE * SCALE); i++)
            {
                for (int32_t j = 0; j < (FLUID_SIZE * SCALE); j++)
                {
                    if (!view_velocity)
                    {
                        if (fluid->density)
                        {
                            uint8_t cell_value = std::clamp(static_cast<uint8_t>(fluid->density[(j / SCALE) + ((i/SCALE) * (FLUID_SIZE))] * 255.0f), uint8_t(0), uint8_t(255));
                            sprite.LoadPixels(j, i, {draw_colors::color_sky_blue[0], draw_colors::color_sky_blue[1], draw_colors::color_sky_blue[2], cell_value});
                        }
                    }
                    else
                    {
                        if (fluid->v_x)
                        {
                            uint8_t cell_value_x = std::clamp(static_cast<uint8_t>(fluid->v_x[(j / SCALE) + ((i/SCALE) * (FLUID_SIZE))] * 255.0f), uint8_t(0), uint8_t(255));
                            uint8_t cell_value_y = std::clamp(static_cast<uint8_t>(fluid->v_y[(j / SCALE) + ((i/SCALE) * (FLUID_SIZE))] * 255.0f), uint8_t(0), uint8_t(255));
                            sprite.LoadPixels(j, i, {cell_value_x, cell_value_y, 0, 255});
                        }
                    }
                }
            }
        }

        // update image texture

        sprite.UpdatePixels();

        // draw text and fluid

        window.clear();
        window.draw(sprite());

        if (show_text)
        {
            window.draw(txt_control_pause);
            window.draw(txt_control_auto_fluid);
            window.draw(txt_control_display_type);
        }

        window.display();

        std::this_thread::sleep_for(std::chrono::microseconds (16667));
    }

    // check if it's okay to join and wait for the thread to complete if this is the case

    if (fluid_thread.joinable())
    {
      fluid_thread.join();
    }

    // deallocate data and exit

    FluidHelperFunctions::FluidCubeFree(fluid);

    return EXIT_SUCCESS;
}

void fluid_update(FluidDataStructures::FluidCube * fluid, const sf::RenderWindow * window, const bool * enable_auto_fluid, const bool * enable_pause)
{
    float dt = 0.0f;

    while (window->isOpen())
    {
        if ((enable_auto_fluid && *enable_auto_fluid) && (enable_pause && !(*enable_pause)))
        {
            auto_fluid(fluid, dt);
        }

        {
            if (enable_pause && !(*enable_pause))
            {
                dt += 1.0f/30.0f;
                std::unique_lock<std::mutex> lock(draw_mutex);
                FluidSim::FluidCubeStep2D(fluid);
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds(16667));
    }
}

void auto_fluid(FluidDataStructures::FluidCube * fluid, const float & dt)
{
    // create jet stream fluid

    const float vx = 5.0f;
    const float vy = 2.0f * std::sin(dt);

    FluidHelperFunctions::FluidAddVelocity(fluid, (FLUID_SIZE / 2), (FLUID_SIZE / 2), 0, 1, vx, vy, 0.0f);
    FluidHelperFunctions::FluidAddDensity(fluid, (FLUID_SIZE / 2), (FLUID_SIZE / 2), 0, 1, 4.0f);
}