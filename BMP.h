#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <tuple>

class NewArr;

class BMP {
public:
    struct TestHeader {
        unsigned char field1;
        unsigned short field2;
        unsigned long field3;
    };
    struct TestHeader2 {
        unsigned char field1;
        unsigned short field2;
        unsigned long field3;
    } __attribute__((__packed__));

public:
    struct BMPHeader {
        uint16_t signature;
        uint32_t file_size;
        uint16_t usless;
        uint16_t usless1;
        uint32_t bitarray_offset;
    } __attribute__((__packed__));

    struct DIBHeader {
        uint32_t dib_headersize;
        uint32_t width;
        uint32_t height;
        uint16_t color_planes;
        uint16_t pix_per_bit;
        uint32_t usless2;
        uint32_t raw_bitmap_data_size;
        uint64_t usless3;
        uint64_t usless4;
    } __attribute__((__packed__));

    struct Pixel {
        uint8_t blue = 0;
        uint8_t green = 0;
        uint8_t red = 0;

        bool operator==(const Pixel& rhv) const {
            return std::tie(blue, green, red) == std::tie(rhv.blue, rhv.green, rhv.red);
        }

        bool operator!=(const Pixel& rhv) const {
            return !(*this == rhv);
        }
    };

public:
    void SetFileWidth(size_t new_width) {
        dib_header.width = new_width;
    }

    void SetFileHeight(size_t new_height) {
        dib_header.height = new_height;
    }

    Pixel& GetPixel(size_t row, size_t column) {
        return pixels_.GetPixel(row, column);
    }

    const Pixel& GetPixel(size_t row, size_t column) const {
        return pixels_.GetPixel(row, column);
    }

    uint32_t GetHeight() const {
        return pixels_.GetHeight();
    }

    uint32_t GetWidth() const {
        return pixels_.GetWidth();
    }

    bool IsEmpty() const {
        return pixels_.IsEmpty();
    }

public:
    class PixelArray {
    public:
        PixelArray() {
        }

        void PushBack(Pixel pixel) {
            pixel_array_elements_.push_back(pixel);
        }

        Pixel& GetPixel(size_t row, size_t column) {
            return pixel_array_elements_[pixel_array_width * row + column];
        }

        void SetHeight(size_t row) {
            pixel_array_height_ = row;
        }

        void SetWidth(size_t column) {
            pixel_array_width = column;
        }

        const Pixel& GetPixel(size_t row, size_t column) const {
            return pixel_array_elements_[pixel_array_width * row + column];
        }

        uint32_t GetHeight() const {
            return pixel_array_height_;
        }

        uint32_t GetWidth() const {
            return pixel_array_width;
        }

        bool IsEmpty() const {
            return pixel_array_elements_.empty();
        }

        void Fill(size_t height, size_t width, uint8_t default_red = 0, uint8_t default_green = 0,
                  uint8_t default_blue = 0) {
            pixel_array_width = width;
            pixel_array_height_ = height;
            for (size_t i = 0; i < height * width; ++i) {
                Pixel pixel{default_blue, default_green, default_red};
                pixel_array_elements_.push_back(pixel);
            }
        }

    protected:
        size_t pixel_array_height_;
        size_t pixel_array_width;
        std::vector<Pixel> pixel_array_elements_;
    };

    void CopyPixels(const PixelArray& other) {
        pixels_ = other;
    }

protected:
    PixelArray pixels_;
    BMPHeader bmp_header;
    DIBHeader dib_header;

public:
    bool Load(std::istream& stream);

    bool Load(const char* file_name);

    bool WriteF(const char* file_name);

    bool WriteF(std::ostream& stream);
};

class NewArr {
public:
    NewArr() : storage_(nullptr), heigth(0), width(0) {
    }

    NewArr(size_t height, size_t width, BMP::Pixel default_pixel = BMP::Pixel()) : NewArr() {

        Resize(height, width, default_pixel);
    }

    NewArr(const NewArr& other);

    NewArr(NewArr&& other);

    ~NewArr() {
        FreeStorage();
    }

    NewArr& operator=(const NewArr& rhv);

    void Resize(size_t height, size_t width, BMP::Pixel default_pixel = BMP::Pixel());

    size_t GetHeight() const {
        return heigth;
    }

    size_t GetWidth() const {
        return width;
    }

    BMP::Pixel& operator()(size_t row, size_t column) {
        return GetPixel(storage_, width, row, column);
    }

    const BMP::Pixel& operator()(size_t row, size_t column) const {
        return GetPixel(storage_, width, row, column);
    }

    BMP::Pixel& At(size_t row, size_t column);

    const BMP::Pixel& At(size_t row, size_t column) const;

protected:
    void FreeStorage() {
        delete[] storage_;

        storage_ = nullptr;
        heigth = 0;
        width = 0;
    }

    BMP::Pixel* AllocateStorage(size_t height, size_t width) {
        return new BMP::Pixel[height * width];
    }

    void CopyStorage(BMP::Pixel* new_storage, size_t new_height, size_t new_width, BMP::Pixel default_pixel) const;

    void Swap(NewArr& other) noexcept {
        std::swap(storage_, other.storage_);
        std::swap(heigth, other.heigth);
        std::swap(width, other.width);
    }

    static BMP::Pixel& GetPixel(BMP::Pixel* storage, size_t width, size_t row, size_t column) {
        return storage[width * row + column];
    }

protected:
    BMP::Pixel* storage_;
    size_t heigth;
    size_t width;
};
