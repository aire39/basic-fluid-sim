//
// Created by sean on 6/30/2022.
//

#include "DrawRect.h"
#include <iostream>

DrawRect::DrawRect(int32_t width, int32_t height)
{
    Create(width, height);
}

DrawRect::DrawRect(int32_t width, int32_t height, std::array<uint8_t, 4> color)
{
    Create(width, height, color);
}

bool DrawRect::Create(int32_t width, int32_t height, std::array<uint8_t, 4> color)
{
    image.create(width, height, sf::Color(color[0], color[1], color[2], color[3]));
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    imageBufferSize = width * height * sizeof (color);
    imageWidth = width;
    imageHeight = height;

    return true;
}

void DrawRect::SetPixel(int32_t x, int32_t y, std::array<uint8_t, 4> color)
{
    image.setPixel(x, y, sf::Color(color[0], color[1], color[2], color[3]));
    texture.update(image);
}

void DrawRect::LoadPixels(int32_t x, int32_t y, std::array<uint8_t, 4> color)
{
    image.setPixel(x, y, sf::Color(color[0], color[1], color[2], color[3]));
}

void DrawRect::UpdatePixels()
{
    texture.update(image);
}

void DrawRect::SetPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void DrawRect::SetScale(float scale_factor)
{
    sprite.setScale(scale_factor, scale_factor);
}

void DrawRect::SetScale(float scale_factor_x, float scale_factor_y)
{
    sprite.setScale(scale_factor_x, scale_factor_y);
}

void DrawRect::ClearImage(std::array<uint8_t, 4> clear_color)
{
    if ((image.getSize().x > 0) && (image.getSize().y > 0))
    {
        Create(imageWidth, imageHeight, clear_color);
    }
    else
    {
        std::cerr << image.getSize().x << " : " << image.getSize().y << "\n";
        std::cerr << "unable to clear DrawRect. DrawRect has not been created. Try using DrawRect::Create(int32_t width, int32_t height, std::array<uint8_t, 4> color)\n";
    }
}

size_t DrawRect::GetBufferSize() const
{
    return imageBufferSize;
}

const uint8_t * DrawRect::ImageData() const
{
    return image.getPixelsPtr();
}
