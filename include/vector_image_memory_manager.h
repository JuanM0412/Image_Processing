#ifndef VECTOR_IMAGE_MEMORY_MANAGER_H
#define VECTOR_IMAGE_MEMORY_MANAGER_H

#include "IImageMemoryManager.h"
#include <vector>

class VectorImageMemoryManager : public IImageMemoryManager {
    public:
        VectorImageMemoryManager() { printf("VectorImageMemoryManager\n"); };
        
        void allocate(int w, int h) override;
        PixelChar& getPixel(int x, int y) override;
        void setPixel(int x, int y, const PixelChar& pixel) override;
        int getWidth() const override;
        int getHeight() const override;
        
        IImageMemoryManager* clone() const override;
    
    private:
        int width = 0;
        int height = 0;
        std::vector<std::vector<PixelChar>> pixels;
};

#endif // VECTOR_IMAGE_MEMORY_MANAGER_H
