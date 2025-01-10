#include <iostream>
#include <cstdlib>
#include <string>

int combineVideoAudio(const std::string& videoFile, const std::string& audioFile, const std::string& outputFile) {
    std::string command = "ffmpeg -y -i \"" + videoFile + "\" -i \"" + audioFile + "\" -c:v copy -c:a copy \"" + outputFile + "\"";
    return system(command.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: combine_video_audio <video_file> <audio_file> <output_file>" << std::endl;
        return -1;
    }

    std::string videoFile = argv[1];
    std::string audioFile = argv[2];
    std::string outputFile = argv[3];

    int ret = combineVideoAudio(videoFile, audioFile, outputFile);
    if (ret != 0) {
        std::cerr << "Error occurred during combining video and audio." << std::endl;
        return ret;
    }

    return 0;
}
