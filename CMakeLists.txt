cmake_minimum_required(VERSION 3.20)
project(memfs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(memfs src/main.cpp)
target_compile_options(memfs PRIVATE -Wall -Wextra -Wpedantic)
