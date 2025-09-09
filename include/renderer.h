#pragma once

#include "tgaimage.h"
#include "objmodel.h"

// Draws a line using Bresenham's line algorithm.
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& image, const TGAColor& color);
