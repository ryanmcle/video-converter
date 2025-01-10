#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

int extractAudio(const std::string& inputFile, const std::string& audioFile) {
    // Call the audio_extractor executable
#ifdef _WIN32
    std::string executable = "audio_extractor.exe";
#else
    std::string executable = "./audio_extractor";
#endif
    std::string command = executable + " \"" + inputFile + "\" \"" + audioFile + "\"";
    return system(command.c_str());
}

int combineVideoAudio(const std::string& videoFile, const std::string& audioFile, const std::string& outputFile) {
    // Call the combine_video_audio executable
#ifdef _WIN32
    std::string executable = "combine_video_audio.exe";
#else
    std::string executable = "./combine_video_audio";
#endif
    std::string command = executable + " \"" + videoFile + "\" \"" + audioFile + "\" \"" + outputFile + "\"";
    return system(command.c_str());
}

int adjustBrightnessContrast(const std::string& inputFile, const std::string& outputFile, double alpha, int beta) {
    // Extract audio
    std::string audioFile = "temp_audio.aac";
    int ret = extractAudio(inputFile, audioFile);
    if (ret != 0) {
        std::cerr << "Error extracting audio." << std::endl;
        return ret;
    }

    // Open video file
    cv::VideoCapture cap(inputFile);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file." << std::endl;
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // Define the codec and create VideoWriter object
    cv::VideoWriter writer("temp_video.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(frameWidth, frameHeight));

    cv::Mat frame;
    while (cap.read(frame)) {
        cv::Mat adjustedFrame;
        frame.convertTo(adjustedFrame, -1, alpha, beta); // alpha: contrast, beta: brightness
        writer.write(adjustedFrame);
    }

    cap.release();
    writer.release();

    // Combine processed video and original audio
    ret = combineVideoAudio("temp_video.mp4", audioFile, outputFile);
    if (ret != 0) {
        std::cerr << "Error combining video and audio." << std::endl;
        return ret;
    }

    // Clean up temporary files
    remove("temp_video.mp4");
    remove(audioFile.c_str());

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Usage: brightness_contrast <input_file> <output_file> <alpha> <beta>" << std::endl;
        std::cout << "Alpha controls contrast (1.0-3.0), Beta controls brightness (0-100)" << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    double alpha = std::stod(argv[3]); // Contrast control
    int beta = std::stoi(argv[4]);     // Brightness control

    int ret = adjustBrightnessContrast(inputFile, outputFile, alpha, beta);
    if (ret != 0) {
        std::cerr << "Error adjusting brightness and contrast." << std::endl;
        return ret;
    }

    return 0;
}
