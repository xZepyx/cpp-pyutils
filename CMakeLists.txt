cmake_minimum_required(VERSION 3.10)
project(cpp-pyutils VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Installation
install(DIRECTORY include/ DESTINATION /usr/local/include)
install(FILES src/pyutils.cpp DESTINATION /usr/local/include)

add_executable(example examples/example.cpp)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)