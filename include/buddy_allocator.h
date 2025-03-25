#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <vector>
#include <map>
#include <cstddef>

class BuddyAllocator {
private:
    struct Block {
        void* address;
        size_t size;
    };

    size_t totalSize;
    void* memoryPool;
    std::map<size_t, std::vector<Block>> freeBlocks;

    size_t nextPowerOfTwo(size_t size);
    void insertBlock(size_t size, void* address);

public:
    explicit BuddyAllocator(size_t size);
    ~BuddyAllocator();

    void* allocate(size_t size);
    void deallocate(void* ptr, size_t size);
};

#endif // BUDDY_ALLOCATOR_H
