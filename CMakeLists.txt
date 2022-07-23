cmake_minimum_required(VERSION 3.16.3)
project(hello)

find_package(Eigen3 3 REQUIRED)
find_package(Vulkan MODULE)
message(STATUS "#    VULKAN:          ${VULKAN_FOUND}")
message(STATUS "#    Eigen3:          ${Eigen3_FOUND}")
add_executable(hello main.cpp)
add_executable(eigenvulkan eigenvulkan.cpp)
include_directories(eigenvulkan
        ${EIGEN3_INCLUDE_DIR}
        ${Vulkan_INCLUDE_DIR}
        openxr_includes)