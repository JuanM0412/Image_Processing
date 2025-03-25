// In include/buddy_image_memory_manager.h
#ifndef BUDDY_IMAGE_MEMORY_MANAGER_H
#define BUDDY_IMAGE_MEMORY_MANAGER_H

#include "IImageMemoryManager.h"
#include "buddy_allocator.h"
#include <cstddef>
#include <stdexcept>

class BuddyImageMemoryManager : public IImageMemoryManager {
public:
    explicit BuddyImageMemoryManager(size_t totalMemorySize);
    ~BuddyImageMemoryManager() override;

    void allocate(int w, int h) override;
    PixelChar& getPixel(int x, int y) override;
    void setPixel(int x, int y, const PixelChar& pixel) override;
    int getWidth() const override { return width; }
    int getHeight() const override { return height; }

    IImageMemoryManager* clone() const override;

private:
    int width = 0;
    int height = 0;
    PixelChar* pixels = nullptr;
    BuddyAllocator allocator;
    size_t allocatedSize = 0;
};

#endif // BUDDY_IMAGE_MEMORY_MANAGER_H