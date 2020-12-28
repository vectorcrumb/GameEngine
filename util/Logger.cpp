//
// Created by lucas on 12/24/20.
//

#include "Logger.h"


// Definition of instance is needed besides declaration. Because it's static,
// the definition must go inside the cpp file and not inline in the header.
Logger* Logger::instance = nullptr;


Logger::Logger() {
    // spdlog has error handling
    try {
        time_t rawtime;
        struct tm *timeinfo;
        // TODO: Possible stack overflow? Change strftime(char*) to string << std::put_time
        char log_name[50];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        // Attempt to format current time
        if (!strftime(log_name, 50, "../logs/%F_%T.txt", timeinfo)) {
            std::cout << "Couldn't get current time for log name." << std::endl;
            exit(60);
        }
        // Create the STDOUT log sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%Y-%m-%d %T] [%^%l%$] %v ");
        // Create the file log sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_name, true);
        file_sink->set_level(spdlog::level::debug);
        // Configure default spdlog logger using file and stdout sinks
        spdlog::sinks_init_list sink_list = {file_sink, console_sink};
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("GameEngine", spdlog::sinks_init_list(sink_list)));
        spdlog::set_level(spdlog::level::debug);
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        exit(50);
    }
    // Inform that spdlog logger is running
    spdlog::info("Initialized logger!");
}

// Singleton caller
Logger* Logger::getInstance() {
    // Spawn an instance
    // TODO: Not thread safe!
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}