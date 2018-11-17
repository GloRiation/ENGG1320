cmake_minimum_required(VERSION 2.8)
project( ENGG1320 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(include)

add_executable( ENGG1320 src/main.cpp src/imageProcessing.cpp)
target_link_libraries( ENGG1320 ${OpenCV_LIBS} )
