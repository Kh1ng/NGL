cmake_minimum_required(VERSION 3.0)
project(ngl)

set(CMAKE_CXX_STANDARD 23)

add_executable(ngl cmake-build-debug/src/main.cpp)
