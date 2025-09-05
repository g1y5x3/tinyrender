cmake_minimum_required(VERSION 3.10)

project(TinyRenderer CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_tgaimage    src/test_tgaimage.cpp    src/tgaimage.cpp)
add_executable(line_drawing     src/line_drawing.cpp     src/renderer.cpp src/tgaimage.cpp)
add_executable(wireframe_render src/wireframe_render.cpp src/renderer.cpp src/tgaimage.cpp src/objmodel.cpp)

target_include_directories(test_tgaimage    PUBLIC include)
target_include_directories(line_drawing     PUBLIC include)
target_include_directories(wireframe_render PUBLIC include)

# Optional: Add compiler warnings
target_compile_options(test_tgaimage     PRIVATE -Wall -Wextra -pedantic -g -O3)
target_compile_options(line_drawing      PRIVATE -Wall -Wextra -pedantic -g -O3)
target_compile_options(wireframe_render  PRIVATE -Wall -Wextra -pedantic -g -O3)

# Add a custom target to remove all .tga files
add_custom_target(clean_tga
    COMMAND ${CMAKE_COMMAND} -E rm -f *.tga
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Removing all generated .tga files"
)