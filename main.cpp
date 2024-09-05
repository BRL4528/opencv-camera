#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Abre a câmera padrão (pode ser alterada para outro índice caso tenha múltiplas câmeras)
    cv::VideoCapture cap(0);  // Tente "cv::VideoCapture cap(1);" se houver mais de uma câmera

    if (!cap.isOpened()) {
        std::cerr << "Erro: Não foi possível abrir a câmera." << std::endl;
        return -1;
    }

    // Define parâmetros de gravação
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int fps = 30;

    // Abre o arquivo de saída
    cv::VideoWriter video("output_simple.mp4", cv::VideoWriter::fourcc('M', 'P', '4', 'V'), fps, cv::Size(frame_width, frame_height));

    if (!video.isOpened()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo de saída para gravação." << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Erro: Frame vazio, finalizando..." << std::endl;
            break;
        }

        // Exibe o vídeo ao vivo
        cv::imshow("Câmera", frame);

        // Grava o frame no arquivo de vídeo
        video.write(frame);

        // Pressione 'q' para sair
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Libera a câmera e fecha o arquivo de vídeo
    cap.release();
    video.release();
    cv::destroyAllWindows();

    return 0;
}
