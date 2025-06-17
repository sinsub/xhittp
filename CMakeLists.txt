cmake_minimum_required(VERSION 3.16)
project(xhittp C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES src/*.c)
add_executable(xhittp ${SRC_FILES})
target_include_directories(xhittp PRIVATE ${PROJECT_SOURCE_DIR}/src)