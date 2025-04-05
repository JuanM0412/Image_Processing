# Time and Memory Complexity Analysis
## Image Operations
### Image Loading (loadImage)
- Time Complexity: O(W × H)
Iterates through each pixel once to transfer from the STB buffer to the memory manager.

- Memory Complexity: O(W × H)
Uses temporary STB buffer and final internal storage.

### Image Scaling (scaleImage)
- Time Complexity: O(W_new × H_new)
For each output pixel, it computes the corresponding input pixel (nearest-neighbor).

- Memory Complexity: O(W_new × H_new)
Allocates a new image of the scaled dimensions.

### Image Rotation (rotateImage)
- Time Complexity: O(W_rot × H_rot)
Processes each output pixel to map to an input pixel.

- Memory Complexity: O(W_rot × H_rot)
Allocates memory for the rotated image.

### Image Saving (saveImage)
- Time Complexity: O(W × H)
Copies each pixel to the output buffer.

- Memory Complexity: O(W × H)
Temporary buffer for saving to output format.

## Memory Management Operations
### Vector Memory Manager
- Allocation: O(W × H) — Resizes a 2D std::vector

- Pixel Access: O(1) — Direct indexing via data[y][x]

-  Clone: O(W × H) — Deep copy of the vector

- Space Complexity: O(W × H)
Additional overhead due to STL vector structure and non-contiguous rows (cache inefficiency possible)

## Buddy Memory Manager
- Allocation: O(log M) — Where M is total memory; uses binary splitting to allocate

- Pixel Access: O(1) — Uses pointer arithmetic

- Clone: O(W × H) — memcpy with allocation overhead

- Space Complexity: O(W × H) + internal fragmentation
Efficient for power-of-two allocations, but may waste space.

# Optimization Opportunities
## Memory Manager Selection:

Buddy system is more efficient for large allocations due to contiguous memory.

Vector-based is easier to implement but may suffer from cache misses and memory fragmentation.

## Scaling and Rotation Quality:

Nearest-neighbor is fast but low-quality.

Consider bilinear or bicubic interpolation for higher-quality image transformations (at a computational cost).

## Memory Reuse:

Avoid creating new memory managers per operation.

Reuse or pool memory to reduce allocation overhead.

## Parallelization:

Image operations are highly parallelizable.

Use multi-threading (e.g., std::thread, OpenMP) to speed up processing on large images.

## General complexity: 
- Time Complexity: O(pixels) for all operations (optimal for pixel-wise image processing)

- Memory Complexity: O(pixels) with trade-offs:

- Buddy: More efficient for large, long-lived, and contiguous allocations; may suffer from internal fragmentation.

- Vector: Simpler, suitable for smaller images; may lead to non-contiguous memory and higher overhead.
