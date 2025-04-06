#include "../include/arg_parser.h"
#include "../include/image.h"
#include "../include/buddy_image_memory_manager.h"
#include "../include/vector_image_memory_manager.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <fstream>
#include <sstream>
#endif

using namespace std;
using namespace std::chrono;

void printMemoryUsage(const std::string& label) {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo));
    SIZE_T memUsed = memInfo.WorkingSetSize;
    std::cout << "[Memory] " << label << ": " << memUsed / (1024.0 * 1024.0) << " MB\n";
#else
    std::ifstream statm("/proc/self/status");
    std::string line;
    while (std::getline(statm, line)) {
        if (line.find("VmRSS:") == 0) {
            std::istringstream iss(line);
            std::string key;
            size_t value;
            std::string unit;
            iss >> key >> value >> unit;
            std::cout << "[Memory] " << label << ": " << (value / 1024.0) << " MB\n";
            break;
        }
    }
#endif
}

void printElapsedTime(const std::string& label, high_resolution_clock::time_point start) {
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    std::cout << "[Time] " << label << ": " << duration << " ms\n";
}

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

        printMemoryUsage("Start");

        auto start = high_resolution_clock::now();
        Image image(memoryManager);
        if (!image.loadImage(args.inputImageName)) {
            cerr << "Error loading input image.\n";
            delete memoryManager;
            return 1;
        }
        printElapsedTime("Image loading", start);
        printMemoryUsage("After loading");

        try {
            start = high_resolution_clock::now();
            Image transformed_image = Image::scaleImage(args.xScale, args.yScale, image, args.mode);
            printElapsedTime("Scaling", start);
            printMemoryUsage("After scaling");

            start = high_resolution_clock::now();
            transformed_image = Image::rotateImage(args.angle, transformed_image, args.mode);
            printElapsedTime("Rotation", start);
            printMemoryUsage("After rotation");

            start = high_resolution_clock::now();
            transformed_image.saveImage(args.outputImageName);
            printElapsedTime("Saving", start);
            printMemoryUsage("After saving");
        } catch (const std::exception& e) {
            cerr << "Transformation failed: " << e.what() << endl;
        }

        delete memoryManager;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
