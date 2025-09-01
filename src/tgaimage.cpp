#include <cstring>
#include <fstream>
#include <iostream>
#include "tgaimage.h"

TGAImage::TGAImage(int w, int h, Format format) :
    data(w * h * format, 0), width(w), height(h), bytespp(format) {}

bool TGAImage::write_tga_file(const std::string& filename, bool rle) const {
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    TGA_Header header{};
    header.bitsperpixel = bytespp << 3;
    header.width = width;
    header.height = height;
    /*
    0  no image data is present
    1  uncompressed color-mapped image
    2  uncompressed true-color image
    3  uncompressed grayscale image
    9  run-length encoded color-mapped image
    10 run-length encoded true-color image
    11 run-length encoded grayscale image
    */
    header.datatypecode = (bytespp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));

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

    const char footer[26] = {'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-',
                             'X', 'F', 'I', 'L', 'E', '.', '\0' };
    out.write(footer, sizeof(footer));
    if (!out.good()) {
        std::cerr << "Failed to write footer\n";
        return false;
    }

    return true;
}

bool TGAImage::set(int x, int y, const TGAColor& c) {
    if (x < 0 || y < 0 || x >= width || y>= height) {
        return false;
    }
    std::memcpy(data.data() + (x + y * width) * bytespp, c.bgra, bytespp);
    return true;
}