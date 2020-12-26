cmake_minimum_required(VERSION 3.1.0)
project(GameEngine VERSION 0.0.1)

add_library(Graphics Renderer.cpp)

find_package(glfw3 REQUIRED)
target_link_libraries(Graphics PUBLIC glfw)
target_include_directories(Graphics PUBLIC "/usr/local/include/horde3d/")