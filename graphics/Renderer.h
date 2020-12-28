//
// Created by lucas on 12/24/20.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H


#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include "../util/Logger.h"


class Renderer {
public:
    Renderer(
            const char* title = "Game",
            int renderDevice = H3DRenderDevice::OpenGL4,
            float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 1000.0f,
            int winWidth = 1024, int winHeight = 768,
            bool fullScreen = false, bool showCursor = false
            );
    ~Renderer();
    bool init();
    void update();
    void terminate();
    void displayCursor(bool makeCursorVisible);
    void configureGLFWCallbacks(GLFWerrorfun errorCallback, GLFWkeyfun keyCallback);
    bool reloadResources();
    GLFWwindow* winHandle;
    // Spawn a default camera!
    H3DNode cam;


private:
    std::string winTitle, resourcePath;

    int renderDevice;
    float fov, nearPlane, farPlane;
    int winWidth, winHeight;
    bool fullScreen, showCursor;

    Logger* logger;



    bool initWindow();
    bool initEngine();
    bool initResources();

};


#endif //GAMEENGINE_RENDERER_H
