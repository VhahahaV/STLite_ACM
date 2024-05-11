cmake_minimum_required(VERSION 3.24)

SET(CMAKE_CXX_STANDARD 17)

PROJECT(map_one)
INCLUDE_DIRECTORIES(../../)
INCLUDE_DIRECTORIES(../)
ADD_EXECUTABLE(map_one code.cpp ../../map.hpp ../../exceptions.hpp ../../utility.hpp)
