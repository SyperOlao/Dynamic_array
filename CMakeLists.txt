cmake_minimum_required(VERSION 4.0)
project(DynamicArray)

set(CMAKE_CXX_STANDARD 20)

add_executable(DynamicArray main.cpp
        core/Array.tpp
        core/Array.h
        core/Consts.h)
