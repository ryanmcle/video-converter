import sys
import os
import subprocess
import platform
from PyQt5.QtWidgets import (
    QApplication, QWidget, QPushButton, QFileDialog,
    QLabel, QLineEdit, QVBoxLayout, QHBoxLayout, QMessageBox,
    QInputDialog
)
from PyQt5.QtCore import Qt, QThread, pyqtSignal

class ProcessThread(QThread):
    finished = pyqtSignal(bool, str)

    def __init__(self, command):
        QThread.__init__(self)
        self.command = command

    def run(self):
        try:
            process = subprocess.Popen(
                self.command, stdout=subprocess.PIPE, stderr=subprocess.PIPE
            )
            output, error = process.communicate()
            success = process.returncode == 0
            message = output.decode('utf-8') if success else error.decode('utf-8')
            self.finished.emit(success, message)
        except Exception as e:
            self.finished.emit(False, str(e))

class VideoEditorGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Video Processing Application')

        # Define paths to executables
        self.current_dir = os.path.dirname(os.path.abspath(__file__))
        self.cpp_backend_dir = os.path.normpath(os.path.join(self.current_dir, '..', 'cpp_backend'))

        if platform.system() == 'Windows':
            self.video_converter_executable = os.path.join(self.cpp_backend_dir, 'video_converter.exe')
            self.video_trimmer_executable = os.path.join(self.cpp_backend_dir, 'video_trimmer.exe')
            self.brightness_contrast_executable = os.path.join(self.cpp_backend_dir, 'brightness_contrast.exe')
            self.audio_extractor_executable = os.path.join(self.cpp_backend_dir, 'audio_extractor.exe')
            self.combine_video_audio_executable = os.path.join(self.cpp_backend_dir, 'combine_video_audio.exe')
        else:
            self.video_converter_executable = os.path.join(self.cpp_backend_dir, 'video_converter')
            self.video_trimmer_executable = os.path.join(self.cpp_backend_dir, 'video_trimmer')
            self.brightness_contrast_executable = os.path.join(self.cpp_backend_dir, 'brightness_contrast')
            self.audio_extractor_executable = os.path.join(self.cpp_backend_dir, 'audio_extractor')
            self.combine_video_audio_executable = os.path.join(self.cpp_backend_dir, 'combine_video_audio')

        self.initUI()

    def initUI(self):
        mainLayout = QVBoxLayout()

        # Input file selector
        self.inputLabel = QLabel('Input Video File:')
        self.inputLineEdit = QLineEdit()
        self.inputButton = QPushButton('Browse')
        self.inputButton.clicked.connect(self.browseInputFile)

        inputLayout = QHBoxLayout()
        inputLayout.addWidget(self.inputLineEdit)
        inputLayout.addWidget(self.inputButton)

        # Output file selector
        self.outputLabel = QLabel('Output Video File:')
        self.outputLineEdit = QLineEdit()
        self.outputButton = QPushButton('Browse')
        self.outputButton.clicked.connect(self.browseOutputFile)

        outputLayout = QHBoxLayout()
        outputLayout.addWidget(self.outputLineEdit)
        outputLayout.addWidget(self.outputButton)

        # Operation buttons
        self.convertButton = QPushButton('Convert Format')
        self.trimButton = QPushButton('Trim Video')
        self.filterButton = QPushButton('Adjust Brightness/Contrast')
        self.grayscaleButton = QPushButton('Apply Grayscale Filter')
        self.extractAudioButton = QPushButton('Extract Audio')  # New Button
        self.combineButton = QPushButton('Combine Video and Audio')  # New Button

        self.convertButton.clicked.connect(self.convertVideo)
        self.trimButton.clicked.connect(self.trimVideo)
        self.filterButton.clicked.connect(self.adjustBrightnessContrast)
        self.grayscaleButton.clicked.connect(self.applyGrayscale)
        self.extractAudioButton.clicked.connect(self.extractAudio)  # Connect to method
        self.combineButton.clicked.connect(self.combineVideoAudio)  # Connect to method

        # Add widgets to main layout
        mainLayout.addWidget(self.inputLabel)
        mainLayout.addLayout(inputLayout)
        mainLayout.addWidget(self.outputLabel)
        mainLayout.addLayout(outputLayout)
        mainLayout.addWidget(self.convertButton)
        mainLayout.addWidget(self.trimButton)
        mainLayout.addWidget(self.filterButton)
        mainLayout.addWidget(self.grayscaleButton)
        mainLayout.addWidget(self.extractAudioButton)
        mainLayout.addWidget(self.combineButton)

        self.setLayout(mainLayout)

    def browseInputFile(self):
        options = QFileDialog.Options()
        filename, _ = QFileDialog.getOpenFileName(
            self, 'Select Input Video File', '', 'Video Files (*.mp4 *.avi *.mkv)', options=options)
        if filename:
            self.inputLineEdit.setText(filename)

    def browseOutputFile(self):
        options = QFileDialog.Options()
        filename, _ = QFileDialog.getSaveFileName(
            self, 'Select Output Video File', '', 'Video Files (*.mp4 *.avi *.mkv)', options=options)
        if filename:
            self.outputLineEdit.setText(filename)

    def convertVideo(self):
        inputFile = self.inputLineEdit.text()
        outputFile = self.outputLineEdit.text()
        if not inputFile or not outputFile:
            QMessageBox.warning(self, 'Warning', 'Please select input and output files.')
            return

        command = [self.video_converter_executable, inputFile, outputFile]
        self.runProcess(command, 'Video Conversion')

    def trimVideo(self):
        inputFile = self.inputLineEdit.text()
        outputFile = self.outputLineEdit.text()
        if not inputFile or not outputFile:
            QMessageBox.warning(self, 'Warning', 'Please select input and output files.')
            return

        start_time, ok = QInputDialog.getDouble(self, 'Start Time', 'Enter start time in seconds:', min=0)
        if not ok:
            return
        end_time, ok = QInputDialog.getDouble(self, 'End Time', 'Enter end time in seconds:', min=start_time)
        if not ok:
            return

        command = [
            self.video_trimmer_executable, inputFile, outputFile, str(start_time), str(end_time)
        ]
        self.runProcess(command, 'Video Trimming')

    def adjustBrightnessContrast(self):
        inputFile = self.inputLineEdit.text()
        outputFile = self.outputLineEdit.text()
        if not inputFile or not outputFile:
            QMessageBox.warning(self, 'Warning', 'Please select input and output files.')
            return

        alpha, ok = QInputDialog.getDouble(self, 'Contrast', 'Enter contrast value (1.0 - 3.0):', min=1.0, max=3.0, decimals=2)
        if not ok:
            return
        beta, ok = QInputDialog.getInt(self, 'Brightness', 'Enter brightness value (0 - 100):', min=0, max=100)
        if not ok:
            return

        command = [
            self.brightness_contrast_executable, inputFile, outputFile, str(alpha), str(beta)
        ]
        self.runProcess(command, 'Adjusting Brightness and Contrast')

    def applyGrayscale(self):
        QMessageBox.information(self, 'Info', 'Grayscale filter functionality not implemented yet.')
        # Implement similar to adjustBrightnessContrast but applying grayscale filter.

    def extractAudio(self):
        inputFile = self.inputLineEdit.text()
        if not inputFile:
            QMessageBox.warning(self, 'Warning', 'Please select an input video file.')
            return

        options = QFileDialog.Options()
        outputFile, _ = QFileDialog.getSaveFileName(
            self, 'Select Output Audio File', '', 'Audio Files (*.aac *.mp3 *.wav)', options=options)
        if not outputFile:
            return

        command = [self.audio_extractor_executable, inputFile, outputFile]
        self.runProcess(command, 'Audio Extraction')

    def combineVideoAudio(self):
        options = QFileDialog.Options()
        videoFile, _ = QFileDialog.getOpenFileName(
            self, 'Select Video File (without audio)', '', 'Video Files (*.mp4 *.avi *.mkv)', options=options)
        if not videoFile:
            return

        audioFile, _ = QFileDialog.getOpenFileName(
            self, 'Select Audio File', '', 'Audio Files (*.aac *.mp3 *.wav)', options=options)
        if not audioFile:
            return

        outputFile, _ = QFileDialog.getSaveFileName(
            self, 'Select Output Video File', '', 'Video Files (*.mp4 *.avi *.mkv)', options=options)
        if not outputFile:
            return

        command = [self.combine_video_audio_executable, videoFile, audioFile, outputFile]
        self.runProcess(command, 'Combining Video and Audio')

    def runProcess(self, command, operation_name):
        self.thread = ProcessThread(command)
        self.thread.finished.connect(lambda success, message: self.onProcessFinished(success, message, operation_name))
        self.thread.start()

    def onProcessFinished(self, success, message, operation_name):
        if success:
            QMessageBox.information(self, 'Success', f'{operation_name} completed successfully.')
        else:
            QMessageBox.critical(self, 'Error', f'{operation_name} failed.\n{message}')

if __name__ == '__main__':
    app = QApplication(sys.argv)
    gui = VideoEditorGUI()
    gui.show()
    sys.exit(app.exec_())
