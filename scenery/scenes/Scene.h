//
// Created by lucas on 12/28/20.
//

#ifndef GAMEENGINE_SCENE_H
#define GAMEENGINE_SCENE_H

#include <string>
#include <utility>
#include <Horde3D.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

class Scene {
public:
    explicit Scene(std::string name) {
        sceneName = std::move(name);
        cam = 0;
    }
    // Destructor is virtual to be able to destruct parent class
    virtual ~Scene() = default;
    virtual bool configure() = 0;
    virtual bool update(bool* keys, double dt) = 0;
    virtual bool configureCamera(GLFWwindow* windowHandler) = 0;
    std::string sceneName;

    H3DNode getCam() const {
        return cam;
    }


protected:
    H3DNode cam;
    const unsigned long debugInterval = 1000;
    unsigned long debugTimeFlag;
};


#endif //GAMEENGINE_SCENE_H
