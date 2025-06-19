cmake_minimum_required(VERSION 3.16)
project(xhittp C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES src/*.c)
add_executable(xhittp ${SRC_FILES})
target_include_directories(xhittp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Unit Testing
add_subdirectory(tests/unity)

file(GLOB TEST_SRC_FILES tests/test_*.c)
file(GLOB SRC_FILES src/*.c)
list(FILTER SRC_FILES EXCLUDE REGEX ".*/main\\.c$")

add_executable(unit_tests ${TEST_SRC_FILES} ${SRC_FILES})
target_include_directories(unit_tests PRIVATE src)
target_link_libraries(unit_tests PRIVATE unity)
