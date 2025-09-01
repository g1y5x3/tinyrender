#pragma once

#include <cstdint>
#include <string>
#include <vector>

// A 24-bit TGA contains only an 18-byte header followed by the image data as packed
// RGB data.
// https://en.wikipedia.org/wiki/Truevision_TGA#Header
#pragma pack(push, 1)
struct TGA_Header {
    uint8_t idlength{};
    uint8_t colormaptype{};
    uint8_t datatypecode{};
    uint16_t colormaporigin{};
    uint16_t colormaplength{};
    uint8_t colormapdepth{};
    uint16_t x_origin{};
    uint16_t y_origin{};
    uint16_t width{};
    uint16_t height{};
    uint8_t bitsperpixel{};
    uint8_t imagedescriptor{};
};
#pragma pack(pop)

struct TGAColor {
    // in memory, this is stored as B, G, R, A
    uint8_t bgra[4] = {0, 0, 0, 255};
    uint8_t bytespp = 4; // bytes per pixel

    TGAColor() = default;

    TGAColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A=255) : 
        bgra{B, G, R, A}, bytespp(4) {}

    TGAColor(const uint8_t* p, uint8_t bpp) : bytespp(bpp) {
        for (int i = 0; i < bpp; ++i) {
            bgra[i] = p[i];
        }
    }

    uint8_t& operator[](const int i) { return bgra[i]; }
};

class TGAImage {
public:
    enum Format {
        GRAYSCALE = 1, RGB = 3, RGBA = 4
    };

    TGAImage() = default;
    TGAImage(int w, int h, Format format);

    bool write_tga_file(const std::string& filename, bool rle = false) const;
    bool set(int x, int y, const TGAColor& c);

private:
    std::vector<uint8_t> data;
    int width = 0;
    int height = 0;
    int bytespp = 0;
};