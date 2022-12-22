cmake_minimum_required(VERSION 3.23)
project(3DGraphicEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)

add_executable(${PROJECT_NAME} main.cpp lve_window.h lve_window.cpp first_app.h first_app.cpp lve_pipeline.h lve_pipeline.cpp lve_device.h lve_device.cpp)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
