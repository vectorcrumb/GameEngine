cmake_minimum_required(VERSION 3.1.0)
project(GameEngine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 REQUIRED)
#list(APPEND EXTRA_LIBS Eigen)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/include/*" "/usr/local/include/horde3d/")

add_executable(GameEngine main.cpp)

target_link_libraries(GameEngine PUBLIC Horde3D Horde3DUtils glfw ${EXTRA_LIBS})
target_include_directories(GameEngine PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${EXTRA_INCLUDES}"
        )

