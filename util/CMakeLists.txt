cmake_minimum_required(VERSION 3.1.0)
project(GameEngine VERSION 0.0.1)

add_library(Logger Logger.cpp)

find_package(spdlog REQUIRED)
target_link_libraries(Logger PUBLIC spdlog::spdlog)
