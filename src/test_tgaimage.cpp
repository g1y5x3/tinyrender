#include <iostream>
#include <string>
#include "tgaimage.h"

constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor red   = {255, 0,   0,   255};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    image.set(75, 75, red);

    image.write_tga_file("output_uncompressed.tga", false);
    image.write_tga_file("output_compressed.tga");

    image.flip_vertically();
    image.write_tga_file("output_compressed_vertflip.tga");

    image.flip_horizontally();
    image.write_tga_file("output_compressed_vertflip_horiflip.tga");

    return 0;
}