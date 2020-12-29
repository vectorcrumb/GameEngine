add_library(Logger Logger.cpp)

find_package(spdlog REQUIRED)
target_link_libraries(Logger PUBLIC spdlog::spdlog)
