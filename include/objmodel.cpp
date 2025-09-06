#include <fstream>
#include <iostream>
#include <sstream>
#include "objmodel.h"
#include "renderer.h"

constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor red   = {255, 0,   0,   255};

// Load a Wavefront .obj file. Only supports vertex positions (v) and faces (f).
bool ObjModel::load_obj_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    // Clear any existing data
    vertices_.clear();

    size_t vertex_count = 0;
    size_t face_count = 0;

    std::string line;
    // First pass to count vertices
    while (std::getline(file, line)) {
        if (line.starts_with("v ")) {
            vertex_count++;
        } else if (line.starts_with("f ")) {
            face_count++;
        }
    }
    std::cout << "Number of vertices: " << vertex_count << std::endl;
    std::cout << "Number of faces: " << face_count << std::endl;

    if (vertex_count > 0) {
        vertices_.reserve(vertex_count);
    }
    if (face_count > 0) {
        faces_.reserve(face_count);
    }

    // Second pass to read vertex data
    file.clear(); // Clear EOF flag
    file.seekg(0); // Rewind to beginning of file

    while (std::getline(file, line)) {
        if (line.starts_with("v ")) {
            std::stringstream ss(line);
            char v;
            float x, y, z;
            if (!(ss >> v >> x >> y >> z)) {
                std::cerr << "Failed to parse vertex line: " << line << std::endl;
                continue;
            }
            vertices_.push_back(Vec3f{x, y, z});
        } else if (line.starts_with("f ")) {
            // right now only support faces with vertex indices only
            std::stringstream ss(line);
            char f;
            ss >> f;

            std::vector<int> face;
            std::string face_vertex_str;
            while (ss >> face_vertex_str) {
                std::stringstream face_ss(face_vertex_str);
                int vertex_index;
                if (!(face_ss >> vertex_index)) {
                    std::cerr << "Failed to parse face vertex: " << face_vertex_str << std::endl;
                    continue;
                }
                // std::cout << vertex_index << ' ';
                face.push_back(vertex_index - 1); // OBJ indices are 1-based
            }
            // std::cout << '\n';
            faces_.push_back(face);
        }
    }

    return true;
}

std::tuple<int, int> ObjModel::project_xy(const Vec3f& v, int width, int height) const {
    int x = static_cast<int>((v.x + 1.f) * width / 2.f);
    int y = static_cast<int>((v.y + 1.f) * height / 2.f);
    return {x, y};
}

bool ObjModel::render_wireframe(TGAImage& image) {
    // convert to screen space
    const int width = image.get_width();
    const int height = image.get_height();

    //render through xy plane
    for (size_t i = 0; i < faces_.size(); ++i) {
        const auto& face = faces_[i];

        // render the edges of the face
        auto [a0, b0] = project_xy(vertices_[face[0]], width, height);
        auto [a1, b1] = project_xy(vertices_[face[1]], width, height);
        auto [a2, b2] = project_xy(vertices_[face[2]], width, height);

        line(a0, b0, a1, b1, image, red);
        line(a1, b1, a2, b2, image, red);
        line(a2, b2, a0, b0, image, red);
    }

    for (size_t i = 0; i < vertices_.size(); ++i) {
        auto [x, y] = project_xy(vertices_[i], width, height);
        image.set(x, y, white);
    }
    
    return true;
}