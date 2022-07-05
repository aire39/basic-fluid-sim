#pragma once

#include <cstdint>
#include <array>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "DrawColors.h"

class DrawRect
{
    private:
        sf::Image image;
        sf::Texture texture;
        sf::Sprite sprite;

        int32_t imageWidth = 0;
        int32_t imageHeight = 0;
        size_t imageBufferSize = 0;

    public:
        DrawRect() = default;
        DrawRect(int32_t width, int32_t height);
        DrawRect(int32_t width, int32_t height, std::array<uint8_t, 4> color);
        ~DrawRect() = default;

        bool Create(int32_t width, int32_t height, std::array<uint8_t, 4> color = draw_colors::color_black);

        void SetPixel(int32_t x, int32_t y, std::array<uint8_t, 4> color);

        void LoadPixels(int32_t x, int32_t y, std::array<uint8_t, 4> color);
        void UpdatePixels();

        void SetPosition(float x, float y);
        void SetScale(float scale_factor);
        void SetScale(float scale_factor_x, float scale_factor_y);

        void ClearImage(std::array<uint8_t, 4> clear_color = draw_colors::color_black);

        size_t GetBufferSize() const;
        const uint8_t * ImageData() const;

        sf::Sprite & operator ()()
        {
            return sprite;
        }
};
