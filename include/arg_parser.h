#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <iostream>

extern const char* VERSION;

using namespace std;

enum class Mode {
    BUDDY_SYSTEM,
    CONVENTIONAL
};

struct Arguments {
    string inputImageName;
    string outputImageName;
    float angle;
    float xScale;
    float yScale;
    Mode mode;
    bool valid = false;
};

Arguments parseArguments(int argc, char* argv[]);
void showHelp();
void showVersion();

inline std::ostream& operator<<(std::ostream& os, Mode mode) {
    switch (mode) {
        case Mode::BUDDY_SYSTEM:
            os << "BUDDY_SYSTEM";
            break;
        case Mode::CONVENTIONAL:
            os << "CONVENTIONAL";
            break;
    }
    return os;
}

#endif