#include "../include/image.h"
#include "../include/buddy_system.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include <iostream>
#include <cmath>
#include <cstring> 
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

#define BUDDY_MEMORY_SIZE (1024 * 1024 * 500) // 500 MB
#define BUDDY_MIN_BLOCK_SIZE 64 // 64 bytes

BuddySystem* buddySystem = nullptr;

Image::Image() : width(0), height(0), finalWidth(0), finalHeight(0) {
    if (buddySystem == nullptr) {
        buddySystem = new BuddySystem(BUDDY_MEMORY_SIZE, BUDDY_MIN_BLOCK_SIZE);
    }
}

Image::~Image() {}

bool Image::loadImage(const std::string &inputImageName) {
    int nChannels;

    unsigned char *data = stbi_load(inputImageName.c_str(), &width, &height, &nChannels, 3);
    if (!data) {
        cerr << "Error: Could not load the image '" << inputImageName << "'.\n";
        return false;
    }

    pixelesChar.resize(height, vector<PixelChar>(width));
    pixelesInt.resize(height, vector<PixelInt>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            unsigned char r = data[index];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];

            pixelesChar[y][x] = {r, g, b};
            pixelesInt[y][x] = {r, g, b};
        }
    }

    stbi_image_free(data);
    
    finalWidth = width;
    finalHeight = height;

    cout << "Image loaded successfully from '" << inputImageName << "'\n";
    cout << "Original dimensions: " << width << "x" << height << " pixels\n";
    return true;
}

bool Image::SaveImage(const std::string &outputImageName) {
    vector<unsigned char> buffer(finalWidth * finalHeight * 3);

    for (int y = 0; y < finalHeight; ++y) {
        for (int x = 0; x < finalWidth; ++x) {
            int index = (y * finalWidth + x) * 3;
            buffer[index] = pixelesChar[y][x].r;
            buffer[index + 1] = pixelesChar[y][x].g;
            buffer[index + 2] = pixelesChar[y][x].b;
        }
    }

    if (stbi_write_png(outputImageName.c_str(), finalWidth, finalHeight, 3, buffer.data(), finalWidth * 3)) {
        cout << "Image saved successfully to '" << outputImageName << "'\n";
        cout << "Final dimensions: " << finalWidth << "x" << finalHeight << " pixels\n";
        return true;
    } else {
        cerr << "Error: Could not save the image to '" << outputImageName << "'.\n";
        return false;
    }
}

bool Image::rotateImage(float angle) {
    auto start = chrono::high_resolution_clock::now();
    
    float angleRad = angle * M_PI / 180.0f;
    
    float cosine = abs(cos(angleRad));
    float sine = abs(sin(angleRad));
    
    int newWidth = ceil(width * cosine + height * sine);
    int newHeight = ceil(width * sine + height * cosine);
    
    size_t requiredMemory = newHeight * newWidth * sizeof(PixelChar)+9000;
    
    size_t minBlockSize = buddySystem->getMinBlockSize();
    size_t paddedMemory = ((requiredMemory + minBlockSize - 1) / minBlockSize) * minBlockSize;
    
    cout << "Required memory for rotated image: " << requiredMemory << " bytes\n";
    cout << "Padded memory: " << paddedMemory << " bytes\n";
    
    if (paddedMemory > BUDDY_MEMORY_SIZE) {
        cerr << "Error: Not enough memory in the Buddy System for the rotated image.\n";
        return false;
    }
    
    vector<vector<PixelChar>> newPixelesChar(newHeight, vector<PixelChar>(newWidth));
    
    PixelChar* rotatedImageData = nullptr;
    
    if (buddySystem) {
        rotatedImageData = static_cast<PixelChar*>(buddySystem->allocate(paddedMemory));
        if (!rotatedImageData) {
            cerr << "Error: Could not allocate memory for rotated image.\n";
            return false;
        }
        
        memset(rotatedImageData, 0, paddedMemory); 
        
        float centerX = width / 2.0f;
        float centerY = height / 2.0f;
        
        float newCenterX = newWidth / 2.0f;
        float newCenterY = newHeight / 2.0f;
        
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                float xNew = x - newCenterX;
                float yNew = y - newCenterY;
                
                float xOrig = xNew * cos(angleRad) + yNew * sin(angleRad);
                float yOrig = -xNew * sin(angleRad) + yNew * cos(angleRad);
                
                xOrig += centerX;
                yOrig += centerY;
                
                if (xOrig >= 0 && xOrig < width && yOrig >= 0 && yOrig < height) {
                    int x0 = static_cast<int>(xOrig);
                    int y0 = static_cast<int>(yOrig);
                    int x1 = x0 + 1 < width ? x0 + 1 : x0;
                    int y1 = y0 + 1 < height ? y0 + 1 : y0;
                    
                    float dx = xOrig - x0;
                    float dy = yOrig - y0;
                    
                    int rotatedIndex = y * newWidth + x;
                    PixelChar& pixelDest = rotatedImageData[rotatedIndex];
                    
                    pixelDest.r = static_cast<unsigned char>(
                        (1-dx)*(1-dy) * pixelesChar[y0][x0].r +
                        dx*(1-dy) * pixelesChar[y0][x1].r +
                        (1-dx)*dy * pixelesChar[y1][x0].r +
                        dx*dy * pixelesChar[y1][x1].r
                    );
                    
                    pixelDest.g = static_cast<unsigned char>(
                        (1-dx)*(1-dy) * pixelesChar[y0][x0].g +
                        dx*(1-dy) * pixelesChar[y0][x1].g +
                        (1-dx)*dy * pixelesChar[y1][x0].g +
                        dx*dy * pixelesChar[y1][x1].g
                    );
                    
                    pixelDest.b = static_cast<unsigned char>(
                        (1-dx)*(1-dy) * pixelesChar[y0][x0].b +
                        dx*(1-dy) * pixelesChar[y0][x1].b +
                        (1-dx)*dy * pixelesChar[y1][x0].b +
                        dx*dy * pixelesChar[y1][x1].b
                    );
                }
            }
        }
        
        finalWidth = newWidth;
        finalHeight = newHeight;
        
        pixelesChar.resize(newHeight, vector<PixelChar>(newWidth));
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                int index = y * newWidth + x;
                pixelesChar[y][x] = rotatedImageData[index];
            }
        }
        
        buddySystem->deallocate(rotatedImageData);
        
        pixelesInt.resize(newHeight, vector<PixelInt>(newWidth));
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                pixelesInt[y][x].r = pixelesChar[y][x].r;
                pixelesInt[y][x].g = pixelesChar[y][x].g;
                pixelesInt[y][x].b = pixelesChar[y][x].b;
            }
        }
    } else {
        cerr << "Error: Buddy system is required but not initialized.\n";
        return false;
    }
    
    auto end = chrono::high_resolution_clock::now();
    processingTime = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << "Image rotated successfully by " << angle << " degrees\n";
    cout << "Processing time: " << processingTime.count() << " ms\n";
    
    return true;
}