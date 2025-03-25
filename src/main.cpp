#include "../include/arg_parser.h"
#include "../include/image.h"
#include "../include/buddy_image_memory_manager.h"
#include "../include/vector_image_memory_manager.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    Arguments args = parseArguments(argc, argv);
    if (!args.valid) {
        cerr << "Invalid arguments provided.\n";
        return 1;
    }

    cout << "Processing Image: " << args.inputImageName 
         << " -> " << args.outputImageName 
         << " | Scale: (" << args.xScale << ", " << args.yScale << ")" 
         << " | Rotation: " << args.angle << " degrees\n"
         << " | Mode: " << args.mode << "\n";

    try {
        IImageMemoryManager* memoryManager = (args.mode == Mode::CONVENTIONAL) 
            ? static_cast<IImageMemoryManager*>(new VectorImageMemoryManager())
            : static_cast<IImageMemoryManager*>(new BuddyImageMemoryManager(1024 * 1024 * 1024));

        printf("Using %s\n", (args.mode == Mode::CONVENTIONAL) ? "vector" : "buddy");

        Image image(memoryManager);
        if (!image.loadImage(args.inputImageName)) {
            cerr << "Error loading input image.\n";
            delete memoryManager;
            return 1;
        }

        image.saveImage(args.outputImageName);

        try {
            Image scaled_image = Image::scaleImage(args.xScale, args.yScale, image, args.mode);
            scaled_image.saveImage("scaled_" + args.outputImageName);
        } catch (const std::exception& e) {
            cerr << "Scaling failed: " << e.what() << endl;
        }

        try {
            Image rotated_image = Image::rotateImage(args.angle, image, args.mode);
            rotated_image.saveImage("rotated_" + args.outputImageName);
        } catch (const std::exception& e) {
            cerr << "Rotation failed: " << e.what() << endl;
        }
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}