#include <cmath>
#include <cstdlib>
#include <ctime>
#include "tgaimage.h"

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    float slope = (by-ay) / static_cast<float>(bx-ax);
    float y = ay;
    for (float x = ax; x <= bx; x++) {
        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
        y += slope;
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    TGAImage framebuffer(64, 64, TGAImage::RGB);

    std::srand(std::time({}));
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%64, ay = rand()%64;
        int bx = rand()%64, by = rand()%64;
        line(ax, ay, bx, by, framebuffer, {rand()%255, rand()%255, rand()%255, rand()%255});
    }

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}