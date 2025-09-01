#include <iostream>
#include <string>
#include "tgaimage.h"

const TGAColor white{255, 255, 255, 255};
const TGAColor red{255, 0,   0,   255};

int main([[maybe_unused]]  int argc, [[maybe_unused]] char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    image.set(52, 41, red);

    const std::string filename = "output.tga";
    if (image.write_tga_file(filename)) {
        std::cout << "Successfully wrote image to " << filename << std::endl;
    } else {
        std::cerr << "Failed to write image to " << filename << std::endl;
        return 1;
    }

    return 0;
}