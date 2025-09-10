#include "tgaimage.h"

constexpr int width  = 128;
constexpr int height = 128;

constexpr TGAColor red   = {255,   0,   0, 255};
constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor green = {  0, 255,   0, 255};

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& image, const TGAColor& color) {
    if (ay > by) { std::swap(ax, bx); std::swap(ay, by); }
    if (ay > cy) { std::swap(ax, cx); std::swap(ay, cy); }
    if (by > cy) { std::swap(bx, cx); std::swap(by, cy); }
    line(ax, ay, bx, by, image, green);
    line(bx, by, cx, cy, image, green);
    line(cx, cy, ax, ay, image, red);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    triangle(  7, 45, 35, 100, 45,  60, framebuffer, red);
    triangle(120, 35, 90,   5, 45, 110, framebuffer, white);
    triangle(115, 83, 80,  90, 85, 120, framebuffer, green);

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}