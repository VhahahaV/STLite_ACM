cmake_minimum_required(VERSION 3.24)

SET(CMAKE_CXX_STANDARD 17)

PROJECT(vector_one)
INCLUDE_DIRECTORIES(../../)
ADD_EXECUTABLE(vector_one code.cpp ../../vector.hpp ../../exceptions.hpp ../../utility.hpp)
