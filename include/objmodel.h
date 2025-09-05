#pragma once

#include <string>
#include <vector>
#include "tgaimage.h"

struct Vec3f {
    float x = 0.f, y = 0.f, z = 0.f;
};

class ObjModel{
public:
    ObjModel() = default;
    ObjModel(const std::string& filename) { load_obj_file(filename); }

    bool load_obj_file(const std::string& filename);
    bool render_wireframe(TGAImage& image);

    std::tuple<int, int> project_xy(const Vec3f& v, int width, int height) const;

private:
    std::vector<Vec3f> vertices_;
    std::vector<std::vector<int>> faces_;
};