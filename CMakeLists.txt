cmake_minimum_required(VERSION 2.6)
project(cornerdetection)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(cornerdetection main.cpp HarrisCorners.cpp)
target_link_libraries(cornerdetection libLHTlib.a ${OpenCV_LIBS} )


install(TARGETS cornerdetection RUNTIME DESTINATION bin)
