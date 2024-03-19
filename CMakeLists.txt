cmake_minimum_required(VERSION 3.0)
project(OpenCV_Project)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your source file(s)
add_executable(OpenCV_Project "C:/Users/Guilherme/Documents/Projetos/Processamento de Imagens/src/main.cpp")

# Link OpenCV libraries
target_link_libraries(OpenCV_Project ${OpenCV_LIBS})
