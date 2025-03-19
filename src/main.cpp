#include "../include/arg_parser.h"
#include "../include/image.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    Arguments args = parseArguments(argc, argv);

    if (!args.valid) {
        return 1;
    }

    cout << args.inputImageName << " - " << args.outputImageName << " - " << args.angle << " - " << args.scale << endl;

    return 0;
}