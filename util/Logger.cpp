//
// Created by lucas on 12/24/20.
//

#include "Logger.h"

Logger::Logger() {

    try {
        time_t rawtime;
        struct tm *timeinfo;
        char log_name[50];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if (!strftime(log_name, 50, "../logs/%F_%T.txt", timeinfo)) {
            std::cout << "Couldn't get current time for log name." << std::endl;
            exit(60);
        }
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%Y-%m-%d %T] [%^%l%$] %v ");
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_name, true);
        file_sink->set_level(spdlog::level::debug);
        spdlog::sinks_init_list sink_list = {file_sink, console_sink};
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("GameEngine", spdlog::sinks_init_list(sink_list)));
        spdlog::set_level(spdlog::level::debug);
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        exit(50);
    }

    spdlog::info("Initialized logger!");

}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}