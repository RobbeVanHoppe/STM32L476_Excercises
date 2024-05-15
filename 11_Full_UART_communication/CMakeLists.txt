cmake_minimum_required(VERSION 3.28)
project(11_Full_UART_communication)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(PCProject)
add_subdirectory(STM32Project)

add_executable(11_Full_UART_communication main.cpp)
