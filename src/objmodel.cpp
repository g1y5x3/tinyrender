#include <fstream>
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

    std::string line;
    // First pass to count vertices
    while (std::getline(file, line)) {
        if (line.starts_with("v ")) {
            vertex_count++;
        }
    }

    std::cout << "Number of vertices: " << vertex_count << std::endl;

    if (vertex_count > 0) {
        vertices_.reserve(vertex_count);
    }

    return true;
}