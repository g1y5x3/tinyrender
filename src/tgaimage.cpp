#include <algorithm>
#include <cstring>
#include <fstream>
#include <format>
#include <iostream>
#include <span>
#include "tgaimage.h"

TGAImage::TGAImage(int w, int h, Format format) :
    data(w * h * static_cast<int>(format), 0),
    width(w),
    height(h),
    bytespp(static_cast<int>(format)) {}

bool TGAImage::write_tga_file(const std::string& filename, bool rle) const {
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << std::format("Failed to open file: {}\n", filename);
        return false;
    }

    TGA_Header header{};
    header.bitsperpixel = bytespp << 3;
    header.width = width;
    header.height = height;
    // TGA datatype codes from the specification:
    // 0  no image data is present
    // 1  uncompressed color-mapped image
    // 2  uncompressed true-color image
    // 3  uncompressed grayscale image
    // 9  run-length encoded color-mapped image
    // 10 run-length encoded true-color image
    // 11 run-length encoded grayscale image
    header.datatypecode = (bytespp == static_cast<int>(Format::GRAYSCALE) ? (rle ? 11 : 3) : (rle ? 10 : 2));
    header.imagedescriptor = 0x00; // bottom-left origin

    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (!out.good()) {
        std::cerr << "Failed to write header\n";
        return false;
    }

    if (!rle) {
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
        if (!out.good()) {
            std::cerr << "Failed to write data\n";
            return false;
        }
    } else {
        std::cerr << "RLE compression not implemented\n";
        return false;
    }

    constexpr char developer_area[4] = {0, 0, 0, 0};
    constexpr char extension_area[4] = {0, 0, 0, 0};
    constexpr char footer[18] = {'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.', '\0' };

    out.write(developer_area, sizeof(developer_area));
    if (!out.good()) {
        std::cerr << "Failed to write developer area\n";
        return false;
    }

    out.write(extension_area, sizeof(extension_area));
    if (!out.good()) {
        std::cerr << "Failed to write extension area\n";
        return false;
    }

    out.write(footer, sizeof(footer));
    if (!out.good()) {
        std::cerr << "Failed to write footer\n";
        return false;
    }

    return true;
}

bool TGAImage::flip_vertically() {
    if (data.empty()) return false;

    const size_t row_size = width * bytespp;
    auto top = data.begin();
    auto bottom = data.end() - row_size;

    for (int i = 0; i < height >> 1; ++i) {
        std::swap_ranges(top, top + row_size, bottom);
        top += row_size;
        bottom -= row_size;
    }
    return true;
}

bool TGAImage::flip_horizontally() {
    if (data.empty()) return false;

    for (int i = 0; i < height; ++i) {
        auto row = data.begin() + i * width * bytespp;

        for (int j = 0; j < width >> 1; ++j) {
            auto left_pixel = row + j * bytespp;
            auto right_pixel = row + (width - 1 - j) * bytespp;
            std::swap_ranges(left_pixel, left_pixel + bytespp, right_pixel);
        }
    }
    return true;
}

TGAColor TGAImage::get(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return TGAColor();
    }
    return TGAColor(data.data() + (x + y * width) * bytespp, bytespp);
}

bool TGAImage::set(int x, int y, const TGAColor& c) {
    if (x < 0 || y < 0 || x >= width || y>= height) {
        return false;
    }
    std::memcpy(data.data() + (x + y * width) * bytespp, c.bgra, bytespp);
    return true;
}