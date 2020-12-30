add_library(InputObserver InputSystem.cpp)

find_package(glfw3 REQUIRED)
target_link_libraries(InputObserver PUBLIC glfw)
