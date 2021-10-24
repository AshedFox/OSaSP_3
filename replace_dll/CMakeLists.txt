cmake_minimum_required(VERSION 3.20)
project(replace_dll)

set(CMAKE_CXX_STANDARD 14)

add_library(replace_dll SHARED replace_dll.cpp)