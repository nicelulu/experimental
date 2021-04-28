cmake_minimum_required(VERSION 3.17)
project(experimental)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(FilterTest)

add_executable(main main.cpp)
