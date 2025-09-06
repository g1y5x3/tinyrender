#include "tgaimage.h"
#include "objmodel.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    ObjModel model("data/diablo3_pose.obj");
    TGAImage framebuffer(1000, 1000, TGAImage::RGB);
    model.render_wireframe(framebuffer);
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}