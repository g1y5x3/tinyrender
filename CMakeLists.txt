cmake_minimum_required(VERSION 3.10)

project(TinyRenderer CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_tgaimage src/test_tgaimage.cpp include/tgaimage.cpp)
add_executable(line_drawing  src/line_drawing.cpp  include/renderer.cpp include/tgaimage.cpp)
add_executable(render_diablo src/render_diablo.cpp include/renderer.cpp include/tgaimage.cpp include/objmodel.cpp)

target_include_directories(test_tgaimage PUBLIC include)
target_include_directories(line_drawing  PUBLIC include)
target_include_directories(render_diablo PUBLIC include)

# Optional: Add compiler warnings
target_compile_options(test_tgaimage PRIVATE -Wall -Wextra -pedantic -g -O3)
target_compile_options(line_drawing  PRIVATE -Wall -Wextra -pedantic -g -O3)
target_compile_options(render_diablo PRIVATE -Wall -Wextra -pedantic -g -O3)

# Add a custom target to remove all .tga files
add_custom_target(clean_tga
    COMMAND ${CMAKE_COMMAND} -E rm -f *.tga
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Removing all generated .tga files"
)