#pragma once

#include "BMP.h"

class BaseFilter {
public:
    virtual void Apply(BMP& bitmap) = 0;
};

class Crop : public BaseFilter {
public:
    Crop(size_t height, size_t width) : new_height_(height), new_width_(width) {
    }

    void Apply(BMP& bitmap) override;

protected:
    size_t new_height_;
    size_t new_width_;
};

class Neg : public BaseFilter {
public:
    void Apply(BMP& bitmap) override;
};

class GreyScaleFilter : public BaseFilter {
public:
    void Apply(BMP& bitmap) override;
};

class SharpeningFilter : public BaseFilter {
public:
    void Apply(BMP& bitmap) override;

protected:
    std::vector<std::vector<int>> sharpening_matrix_ = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
};
