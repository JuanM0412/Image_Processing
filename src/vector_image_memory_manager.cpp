#include "../include/vector_image_memory_manager.h"
#include <stdexcept>

void VectorImageMemoryManager::allocate(int w, int h) {
    width = w;
    height = h;
    pixels.resize(height, std::vector<PixelChar>(width));
}

IImageMemoryManager* VectorImageMemoryManager::clone() const {
    VectorImageMemoryManager* newManager = new VectorImageMemoryManager();
    newManager->width = width;
    newManager->height = height;
    newManager->pixels = pixels;
    return newManager;
}

PixelChar& VectorImageMemoryManager::getPixel(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return pixels[y][x];
}

void VectorImageMemoryManager::setPixel(int x, int y, const PixelChar& pixel) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    pixels[y][x] = pixel;
}

int VectorImageMemoryManager::getWidth() const {
    return width;
}

int VectorImageMemoryManager::getHeight() const {
    return height;
}
