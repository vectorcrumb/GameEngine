//
// Created by lucas on 12/24/20.
//

#ifndef GAMEENGINE_LOGGER_H
#define GAMEENGINE_LOGGER_H

#include <iostream>
#include <chrono>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger {
public:
    static Logger* getInstance();
    ~Logger();
    Logger(Logger& other) = delete;
    void operator=(const Logger &) = delete;
protected:
    Logger();
    static Logger* instance;
};


#endif //GAMEENGINE_LOGGER_H
