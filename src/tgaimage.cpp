#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ranges>
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
        std::cerr << "Failed to open file: " << filename << '\n';
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
        if (!write_rle_data(out)) {
            std::cerr << "Failed to write RLE data\n";
            return false;
        }
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

bool TGAImage::write_rle_data(std::ofstream& out) const {
    std::span<const std::uint8_t> image_data{data};
    constexpr size_t max_chunk_length = 128;

    while(!image_data.empty()) {
        auto first_pixel = image_data.subspan(0, bytespp);
        size_t chunk_start = 0;

        // Check if we should start a run-length packet or a raw packet
        bool is_run_length = false;
        if (image_data.size() >= 2 * static_cast<size_t>(bytespp)) {
            auto second_pixel = image_data.subspan(bytespp, bytespp);
            if (std::ranges::equal(first_pixel, second_pixel)) {
                is_run_length = true;
            }
        }

        if (is_run_length) {
            size_t run_length = 2;
            while (run_length < max_chunk_length && image_data.size() > run_length * bytespp) {
                auto next_pixel = image_data.subspan(run_length * bytespp, bytespp);
                if (!std::ranges::equal(first_pixel, next_pixel)) {
                    break;
                }
                run_length++;
            }
            // Write the run-length packet header and one pixel of data
            out.put(static_cast<unsigned char>((run_length - 1) | 0x80));
            out.write(reinterpret_cast<const char*>(first_pixel.data()), bytespp);
            chunk_start += run_length * bytespp;
        } else {
            // Raw packet, a run of three or more identical pixels is when RLE becomes more efficient
            size_t raw_length = 1;
            while (raw_length < max_chunk_length && image_data.size() > raw_length * bytespp) {
                if (image_data.size() >= (raw_length + 2) * bytespp) {
                    auto p1 = image_data.subspan((raw_length - 1) * bytespp, bytespp);
                    auto p2 = image_data.subspan(raw_length * bytespp, bytespp);
                    auto p3 = image_data.subspan((raw_length + 1) * bytespp, bytespp);
                    if (std::ranges::equal(p1, p2) && std::ranges::equal(p2, p3)) {
                        break;
                    }
                }
                raw_length++;
            }
            // Write the raw packet header and the pixel data
            out.put(static_cast<unsigned char>(raw_length - 1));
            auto chunk_to_write = image_data.subspan(0, raw_length * bytespp);
            out.write(reinterpret_cast<const char*>(image_data.data()), raw_length * bytespp);
            chunk_start += raw_length * bytespp;
        }

        if (!out) return false;
        // Advance to the next uncompressed pixel
        image_data = image_data.subspan(chunk_start);
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