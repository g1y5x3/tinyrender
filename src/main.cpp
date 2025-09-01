#include "tgaimage.h"

const TGAColor white{255, 255, 255, 255};
const TGAColor red{255, 0,   0,   255};

int main([[maybe_unused]]  int argc, [[maybe_unused]] char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    return 0;
}