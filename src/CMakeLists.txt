# Name the sources
set(SOURCES
	CEventReceiver.cpp
	IQuadSceneNode.cpp
	main.cpp
	)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Irrlicht REQUIRED)
#find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(Aruco REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_executable(irrbull ${SOURCES})
add_executable(aruco_simple aruco_simple.cpp)

if(BOOST_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	target_link_libraries(irrbull ${Boost_LIBRARIES})
endif()

if(IRRLICHT_FOUND)
	include_directories(${IRRLICHT_INCLUDE_DIRS})
	target_link_libraries(irrbull ${IRRLICHT_LIBRARIES})
endif()

if(BULLET_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
	target_link_libraries(irrbull ${BULLET_LIBRARIES})
endif()

if(Aruco_FOUND)
	include_directories(aruco_simple ${Aruco_INCLUDE_DIR})
	target_link_libraries(aruco_simple ${Aruco_LIBRARY})
	include_directories(irrbull ${Aruco_INCLUDE_DIR})
	target_link_libraries(irrbull ${Aruco_LIBRARY})
endif()

if(OpenCV_FOUND)
	include_directories(aruco_simple ${OpenCV_INCLUDE_DIR})
	target_link_libraries(aruco_simple ${OpenCV_LIBRARIES})
	include_directories(irrbull ${OpenCV_INCLUDE_DIR})
	target_link_libraries(irrbull ${OpenCV_LIBRARIES})
endif()

# Set output
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
