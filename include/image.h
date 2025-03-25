#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <iostream>
#include "IImageMemoryManager.h"
#include "arg_parser.h"
#include "vector_image_memory_manager.h"
#include "buddy_image_memory_manager.h"

class Image {
    public:
        Image() = default;
        explicit Image(IImageMemoryManager* memManager);
        Image(const Image& other);
        Image(Image&& other) noexcept;
        
        Image& operator=(const Image& other);
        Image& operator=(Image&& other) noexcept;
        
        ~Image();
    
        bool loadImage(const std::string &inputImageName);
        bool saveImage(const std::string &outputImageName);
        
        static Image scaleImage(float xScale, float yScale, Image &originalImage, Mode mode = Mode::CONVENTIONAL);
        static Image rotateImage(float angle, Image &originalImage, Mode mode = Mode::CONVENTIONAL);
        
        int getWidth() const { return width; }
        int getHeight() const { return height; }
    
    private:
        int width = 0;
        int height = 0;
        IImageMemoryManager* memoryManager = nullptr;
};

#endif // IMAGE_H
