#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // GStreamer pipeline para o Jetson (adaptar se necessário)
    std::string pipeline = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1080, framerate=30/1 ! nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    // Inicializa a captura com a pipeline GStreamer
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        std::cerr << "Erro: Não foi possível abrir a câmera com GStreamer." << std::endl;
        return -1;
    }

    // Define parâmetros de gravação
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int fps = 30;

    // Abre arquivo de saída
    cv::VideoWriter video("output.mp4", cv::VideoWriter::fourcc('M', 'P', '4', 'V'), fps, cv::Size(frame_width, frame_height));

    if (!video.isOpened()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo de saída para gravação." << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Exibe o vídeo ao vivo
        cv::imshow("Câmera Jetson", frame);

        // Grava o frame no arquivo de vídeo
        video.write(frame);

        // Pressione 'q' para sair
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Libera a câmera e fecha os arquivos
    cap.release();
    video.release();
    cv::destroyAllWindows();

    return 0;
}
