#include <fstream>
#include <sstream>
#include <iostream>
#include "objmodel.h"

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