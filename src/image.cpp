#include "../include/image.h"
#include <iostream>

using namespace std;

Image::Image() : width(0), height(0) {}

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

    cout << "Image loaded successfully from '" << inputImageName << "'\n";
    return true;
}

bool Image::SaveImage(const std::string &outputImageName) {
    vector<unsigned char> buffer(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            buffer[index] = pixelesChar[y][x].r;
            buffer[index + 1] = pixelesChar[y][x].g;
            buffer[index + 2] = pixelesChar[y][x].b;
        }
    }

    if (stbi_write_png(outputImageName.c_str(), width, height, 3, buffer.data(), width * 3)) {
        cout << "Image saved successfully to '" << outputImageName << "'\n";
        return true;
    } else {
        cerr << "Error: Could not save the image to '" << outputImageName << "'.\n";
        return false;
    }
}

Image Image::scaleImage(float xScale, float yScale, Image &originalImage) {
    cout << "Original image size: " << originalImage.width << "x" << originalImage.height << endl;
    float newWidth = originalImage.width * xScale;
    float newHeight = originalImage.height * yScale;
    cout << "New image size: " << newWidth << "x" << newHeight << endl;

    float xScaleFactor = (float)originalImage.width / newWidth;
    float yScaleFactor = (float)originalImage.height / newHeight;

    Image scaledImage;
    scaledImage.width = newWidth;
    scaledImage.height = newHeight;
    scaledImage.pixelesChar.resize(newHeight, vector<PixelChar>(newWidth));

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int newX = (int)x * xScaleFactor;
            int newY = (int)y * yScaleFactor;

            scaledImage.pixelesChar[y][x] = originalImage.pixelesChar[newY][newX];
        }
    }

    return scaledImage;
}