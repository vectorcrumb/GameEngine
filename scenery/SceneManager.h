//
// Created by lucas on 12/28/20.
//

#ifndef GAMEENGINE_SCENEMANAGER_H
#define GAMEENGINE_SCENEMANAGER_H

#include <vector>
#include <memory>

#include <Horde3DUtils.h>

#include "scenes/Scene.h"
#include "scenes/BasicScene.h"
#include "../util/Logger.h"


class SceneManager {
public:
    SceneManager();
    ~SceneManager();
    bool init(GLFWwindow* windowHandler);
    bool registerScene(const std::shared_ptr<Scene>& newScene);
    bool reloadResources();

    std::shared_ptr<Scene> getCurrentScene();
private:
    std::vector<std::shared_ptr<Scene>> scenes;
    unsigned int currentSceneIndex;
    std::string resourcePath;

    Logger* logger;

};


#endif //GAMEENGINE_SCENEMANAGER_H
