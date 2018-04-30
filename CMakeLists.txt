cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(SuperPixelSegmentationEditor)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package( OpenCV 3 REQUIRED )

file(GLOB SRC . *.cpp)
file(GLOB HEADERS . *.h)
file(GLOB UIS . *.ui)
add_executable (SuperPixelSegmentationEditor ${SRC} ${HEADERS} ${UIS})
target_link_libraries(SuperPixelSegmentationEditor Qt5::Widgets ${OpenCV_LIBS})
