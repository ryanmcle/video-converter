#include <iostream>
#include <cstdlib>
#include <string>

int extractAudio(const std::string& inputFile, const std::string& outputFile) {
    std::string command = "ffmpeg -y -i \"" + inputFile + "\" -vn -acodec copy \"" + outputFile + "\"";
    return system(command.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: audio_extractor <input_video_file> <output_audio_file>" << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int ret = extractAudio(inputFile, outputFile);
    if (ret != 0) {
        std::cerr << "Error occurred during audio extraction." << std::endl;
        return ret;
    }

    return 0;
}
