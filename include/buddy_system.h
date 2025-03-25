// buddy_system.h
#ifndef BUDDY_SYSTEM_H
#define BUDDY_SYSTEM_H

#include <vector>
#include <cstddef>
#include <cmath>
#include <iostream>

class BuddySystem {
private:
    struct Block {
        void* address;
        size_t size;
        bool free;
        Block(void* addr, size_t sz, bool f) : address(addr), size(sz), free(f) {}
    };

    void* memory;
    size_t total_size;
    size_t min_block_size;
    std::vector<std::vector<Block>> free_lists;
    size_t max_level;

    size_t getLevel(size_t size) const;
    void split(size_t level, size_t index);
    void merge(size_t level, size_t index);
    void* allocateFromLevel(size_t level, size_t size);

public:
    size_t getMinBlockSize() const { return min_block_size; }
    BuddySystem(size_t total_memory_size, size_t minimum_block_size);
    ~BuddySystem();

    void* allocate(size_t size);
    void deallocate(void* ptr);
    void printStatus() const;
};

#endif // BUDDY_SYSTEM_H