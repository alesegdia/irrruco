cmake_minimum_required(VERSION 2.6)

project(irruco)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

list(APPEND CMAKE_CXX_FLAGS "-m64 -std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")


add_subdirectory(src)

