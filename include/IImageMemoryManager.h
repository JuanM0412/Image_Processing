#ifndef I_IMAGE_MEMORY_MANAGER_H
#define I_IMAGE_MEMORY_MANAGER_H

#include <vector>
#include "stb_image.h"
#include "stb_image_write.h"

struct PixelChar {
    unsigned char r, g, b;
};

class IImageMemoryManager {
    public:
        virtual ~IImageMemoryManager() = default;

        virtual void allocate(int width, int height) = 0;
        virtual PixelChar& getPixel(int x, int y) = 0;
        virtual void setPixel(int x, int y, const PixelChar& pixel) = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

        virtual IImageMemoryManager* clone() const = 0;
};

#endif // I_IMAGE_MEMORY_MANAGER_H
