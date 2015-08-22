cmake_minimum_required(VERSION 2.8)
project( playground )
find_package( OpenCV REQUIRED )
add_executable(playground main.cpp Gradient.cpp Helper.cpp Filter.cpp CVLib.cpp Corners.cpp HoughLines.cpp)
target_link_libraries(playground ${OpenCV_LIBS} )

# add_library(CVLib STATIC CannyEdge.hpp Filter.hpp Filter.cpp Gradient.cpp Gradient.hpp Helper.cpp Helper.hpp CVLib.cpp CVLib.h)