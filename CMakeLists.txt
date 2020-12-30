cmake_minimum_required(VERSION 3.1.0)
project(GameEngine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(graphics)
add_subdirectory(util)
add_subdirectory(scenery)
add_subdirectory(input)

find_package(glfw3 REQUIRED)
find_package(Eigen3 REQUIRED)
list(APPEND EXTRA_LIBS Graphics Logger SceneManager InputObserver)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/include/*" "/usr/local/include/horde3d/")
include_directories(./scenery/scenes/)

add_executable(GameEngine main.cpp)

target_link_libraries(GameEngine PUBLIC Horde3D Horde3DUtils glfw ${EXTRA_LIBS})
target_include_directories(GameEngine PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${EXTRA_INCLUDES}"
        )

