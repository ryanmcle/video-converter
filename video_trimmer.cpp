#include <iostream>
#include <cstdlib>
#include <string>

int trimVideo(const std::string& inputFile, const std::string& outputFile, double startSec, double endSec) {
    std::string command = "ffmpeg -y -i \"" + inputFile + "\" -ss " + std::to_string(startSec) +
                          " -to " + std::to_string(endSec) + " -c copy \"" + outputFile + "\"";
    return system(command.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Usage: video_trimmer <input_file> <output_file> <start_time> <end_time>" << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    double startSec = std::stod(argv[3]);
    double endSec = std::stod(argv[4]);

    int ret = trimVideo(inputFile, outputFile, startSec, endSec);
    if (ret != 0) {
        std::cerr << "Error occurred during trimming." << std::endl;
        return ret;
    }

    return 0;
}
