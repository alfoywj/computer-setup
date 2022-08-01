add_library(offscreen ImageBuffer.cpp ImageBuffer.h OffScreen.cpp OffScreen.h VkBundle.cpp VkBundle.h ArcCamera.cpp)

include_directories(${EIGEN3_INCLUDE_DIR})

target_include_directories(offscreen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})