cmake_minimum_required(VERSION 3.10)
project(cpp-pyutils VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only library target
add_library(pyutils INTERFACE)

# Public include directory
target_include_directories(pyutils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Example executable
add_executable(example examples/example.cpp)
target_link_libraries(example PRIVATE pyutils)
