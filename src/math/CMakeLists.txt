cmake_minimum_required(VERSION 3.16.3)
project(hello)

message(STATUS "#    VULKAN:          ${VULKAN_FOUND}")
message(STATUS "#    Eigen3:          ${Eigen3_FOUND}")
add_executable(hello main.cpp)
add_executable(eigenvulkan eigenvulkan.cpp)
add_executable(eigencalculus eigengame.cpp)
include_directories(eigencalculus
        ${EIGEN3_INCLUDE_DIR}
        ${Vulkan_INCLUDE_DIR}
        ../../openxr_includes)
include_directories(eigenvulkan
        ${EIGEN3_INCLUDE_DIR}
        ${Vulkan_INCLUDE_DIR}
        ../../openxr_includes)