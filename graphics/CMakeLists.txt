add_library(Graphics Renderer.cpp)

find_package(glfw3 REQUIRED)
target_include_directories(Graphics PUBLIC "/usr/local/include/horde3d/")
target_link_libraries(Graphics PUBLIC glfw Horde3D)
