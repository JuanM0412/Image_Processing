#include "../include/arg_parser.h"
#include "../include/image.h"
#include "../include/buddy_image_memory_manager.h"
#include "../include/vector_image_memory_manager.h"
#include <chrono>
#include <sys/resource.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

void printMemoryUsage(const string& label) {
    ifstream statm("/proc/self/status");
    string line;
    while (getline(statm, line)) {
        if (line.find("VmRSS:") == 0) {
            istringstream iss(line);
            string key;
            size_t value;
            string unit;
            iss >> key >> value >> unit;
            cout << "[Memory] " << label << ": " << (value / 1024.0) << " MB\n";
            break;
        }
    }
}

void printPeakMemoryUsage(const string& label) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    cout << "[Peak Memory] " << label << ": " 
         << (usage.ru_maxrss / 1024.0) << " MB (Peak)" << endl;
}

void printElapsedTime(const string& label, high_resolution_clock::time_point start) {
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "[Time] " << label << ": " << duration << " ms\n";
}

int main(int argc, char *argv[]) {
    Arguments args = parseArguments(argc, argv);
    if (!args.valid) {
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
            printPeakMemoryUsage("Peak Memory Usage");
        } catch (const std::exception& e) {
            cerr << "Transformation failed: " << e.what() << endl;
        }
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
