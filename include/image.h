#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <cstddef>
#include "stb_image.h"
#include "stb_image_write.h"

struct PixelChar {
    unsigned char r, g, b;
};

struct PixelInt {
    int r, g, b;
};

class Image {
    public:
        Image();
        ~Image();

        bool loadImage(const std::string &inputImageName);
        bool SaveImage(const std::string &outputImageName);

    private:
        int width, height;

        std::vector<std::vector<PixelChar>> pixelesChar;
        std::vector<std::vector<PixelInt>> pixelesInt;
};

#endif
