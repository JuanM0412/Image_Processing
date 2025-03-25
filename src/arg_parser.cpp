#include "../include/arg_parser.h"
#include <cstring>

const char* VERSION = "1.0.0";

using namespace std;

Arguments parseArguments(int argc, char* argv[]) {
    Arguments args;
    args.valid = false;
    args.angle = 0;
    args.scale = 0;
    args.mode = Mode::CONVENTIONAL;

    if (argc < 2) {
        cerr << "Error: Missing arguments. Use -h for help." << endl;
        return args;
    }

    for (int i = 1; i < argc; ++i) {
        string option = argv[i];

        if (option == "-h" || option == "--help") {
            showHelp();
            return args;
        }
        if (option == "-v" || option == "--version") {
            showVersion();
            return args;
        }
        if (option == "-i" || option == "--input_image_name") {
            if (i + 1 < argc) {
                args.inputImageName = argv[++i];
            } else {
                cerr << "Error: Missing input image name." << endl;
                return args;
            }
        }
        else if (option == "-o" || option == "--output_image_name") {
            if (i + 1 < argc) {
                args.outputImageName = argv[++i];
            } else {
                cerr << "Error: Missing output image name." << endl;
                return args;
            }
        }
        else if (option == "-a" || option == "--angle") {
            if (i + 1 < argc) {
                try {
                    args.angle = stof(argv[++i]);
                } catch (const exception&) {
                    cerr << "Error: Invalid angle value." << endl;
                    return args;
                }
            } else {
                cerr << "Error: Missing angle value." << endl;
                return args;
            }
        }
        else if (option == "-s" || option == "--scale") {
            if (i + 1 < argc) {
                try {
                    args.scale = stof(argv[++i]);
                } catch (const exception&) {
                    cerr << "Error: Invalid scale value." << endl;
                    return args;
                }
            } else {
                cerr << "Error: Missing scale value." << endl;
                return args;
            }
        }
        else if (option == "-b" || option == "--buddy_system") {
            args.mode = Mode::BUDDY_SYSTEM;
        }
        else {
            cerr << "Error: Unknown option '" << option << "'." << endl;
            return args;
        }
    }

    if (args.inputImageName.empty() || args.outputImageName.empty()) {
        cerr << "Error: Input and output image name is required." << endl;
        return args;
    }

    args.valid = true;
    return args;
}

void showHelp() {
    cout << "Usage: ./image_processing -i <image_name> -o <resulting_image_name> -a <value> -s <value>\n"
              << "Options:\n"
              << "  -h, --help              Show this help message\n"
              << "  -v, --version               Show program version\n"
              << "  -i, --input_image_name <image_name>             Load specified image\n"
              << "  -o, --output_image_name <resulting_image_name>              Save resulting image with specified name\n"
              << "  -a, --angle <value>             Rotate image with specified angle\n"
              << "  -s, --scale <value>             Scale image with specified scale\n"
              << "  -b, --buddy_system              Activate buddy system mode (by default the program doesn't use buddy system)\n";
}

void showVersion() {
    cout << "Version: " << VERSION << endl;
}