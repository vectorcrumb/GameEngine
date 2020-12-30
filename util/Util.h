//
// Created by lucas on 12/29/20.
//

#ifndef GAMEENGINE_UTIL_H
#define GAMEENGINE_UTIL_H

#include <chrono>


static unsigned long currentMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

static void timerStart(std::function<void(void)> func, unsigned int interval) {
    std::thread([func, interval]() {
        while (true) {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}


#endif //GAMEENGINE_UTIL_H
