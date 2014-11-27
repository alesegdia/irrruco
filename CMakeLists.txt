
cmake_minimum_required(VERSION 2.8)
project(aruco_lab)
#SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
find_package(aruco REQUIRED )
add_subdirectory(src)
