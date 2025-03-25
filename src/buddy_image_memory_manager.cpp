#include <cstring>
#include <iostream>
#include "../include/buddy_image_memory_manager.h"

BuddyImageMemoryManager::BuddyImageMemoryManager(size_t totalMemorySize)
    : allocator(totalMemorySize) { printf("BuddyImageMemoryManager\n"); }

BuddyImageMemoryManager::~BuddyImageMemoryManager() {
    if (pixels) {
        allocator.deallocate(pixels, allocatedSize);
    }
}

void BuddyImageMemoryManager::allocate(int w, int h) {
    width = w;
    height = h;
    
    allocatedSize = width * height * sizeof(PixelChar);
    
    try {
        pixels = static_cast<PixelChar*>(allocator.allocate(allocatedSize));
        
        std::memset(pixels, 0, allocatedSize);
    } catch (const std::exception& e) {
        std::cerr << "Memory allocation failed in BuddyImageMemoryManager: " 
                  << e.what() << std::endl;
        throw;
    }
}

PixelChar& BuddyImageMemoryManager::getPixel(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return pixels[y * width + x];
}

void BuddyImageMemoryManager::setPixel(int x, int y, const PixelChar& pixel) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    pixels[y * width + x] = pixel;
}

IImageMemoryManager* BuddyImageMemoryManager::clone() const {
    auto* newManager = new BuddyImageMemoryManager(width * height * sizeof(PixelChar));
    
    newManager->allocate(width, height);
    std::memcpy(newManager->pixels, pixels, width * height * sizeof(PixelChar));
    
    return newManager;
}