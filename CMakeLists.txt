cmake_minimum_required(VERSION 3.23)
project(3DGraphicEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES "arm64")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        lve_window.cpp
        first_app.cpp
        lve_pipeline.cpp
        lve_device.cpp
        lve_swapchain.h
        lve_swapchain.cpp
)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)