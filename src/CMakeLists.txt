set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(aruco_simple aruco_simple.cpp)
target_link_libraries(aruco_simple  ${aruco_LIBS})

file( GLOB_RECURSE SRCs */*.cpp )
file( GLOB_RECURSE INCs */*.h )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY} )

