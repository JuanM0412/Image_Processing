#include "../include/image.h"
#include <cmath>
#include <algorithm>

#define PI 3.14159265358979323846

using namespace std;

Image::Image(IImageMemoryManager* memManager) 
    : memoryManager(memManager) {}

Image::Image(const Image& other) {
    if (other.memoryManager) {
        memoryManager = other.memoryManager->clone();
    }
    
    width = other.width;
    height = other.height;
}

Image::Image(Image&& other) noexcept 
    : width(other.width), 
      height(other.height), 
      memoryManager(other.memoryManager) {
    other.width = 0;
    other.height = 0;
    other.memoryManager = nullptr;
}

Image& Image::operator=(const Image& other) {
    if (this != &other) {
        delete memoryManager;
        
        if (other.memoryManager) {
            memoryManager = other.memoryManager->clone();
        } else {
            memoryManager = nullptr;
        }
        
        width = other.width;
        height = other.height;
    }
    return *this;
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        delete memoryManager;
        
        width = other.width;
        height = other.height;
        memoryManager = other.memoryManager;
        
        other.width = 0;
        other.height = 0;
        other.memoryManager = nullptr;
    }
    return *this;
}

Image::~Image() {
    delete memoryManager;
}

bool Image::loadImage(const string &inputImageName) {
    int nChannels;
    unsigned char *data = stbi_load(inputImageName.c_str(), &width, &height, &nChannels, 3);
    if (!data) {
        cerr << "Error: Could not load the image '" << inputImageName << "'.\n";
        return false;
    }

    memoryManager->allocate(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            PixelChar pixel = { data[index], data[index + 1], data[index + 2] };
            memoryManager->setPixel(x, y, pixel);
        }
    }

    stbi_image_free(data);
    cout << "Image loaded successfully from '" << inputImageName << "'\n";
    return true;
}

bool Image::saveImage(const string &outputImageName) {
    if (width <= 0 || height <= 0) {
        cerr << "Error: Cannot save image with invalid dimensions.\n";
        return false;
    }

    vector<unsigned char> buffer(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            PixelChar pixel = memoryManager->getPixel(x, y);
            buffer[index] = pixel.r;
            buffer[index + 1] = pixel.g;
            buffer[index + 2] = pixel.b;
        }
    }

    int result = -1;
    string fileExtension = outputImageName.substr(outputImageName.find_last_of(".") + 1);

    if (fileExtension == "png") {
        result = stbi_write_png(outputImageName.c_str(), width, height, 3, buffer.data(), width * 3);
    } else if (fileExtension == "jpg" || fileExtension == "jpeg") {
        result = stbi_write_jpg(outputImageName.c_str(), width, height, 3, buffer.data(), 90);
    } else if (fileExtension == "bmp") {
        result = stbi_write_bmp(outputImageName.c_str(), width, height, 3, buffer.data());
    } else {
        result = stbi_write_png(outputImageName.c_str(), width, height, 3, buffer.data(), width * 3);
    }

    if (result) {
        cout << "Image saved successfully to '" << outputImageName << "'\n";
        return true;
    } else {
        cerr << "Error: Could not save the image to '" << outputImageName << "'.\n";
        
        FILE* testFile = fopen(outputImageName.c_str(), "wb");
        if (testFile == nullptr) {
            cerr << "Cannot open file for writing. Check permissions and directory.\n";
            perror("File open error");
        } else {
            fclose(testFile);
        }

        return false;
    }
}

Image Image::scaleImage(float xScale, float yScale, Image &originalImage, Mode mode) {
    cout << "Original image size: " << originalImage.getWidth() << "x" << originalImage.getHeight() << endl;

    int newWidth = static_cast<int>(originalImage.getWidth() * xScale);
    int newHeight = static_cast<int>(originalImage.getHeight() * yScale);

    cout << "New image size: " << newWidth << "x" << newHeight << endl;

    if (newWidth <= 0 || newHeight <= 0) {
        throw runtime_error("Invalid scaled image size");
    }

    IImageMemoryManager* scaledMemoryManager = (mode == Mode::CONVENTIONAL) 
            ? static_cast<IImageMemoryManager*>(new VectorImageMemoryManager())
            : static_cast<IImageMemoryManager*>(new BuddyImageMemoryManager(1024 * 1024 * 1024));
    Image scaledImage(scaledMemoryManager);
    
    try {
        scaledMemoryManager->allocate(newWidth, newHeight);
        
        scaledImage.width = newWidth;
        scaledImage.height = newHeight;
    } catch (const exception& e) {
        throw runtime_error("Memory allocation failed for scaled image: " + string(e.what()));
    }

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int origX = static_cast<int>(x / xScale);
            int origY = static_cast<int>(y / yScale);

            origX = min(max(origX, 0), originalImage.getWidth() - 1);
            origY = min(max(origY, 0), originalImage.getHeight() - 1);

            PixelChar pixel = originalImage.memoryManager->getPixel(origX, origY);
            
            scaledImage.memoryManager->setPixel(x, y, pixel);
        }
    }

    return scaledImage;
}

Image Image::rotateImage(float angle, Image &originalImage, Mode mode) {
    cout << "Rotating image by " << angle << " degrees\n";

    float radians = angle * (PI / 180.0);

    int origWidth = originalImage.getWidth();
    int origHeight = originalImage.getHeight();

    int newWidth = abs(origWidth * cos(radians)) + abs(origHeight * sin(radians));
    int newHeight = abs(origWidth * sin(radians)) + abs(origHeight * cos(radians));

    cout << "New image size after rotation: " << newWidth << "x" << newHeight << endl;

    if (newWidth <= 0 || newHeight <= 0) {
        throw runtime_error("Invalid rotated image size");
    }

    IImageMemoryManager* rotatedMemoryManager = (mode == Mode::CONVENTIONAL) 
        ? static_cast<IImageMemoryManager*>(new VectorImageMemoryManager())
        : static_cast<IImageMemoryManager*>(new BuddyImageMemoryManager(1024 * 1024 * 1024));

    Image rotatedImage(rotatedMemoryManager);
    rotatedMemoryManager->allocate(newWidth, newHeight);
    rotatedImage.width = newWidth;
    rotatedImage.height = newHeight;

    float centerX = origWidth / 2.0f;
    float centerY = origHeight / 2.0f;
    float newCenterX = newWidth / 2.0f;
    float newCenterY = newHeight / 2.0f;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            float relX = x - newCenterX;
            float relY = y - newCenterY;

            int origX = static_cast<int>(cos(-radians) * relX - sin(-radians) * relY + centerX);
            int origY = static_cast<int>(sin(-radians) * relX + cos(-radians) * relY + centerY);

            // Check if original coordinates are within bounds
            if (origX >= 0 && origX < origWidth && origY >= 0 && origY < origHeight) {
                PixelChar pixel = originalImage.memoryManager->getPixel(origX, origY);
                rotatedImage.memoryManager->setPixel(x, y, pixel);
            } else {
                // Set out-of-bounds pixels to black
                rotatedImage.memoryManager->setPixel(x, y, {0, 0, 0});
            }
        }
    }

    return rotatedImage;
}