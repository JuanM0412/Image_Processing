#include "../include/arg_parser.h"
#include "../include/image.h"
#include "../include/buddy_system.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern BuddySystem* buddySystem;

int main(int argc, char *argv[]) {
    Arguments args = parseArguments(argc, argv);

    if (!args.valid) {
        return 1;
    }

    if (args.mode == Mode::BUDDY_SYSTEM && buddySystem == nullptr) {
    const size_t BUDDY_MEMORY_SIZE = 1024 * 1024 * 200; // 200 MB
    const size_t BUDDY_MIN_BLOCK_SIZE = 64; // 64 bytes
    
    buddySystem = new BuddySystem(BUDDY_MEMORY_SIZE, BUDDY_MIN_BLOCK_SIZE);
    cout << "Buddy system initialized with " << (BUDDY_MEMORY_SIZE / 1024 / 1024) << " MB of memory\n";
    }

    Image image;
    
    if (!image.loadImage(args.inputImageName)) {
        return 1;
    }
    
    if (args.angle != 0) {
        if (!image.rotateImage(args.angle)) {
            cerr << "Error: Failed to rotate the image.\n";
            return 1;
        }
        
        cout << "\nProcessing Summary:\n";
        cout << "Original dimensions: " << image.getWidth() << "x" << image.getHeight() << " pixels\n";
        cout << "Final dimensions: " << image.getFinalWidth() << "x" << image.getFinalHeight() << " pixels\n";
        cout << "Rotation angle: " << args.angle << " degrees\n";
        cout << "Processing time: " << image.getProcessingTime() << " ms\n";
    }
    
    if (args.scale != 0) {
        cout << "Scaling functionality not implemented yet\n";
    }
    
    if (!image.SaveImage(args.outputImageName)) {
        return 1;
    }
    
    if (buddySystem != nullptr) {
        delete buddySystem;
        buddySystem = nullptr;
    }

    return 0;
}