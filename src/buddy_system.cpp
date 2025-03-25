#include "../include/buddy_system.h"
#include <cstring>
#include <algorithm>
#include <cassert>

BuddySystem::BuddySystem(size_t total_memory_size, size_t minimum_block_size) {
    min_block_size = 1;
    while (min_block_size < minimum_block_size) {
        min_block_size *= 2;
    }

    total_size = 1;
    while (total_size < total_memory_size) {
        total_size *= 2;
    }

    max_level = 0;
    size_t size = total_size;
    while (size >= min_block_size) {
        size /= 2;
        max_level++;
    }

    memory = std::malloc(total_size);
    if (!memory) {
        throw std::bad_alloc();
    }

    free_lists.resize(max_level + 1);
    
    free_lists[0].push_back(Block(memory, total_size, true));
}

BuddySystem::~BuddySystem() {
    if (memory) {
        std::free(memory);
    }
}

size_t BuddySystem::getLevel(size_t size) const {
    size_t level = 0;
    size_t block_size = total_size;
    
    while (block_size > size && level < max_level) {
        block_size /= 2;
        level++;
    }
    
    if (block_size < size) {
        level--; 
    }
    
    return level;
}

void BuddySystem::split(size_t level, size_t index) {
    if (level >= max_level) {
        std::cerr << "Error: Cannot split block at max level.\n";
        return;
    }
    
    Block& block = free_lists[level][index];
    if (!block.free) {
        std::cerr << "Error: Block is not free, cannot split.\n";
        return;
    }
    
    size_t new_size = block.size / 2;
    
    void* second_addr = static_cast<char*>(block.address) + new_size;
    Block first_block(block.address, new_size, true);
    Block second_block(second_addr, new_size, true);
    
    free_lists[level].erase(free_lists[level].begin() + index);
    
    free_lists[level + 1].push_back(first_block);
    free_lists[level + 1].push_back(second_block);
    
    std::cout << "Split block at level " << level << " into two blocks at level " << level + 1 << "\n";
}

void BuddySystem::merge(size_t level, size_t index) {
    if (level == 0) return;
    
    size_t buddy_index = index ^ 1; // XOR to find the buddy
    if (buddy_index >= free_lists[level].size()) return;
    
    Block& block = free_lists[level][index];
    Block& buddy = free_lists[level][buddy_index];
    
    if (!block.free || !buddy.free) return;
    
    size_t block_addr = reinterpret_cast<size_t>(block.address);
    size_t buddy_addr = reinterpret_cast<size_t>(buddy.address);
    size_t parent_addr = block_addr & ~(block.size);
    
    if (parent_addr != (buddy_addr & ~(buddy.size))) return;
    
    void* parent_ptr = reinterpret_cast<void*>(parent_addr);
    Block parent_block(parent_ptr, block.size * 2, true);
    
    if (index > buddy_index) {
        free_lists[level].erase(free_lists[level].begin() + index);
        free_lists[level].erase(free_lists[level].begin() + buddy_index);
    } else {
        free_lists[level].erase(free_lists[level].begin() + buddy_index);
        free_lists[level].erase(free_lists[level].begin() + index);
    }
    
    free_lists[level - 1].push_back(parent_block);
    
    for (size_t i = 0; i < free_lists[level - 1].size(); i++) {
        if (free_lists[level - 1][i].address == parent_ptr) {
            merge(level - 1, i);
            break;
        }
    }
}

void* BuddySystem::allocateFromLevel(size_t level, size_t size) {
    for (size_t i = 0; i < free_lists[level].size(); i++) {
        if (free_lists[level][i].free) {
            Block& block = free_lists[level][i];
            block.free = false;
            std::cout << "Allocated block of size " << block.size << " at level " << level << "\n";
            return block.address;
        }
    }
    
    if (level == 0) {
        std::cerr << "Error: No more memory available in the Buddy System.\n";
        return nullptr; 
    }
    
    size_t parent_level = level - 1;
    
    for (size_t i = 0; i < free_lists[parent_level].size(); i++) {
        if (free_lists[parent_level][i].free) {
            split(parent_level, i);
            
            for (size_t j = 0; j < free_lists[level].size(); j++) {
                if (free_lists[level][j].free) {
                    free_lists[level][j].free = false;
                    std::cout << "Allocated block of size " << free_lists[level][j].size << " at level " << level << "\n";
                    return free_lists[level][j].address;
                }
            }
        }
    }
    
    void* result = allocateFromLevel(parent_level, size * 2);
    if (!result) {
        std::cerr << "Error: Could not allocate memory from higher level.\n";
        return nullptr;
    }
    
    return result;
}

void* BuddySystem::allocate(size_t size) {
    std::cout << "Allocating " << size << " bytes\n";
    printStatus(); 
    size = std::max(size, static_cast<size_t>(1));
    
    size_t padded_size = 1;
    while (padded_size < size) {
        padded_size *= 2;
    }
    
    padded_size = std::max(padded_size, min_block_size);
    
    if (padded_size > total_size) {
        return nullptr;
    }
    
    size_t level = getLevel(padded_size);
    
    return allocateFromLevel(level, padded_size);
}

void BuddySystem::deallocate(void* ptr) {
    std::cout << "Deallocating memory block\n";
    printStatus();
    if (!ptr) return;
    
    for (size_t level = 0; level <= max_level; level++) {
        for (size_t i = 0; i < free_lists[level].size(); i++) {
            if (free_lists[level][i].address == ptr) {
                free_lists[level][i].free = true;
                merge(level, i);
                return;
            }
        }
    }
}

void BuddySystem::printStatus() const {
    std::cout << "Buddy System Status:" << std::endl;
    std::cout << "Total Memory: " << total_size << " bytes" << std::endl;
    std::cout << "Minimum Block Size: " << min_block_size << " bytes" << std::endl;
    
    for (size_t level = 0; level <= max_level; level++) {
        size_t block_size = total_size / (1 << level);
        std::cout << "Level " << level << " (Block Size: " << block_size << "): ";
        
        for (const auto& block : free_lists[level]) {
            std::cout << (block.free ? "F" : "A") << " ";
        }
        std::cout << std::endl;
    }
}