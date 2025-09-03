cmake_minimum_required(VERSION 3.10)

project(TinyRenderer CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_tgaimage src/main.cpp src/tgaimage.cpp)

target_include_directories(test_tgaimage PUBLIC include)

# Optional: Add compiler warnings
target_compile_options(test_tgaimage PRIVATE -Wall -Wextra -pedantic)

# Add these files to the list of files to be deleted by "make clean"
file(GLOB TGA_FILES "build/*.tga")
set_directory_properties(PROPERTIES
    ADDITIONAL_MAKE_CLEAN_FILES "${TGA_FILES}"
)