@echo off

echo Compiling video_converter.cpp...
C:\msys64\mingw64\bin\g++.exe video_converter.cpp -o video_converter.exe -lavformat -lavcodec -lavutil -IC:\msys64\mingw64\include

echo Compiling video_trimmer.cpp...
C:\msys64\mingw64\bin\g++.exe video_trimmer.cpp -o video_trimmer.exe

echo Compiling audio_extractor.cpp...
C:\msys64\mingw64\bin\g++.exe audio_extractor.cpp -o audio_extractor.exe

echo Compiling combine_video_audio.cpp...
C:\msys64\mingw64\bin\g++.exe combine_video_audio.cpp -o combine_video_audio.exe

echo Compiling brightness_contrast.cpp...
C:\msys64\mingw64\bin\g++.exe brightness_contrast.cpp -o brightness_contrast.exe ^
    -I%OPENCV_INCLUDE% -L%OPENCV_LIB% -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -L"C:\msys64\mingw64\lib"

echo Build finished.
