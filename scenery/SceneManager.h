//
// Created by lucas on 12/28/20.
//

#ifndef GAMEENGINE_SCENEMANAGER_H
#define GAMEENGINE_SCENEMANAGER_H

#include <vector>

#include <Horde3DUtils.h>

#include "scenes/Scene.h"
#include "scenes/BasicScene.h"
#include "../util/Logger.h"


class SceneManager {
public:
    SceneManager();
    ~SceneManager();
    bool init(GLFWwindow* windowHandler);
    bool registerScene(Scene* newScene);
    bool reloadResources();

    Scene* getCurrentScene();
private:
    std::vector<Scene*> scenes;
    unsigned int currentSceneIndex;
    std::string resourcePath;

    Logger* logger;

};


#endif //GAMEENGINE_SCENEMANAGER_H
