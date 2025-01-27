#include "filters.h"
#include "BMP.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace ColourHelpers {
void ColorizePixel(BMP::Pixel& pixel, float red = 0, float green = 0, float blue = 0) {
    pixel.red = std::min(255, std::max(0, static_cast<int32_t>(red)));
    pixel.green = std::min(255, std::max(0, static_cast<int32_t>(green)));
    pixel.blue = std::min(255, std::max(0, static_cast<int32_t>(blue)));
}

template <typename T>
void ApplyMatrix(std::vector<std::vector<T>>& matrix, BMP& bitmap) {

    struct CurrentColour {
        T red = 0;
        T green = 0;
        T blue = 0;
    };

    BMP::PixelArray new_pixels;
    new_pixels.SetHeight(bitmap.GetHeight());
    new_pixels.SetWidth(bitmap.GetWidth());

    int32_t edge_value_x = matrix.size() / 2;
    int32_t edge_value_y = matrix[0].size() / 2;
    for (int32_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (int32_t column = 0; column < bitmap.GetWidth(); ++column) {
            BMP::Pixel pixel{};
            CurrentColour cur_colour;

            for (int32_t dx = -edge_value_x; dx <= edge_value_x; ++dx) {
                for (int32_t dy = -edge_value_y; dy <= edge_value_y; ++dy) {
                    int32_t x = row + dx;
                    int32_t y = column + dy;
                    x = std::min(std::max(x, 0), static_cast<int32_t>(bitmap.GetHeight()) - 1);
                    y = std::min(std::max(y, 0), static_cast<int32_t>(bitmap.GetWidth()) - 1);

                    cur_colour.red += bitmap.GetPixel(x, y).red * matrix[dx + edge_value_x][dy + edge_value_y];
                    cur_colour.green += bitmap.GetPixel(x, y).green * matrix[dx + edge_value_x][dy + edge_value_y];
                    cur_colour.blue += bitmap.GetPixel(x, y).blue * matrix[dx + edge_value_x][dy + edge_value_y];
                }
            }
            ColorizePixel(pixel, cur_colour.red, cur_colour.green, cur_colour.blue);
            new_pixels.PushBack(pixel);
        }
    }
    bitmap.CopyPixels(new_pixels);
}
}

void Crop::Apply(BMP& bitmap) {
    if (new_height_ > bitmap.GetHeight()) {
        new_height_ = bitmap.GetHeight();
    } else {
        bitmap.SetFileHeight(new_height_);
    }
    if (new_width_ > bitmap.GetWidth()) {
        new_width_ = bitmap.GetWidth();
    } else {
        bitmap.SetFileWidth(new_width_);
    }
    BMP::PixelArray new_pixels;
    new_pixels.SetHeight(new_height_);
    new_pixels.SetWidth(new_width_);
    for (size_t row = bitmap.GetHeight() - new_height_; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < new_width_; ++column) {
            new_pixels.PushBack(bitmap.GetPixel(row, column));
        }
    }
    bitmap.CopyPixels(new_pixels);
}

void Neg::Apply(BMP& bitmap) {
    for (size_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < bitmap.GetWidth(); ++column) {
            ColourHelpers::ColorizePixel(bitmap.GetPixel(row, column), 255.0f - bitmap.GetPixel(row, column).red,
                                         255.0f - bitmap.GetPixel(row, column).green,
                                         255.0f - bitmap.GetPixel(row, column).blue);
        }
    }
}

void GreyScaleFilter::Apply(BMP& bitmap) {
    for (size_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < bitmap.GetWidth(); ++column) {
            uint8_t new_colour = bitmap.GetPixel(row, column).red * 0.299 + bitmap.GetPixel(row, column).blue * 0.114 +
                                 bitmap.GetPixel(row, column).green * 0.587;
            ColourHelpers::ColorizePixel(bitmap.GetPixel(row, column), new_colour, new_colour, new_colour);
        }
    }
}

void SharpeningFilter::Apply(BMP& bitmap) {
    ColourHelpers::ApplyMatrix(sharpening_matrix_, bitmap);
}
