#add_library(Scene Scene.cpp)
add_library(SceneManager SceneManager.cpp)

#include_directories(./scenes/)

find_package(glfw3 REQUIRED)
target_include_directories(SceneManager PUBLIC "/usr/local/include/horde3d/")
target_link_libraries(SceneManager PUBLIC glfw Horde3D)
