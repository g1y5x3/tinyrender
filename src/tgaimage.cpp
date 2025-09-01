#include "tgaimage.h"
#include <vector>

TGAImage::TGAImage(int w, int h, Format format) : 
    data(w * h * format, 0), width(w), height(h), bytespp(format) {}