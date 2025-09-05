#pragma once

#include <string>
#include <vector>

struct Vec3f {
    float x = 0.f, y = 0.f, z = 0.f;
};

class ObjModel{
public:
    ObjModel() = default;
    bool load_obj_file(const std::string& filename);

private:
    std::vector<Vec3f> vertices_;
    std::vector<std::vector<int>> faces_;
};