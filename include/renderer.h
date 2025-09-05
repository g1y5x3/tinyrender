#pragma once

#include "tgaimage.h"
#include "objmodel.h"

// Draws a line using Bresenham's line algorithm.
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

// // Draws a wireframe triangle by connecting its three vertices.
// void triangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, TGAImage& image, const TGAColor& color);
