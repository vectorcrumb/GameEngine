//
// Created by lucas on 12/28/20.
//

#include "SceneManager.h"

bool SceneManager::init(GLFWwindow* windowHandler) {
    // Register each scene one by one here
    registerScene(new BasicScene());
    // TODO: This is a bit hackish, but not loading resources in between declaring
    //  them and configuring nodes throws an ugly error. However, this might be a
    //  way of implementing lazy loading?
    // Load resources once all scenes have asked for them
    reloadResources();
    // Configure each scene and their cameras
    for (Scene* scene : scenes) {
        scene->configure();
        scene->configureCamera(windowHandler);
    }

    return true;
}

bool SceneManager::registerScene(Scene* newScene) {
    scenes.push_back(newScene);
    return true;
}

SceneManager::SceneManager()
    : currentSceneIndex{0},
      resourcePath{"../Content/"}{
    logger = Logger::getInstance();
}

SceneManager::~SceneManager() {
    scenes.clear();
}

Scene *SceneManager::getCurrentScene() {
    return scenes.at(currentSceneIndex);
}

bool SceneManager::reloadResources() {
    if(!h3dutLoadResourcesFromDisk(resourcePath.c_str())) {
        spdlog::critical("Failed to load resources from path {}", resourcePath.c_str());
        return false;
    }
    return true;
}