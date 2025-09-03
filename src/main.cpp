#include <iostream>
#include <string>
#include "tgaimage.h"

const TGAColor white{255, 255, 255, 255};
const TGAColor red{255, 0,   0,   255};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    image.set(75, 75, red);

    bool use_rle = false;
    std::string filename = "output_uncompressed.tga";
    if (image.write_tga_file(filename, use_rle)) {
        std::cout << "Successfully wrote image to " << filename << std::endl;
    } else {
        std::cerr << "Failed to write image to " << filename << std::endl;
        return 1;
    }

    use_rle = true;
    filename = "output_compressed.tga";
    if (image.write_tga_file(filename, use_rle)) {
        std::cout << "Successfully wrote image to " << filename << std::endl;
    } else {
        std::cerr << "Failed to write image to " << filename << std::endl;
        return 1;
    }

    image.flip_vertically();
    filename = "output_compressed_vertflip.tga";
    if (image.write_tga_file(filename, use_rle)) {
        std::cout << "Successfully wrote image to " << filename << std::endl;
    } else {
        std::cerr << "Failed to write image to " << filename << std::endl;
        return 1;
    }

    image.flip_horizontally();
    filename = "output_compressed_horiflip.tga";
    if (image.write_tga_file(filename, use_rle)) {
        std::cout << "Successfully wrote image to " << filename << std::endl;
    } else {
        std::cerr << "Failed to write image to " << filename << std::endl;
        return 1;
    }

    return 0;
}