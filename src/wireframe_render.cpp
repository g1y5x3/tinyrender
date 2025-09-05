#include "tgaimage.h"
#include "objmodel.h"

constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor red   = {255, 0,   0,   255};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {

    ObjModel model;
    model.load_obj_file("data/diablo3_pose.obj");


    return 0;
}