extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
}

#include <iostream>
#include <string>

int convertVideoFormat(const std::string& inputFile, const std::string& outputFile) {
    AVFormatContext *inFormatContext = nullptr, *outFormatContext = nullptr;
    AVPacket packet;
    int ret;

    // Open input file
    if ((ret = avformat_open_input(&inFormatContext, inputFile.c_str(), nullptr, nullptr)) < 0) {
        std::cerr << "Could not open input file." << std::endl;
        return ret;
    }

    // Retrieve input stream information
    if ((ret = avformat_find_stream_info(inFormatContext, nullptr)) < 0) {
        std::cerr << "Failed to retrieve input stream information" << std::endl;
        return ret;
    }

    // Allocate output format context
    avformat_alloc_output_context2(&outFormatContext, nullptr, nullptr, outputFile.c_str());
    if (!outFormatContext) {
        std::cerr << "Could not create output context" << std::endl;
        ret = AVERROR_UNKNOWN;
        return ret;
    }

    // Copy streams from input to output
    for (unsigned int i = 0; i < inFormatContext->nb_streams; i++) {
        AVStream *inStream = inFormatContext->streams[i];
        AVStream *outStream = avformat_new_stream(outFormatContext, nullptr);
        if (!outStream) {
            std::cerr << "Failed allocating output stream" << std::endl;
            ret = AVERROR_UNKNOWN;
            return ret;
        }

        // Copy the codec parameters
        ret = avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
        if (ret < 0) {
            std::cerr << "Failed to copy codec parameters" << std::endl;
            return ret;
        }
        outStream->codecpar->codec_tag = 0;
    }

    // Open the output file
    if (!(outFormatContext->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&outFormatContext->pb, outputFile.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            std::cerr << "Could not open output file." << std::endl;
            return ret;
        }
    }

    // Write the stream header
    ret = avformat_write_header(outFormatContext, nullptr);
    if (ret < 0) {
        std::cerr << "Error occurred when writing header" << std::endl;
        return ret;
    }

    // Read packets from input file and write them to the output file
    while (av_read_frame(inFormatContext, &packet) >= 0) {
        AVStream *inStream  = inFormatContext->streams[packet.stream_index];
        AVStream *outStream = outFormatContext->streams[packet.stream_index];

        // Copy packet
        packet.pts = av_rescale_q_rnd(packet.pts, inStream->time_base, outStream->time_base,
            (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        packet.dts = av_rescale_q_rnd(packet.dts, inStream->time_base, outStream->time_base,
            (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        packet.duration = av_rescale_q(packet.duration, inStream->time_base, outStream->time_base);
        packet.pos = -1;

        ret = av_interleaved_write_frame(outFormatContext, &packet);
        if (ret < 0) {
            std::cerr << "Error muxing packet" << std::endl;
            break;
        }
        av_packet_unref(&packet);
    }

    // Write the trailer
    av_write_trailer(outFormatContext);

    // Close input and output files
    avformat_close_input(&inFormatContext);

    if (outFormatContext && !(outFormatContext->oformat->flags & AVFMT_NOFILE))
        avio_closep(&outFormatContext->pb);
    avformat_free_context(outFormatContext);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: video_converter <input_file> <output_file>" << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    // Initialize FFmpeg library
    av_register_all();

    int ret = convertVideoFormat(inputFile, outputFile);
    if (ret < 0) {
        std::cerr << "Error occurred during conversion: " << av_err2str(ret) << std::endl;
        return ret;
    }

    return 0;
}
