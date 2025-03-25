#include "../include/buddy_allocator.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdint>

size_t BuddyAllocator::nextPowerOfTwo(size_t size) {
    size_t power = 1;
    while (power < size) {
        power *= 2;
    }
    return power;
}

void BuddyAllocator::insertBlock(size_t size, void* address) {
    freeBlocks[size].push_back({address, size});
}

BuddyAllocator::BuddyAllocator(size_t size) {
    totalSize = nextPowerOfTwo(size);
    memoryPool = std::malloc(totalSize);
    if (!memoryPool) {
        throw std::bad_alloc();
    }
    insertBlock(totalSize, memoryPool);
}

void* BuddyAllocator::allocate(size_t size) {
    size = nextPowerOfTwo(size);

    auto it = freeBlocks.lower_bound(size);
    if (it == freeBlocks.end()) {
        std::cerr << "No available memory for size: " << size << " bytes\n";
        return nullptr;
    }

    Block block = it->second.back();
    it->second.pop_back();
    if (it->second.empty()) {
        freeBlocks.erase(it);
    }

    while (block.size > size) {
        block.size /= 2;
        void* buddy = static_cast<char*>(block.address) + block.size;
        insertBlock(block.size, buddy);
    }

    return block.address;
}

void BuddyAllocator::deallocate(void* ptr, size_t size) {
    size = nextPowerOfTwo(size);

    void* buddy = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(ptr) ^ size
    );

    auto it = freeBlocks.find(size);
    if (it != freeBlocks.end()) {
        for (auto blockIt = it->second.begin(); blockIt != it->second.end(); ++blockIt) {
            if (blockIt->address == buddy) {
                it->second.erase(blockIt);
                deallocate(std::min(ptr, buddy), size * 2);
                return;
            }
        }
    }

    insertBlock(size, ptr);
}

BuddyAllocator::~BuddyAllocator() {
    std::free(memoryPool);
}
