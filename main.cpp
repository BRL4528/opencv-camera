#include <opencv2/opencv.hpp>
#include <iostream>

// Utility function to convert string format to FOURCC code
int getFourCC(const std::string& format) {
    if (format == "YUYV") {
        return cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V');
    } else if (format == "MJPG") {
        return cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    } else {
        std::cerr << "Error: Unsupported format " << format << std::endl;
        return -1;
    }
}

void apply_camera_setting(const std::string& command, const std::string& error_message) {
    if (system(command.c_str()) != 0) {
        std::cerr << error_message << std::endl;
    }
}

int main() {
    // Open the webcam using V4L2 backend
    cv::VideoCapture cap(0, cv::CAP_V4L2);

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the webcam." << std::endl;
        return -1;
    }

    // Try setting the video format to YUYV
    int fourcc = getFourCC("MJPG");
    bool formatSet = cap.set(cv::CAP_PROP_FOURCC, fourcc);
    
    // If YUYV is not supported, try setting the format to MJPG
    if (!formatSet) {
        std::cout << "YUYV format not supported. Trying MJPG..." << std::endl;
        fourcc = getFourCC("MJPG");
        formatSet = cap.set(cv::CAP_PROP_FOURCC, fourcc);
    }

    if (!formatSet) {
        std::cerr << "Error: Neither YUYV nor MJPG formats are supported." << std::endl;
        return -1;
    }

    // Set camera parameters
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FPS, 30);

    // Apply camera settings using v4l2-ctl
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=brightness=128", "Failed to set brightness.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=contrast=128", "Failed to set contrast.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=saturation=128", "Failed to set saturation.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=white_balance_automatic=1", "Failed to set white balance automatic.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=gain=0", "Failed to set gain.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=power_line_frequency=2", "Failed to set power line frequency.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=sharpness=128", "Failed to set sharpness.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=backlight_compensation=1", "Failed to set backlight compensation.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=auto_exposure=3", "Failed to set auto exposure.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=exposure_dynamic_framerate=0", "Failed to set exposure dynamic framerate.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=pan_absolute=0", "Failed to set pan absolute.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=tilt_absolute=0", "Failed to set tilt absolute.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=focus_automatic_continuous=1", "Failed to set focus automatic continuous.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=zoom_absolute=100", "Failed to set zoom absolute.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=led1_mode=3", "Failed to set LED1 mode.");
    apply_camera_setting("v4l2-ctl -d /dev/video0 --set-ctrl=led1_frequency=0", "Failed to set LED1 frequency.");

    // Verify format, parameters, and FPS
    char format_str[5];
    int fourcc_code = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));
    format_str[0] = fourcc_code & 0xFF;
    format_str[1] = (fourcc_code >> 8) & 0xFF;
    format_str[2] = (fourcc_code >> 16) & 0xFF;
    format_str[3] = (fourcc_code >> 24) & 0xFF;
    format_str[4] = '\0';
    std::cout << "Video format: " << format_str << std::endl;

    std::cout << "Frame width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "Frame height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "Brightness: " << cap.get(cv::CAP_PROP_BRIGHTNESS) << std::endl;
    std::cout << "Contrast: " << cap.get(cv::CAP_PROP_CONTRAST) << std::endl;
    std::cout << "Saturation: " << cap.get(cv::CAP_PROP_SATURATION) << std::endl;
    std::cout << "Gain: " << cap.get(cv::CAP_PROP_GAIN) << std::endl;
    std::cout << "Exposure: " << cap.get(cv::CAP_PROP_EXPOSURE) << std::endl;
    std::cout << "Auto Exposure: " << cap.get(cv::CAP_PROP_AUTO_EXPOSURE) << std::endl;
    std::cout << "Auto White Balance: " << cap.get(cv::CAP_PROP_AUTO_WB) << std::endl;
    std::cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << std::endl;

    // Create a window to display the webcam feed
    cv::namedWindow("Webcam Feed", cv::WINDOW_AUTOSIZE);

    cv::VideoWriter videoWriter;
    bool recording = false;
    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = static_cast<int>(1000 / fps);  // delay in milliseconds for waitKey

    while (true) {
        auto start = std::chrono::steady_clock::now();  // Start timing for frame capture
        cv::Mat frame;
        cap >> frame;  // Capture frame

        if (frame.empty()) {
            std::cerr << "Error: Could not grab a frame." << std::endl;
            break;
        }

        cv::imshow("Webcam Feed", frame);  // Display the frame

        char key = static_cast<char>(cv::waitKey(1));  // 1 ms wait for keypress

        if (key == 'q') {
            break;  // Exit the loop if the user presses 'q'
        } else if (key == 's') {
            if (!recording) {
                // Start recording
                std::string filename = "output.mp4";
                int codec = cv::VideoWriter::fourcc('H', '2', '6', '4');
                cv::Size frameSize(static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)),
                                   static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

                videoWriter.open(filename, codec, fps, frameSize, true);

                if (!videoWriter.isOpened()) {
                    std::cerr << "Error: Could not open the video file for writing." << std::endl;
                } else {
                    std::cout << "Recording started..." << std::endl;
                    recording = true;
                }
            } else {
                // Stop recording
                videoWriter.release();
                std::cout << "Recording stopped and saved." << std::endl;
                recording = false;
            }
        }

        if (recording) {
            videoWriter.write(frame);  // Write the frame to the video file
        }

        auto end = std::chrono::steady_clock::now();  // End timing for frame capture
        std::chrono::duration<double, std::milli> elapsed = end - start;

        int sleepTime = delay - static_cast<int>(elapsed.count());
        if (sleepTime > 0) {
            cv::waitKey(sleepTime);  // Wait the remaining time to match the camera's FPS
        }
    }

    // Release the capture and video writer, and destroy the window
    cap.release();
    videoWriter.release();
   cv::destroyAllWindows();

    return 0;
}