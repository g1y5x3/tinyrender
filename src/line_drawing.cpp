#include <ctime>
#include "tgaimage.h"
#include "renderer.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    TGAImage framebuffer(64, 64, TGAImage::RGB);

    std::srand(std::time({}));
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%64, ay = rand()%64;
        int bx = rand()%64, by = rand()%64;
        line(ax, ay, bx, by, framebuffer, {static_cast<uint8_t>(rand()%255),
                                           static_cast<uint8_t>(rand()%255),
                                           static_cast<uint8_t>(rand()%255),
                                           static_cast<uint8_t>(rand()%255)});
    }

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}