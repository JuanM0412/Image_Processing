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
    float scale;
    Mode mode;
    bool valid = false;
};

Arguments parseArguments(int argc, char* argv[]);
void showHelp();
void showVersion();

#endif