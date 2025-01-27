#include "BMP.h"

#include <fstream>
#include <stdexcept>

bool BMP::Load(const char* file_name) {
    std::fstream file;
    file.open(file_name, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) {
        return false;
    }
    Load(file);
    file.close();
    return true;
}

bool BMP::Load(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
    stream.read(reinterpret_cast<char*>(&dib_header), sizeof(dib_header));

    const size_t RUBBISH_BETWEEN_PIXEL_LINES = (4 - (dib_header.width * 3) % 4) % 4;
    pixels_.SetHeight(dib_header.height);
    pixels_.SetWidth(dib_header.width);
    for (size_t row = 0; row < dib_header.height; ++row) {
        for (size_t column = 0; column < dib_header.width; ++column) {
            Pixel pixel{};
            stream.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
            pixels_.PushBack(pixel);
        }
        stream.ignore(RUBBISH_BETWEEN_PIXEL_LINES);
    }

    return true;
}

bool BMP::WriteF(const char* file_name) {
    std::ofstream file;
    file.open(file_name, std::ios_base::out | std::ios_base::binary);
    if (!file.is_open()) {
        return false;
    }
    WriteF(file);
    file.close();
    return true;
}

bool BMP::WriteF(std::ostream& stream) {
    stream.write(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
    stream.write(reinterpret_cast<char*>(&dib_header), sizeof(dib_header));

    const size_t RUBBISH_BETWEEN_PIXEL_LINES = (4 - (dib_header.width * 3) % 4) % 4;
    for (size_t row = 0; row < dib_header.height; ++row) {
        for (size_t column = 0; column < dib_header.width; ++column) {
            stream.write(reinterpret_cast<char*>(&pixels_.GetPixel(row, column)),
                         sizeof(pixels_.GetPixel(row, column)));
        }
        static const uint32_t RUBBISH = 0x55AA55AA;
        stream.write(reinterpret_cast<const char*>(&RUBBISH), RUBBISH_BETWEEN_PIXEL_LINES);
    }
    return true;
}

NewArr::NewArr(const NewArr& other) : NewArr() {
    if (!other.storage_) {
        return;
    }
    storage_ = AllocateStorage(other.heigth, other.width);
    heigth = other.heigth;
    width = other.width;
    other.CopyStorage(storage_, heigth, width, {0, 0, 0});
}

NewArr& NewArr::operator=(const NewArr& rhv) {
    if (&rhv == this) {
        return *this;
    }
    NewArr temporary(rhv);
    Swap(temporary);
    return *this;
}

void NewArr::Resize(size_t height, size_t width, BMP::Pixel default_pixel) {
    if (heigth == height && width == width) {
        return;
    }
    if (height == 0 || width == 0) {
        FreeStorage();
        return;
    }
    BMP::Pixel* new_pixels = AllocateStorage(height, width);
    CopyStorage(new_pixels, height, width, default_pixel);
    FreeStorage();
    storage_ = new_pixels;
    heigth = height;
    width = width;
}

void NewArr::CopyStorage(BMP::Pixel* new_storage, size_t new_height, size_t new_width, BMP::Pixel default_pixel) const {
    for (size_t row = 0; row < new_height; ++row) {
        for (size_t column = 0; column < new_width; ++column) {
            if (row < heigth && column < width) {
                GetPixel(new_storage, new_width, row, column) = GetPixel(storage_, width, row, column);
            } else {
                GetPixel(new_storage, new_width, row, column) = default_pixel;
            }
        }
    }
}

BMP::Pixel& NewArr::At(size_t row, size_t column) {
    if (row >= heigth || column >= width) {
        throw std::out_of_range("Invalid row or column");
    }
    return GetPixel(storage_, width, row, column);
}

const BMP::Pixel& NewArr::At(size_t row, size_t column) const {
    if (row >= heigth || column >= width) {
        throw std::out_of_range("Invalid row or column");
    }
    return GetPixel(storage_, width, row, column);
}

NewArr::NewArr(NewArr&& other) {
    storage_ = other.storage_;
    width = other.width;
    heigth = other.heigth;
    other.storage_ = nullptr;
    other.width = 0;
    other.heigth = 0;
}
