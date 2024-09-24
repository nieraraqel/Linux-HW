cmake_minimum_required(VERSION 3.29)
project(ComplexNumber)

set(CMAKE_CXX_STANDARD 20)

add_executable(ComplexNumber main.cpp
        ComplexNumber.h
        ComplexNumber.cpp
        ComplexNumberSort.h)
