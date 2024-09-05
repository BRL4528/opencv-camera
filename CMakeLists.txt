cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(WebcamCapture)

# Definir o padrão de compilação C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Encontrar o OpenCV
find_package(OpenCV REQUIRED)

# Incluir os diretórios do OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Adicionar o executável
add_executable(WebcamCapture main.cpp)

# Ligar o executável com as bibliotecas necessárias (OpenCV e biblioteca de sistema)
target_link_libraries(WebcamCapture ${OpenCV_LIBS})
