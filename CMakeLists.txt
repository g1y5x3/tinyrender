cmake_minimum_required(VERSION 3.10)

project(TinyRenderer CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp src/tgaimage.cpp)

target_include_directories(main PUBLIC include)

# Optional: Add compiler warnings
target_compile_options(main PRIVATE -Wall -Wextra -pedantic)