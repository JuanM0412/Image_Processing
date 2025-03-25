#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <chrono>

struct PixelChar {
    unsigned char r, g, b;
};

struct PixelInt {
    int r, g, b;
};

class Image {
private:
    int width, height;
    std::vector<std::vector<PixelChar>> pixelesChar;
    std::vector<std::vector<PixelInt>> pixelesInt;
    
    int finalWidth, finalHeight;
    
    std::chrono::duration<double, std::milli> processingTime;

public:
    Image();
    ~Image();

    bool loadImage(const std::string &inputImageName);
    bool SaveImage(const std::string &outputImageName);
    
    bool rotateImage(float angle);
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getFinalWidth() const { return finalWidth; }
    int getFinalHeight() const { return finalHeight; }
    double getProcessingTime() const { return processingTime.count(); }
};

#endif 