cmake_minimum_required(VERSION 3.28)
project(LinuxUART)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})