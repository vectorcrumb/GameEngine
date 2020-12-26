//
// Created by lucas on 12/24/20.
//

#include "Renderer.h"

Renderer::Renderer(const char *title,
                   int renderDevice,
                   float fov, float nearPlane, float farPlane,
                   int winWidth, int winHeight,
                   bool fullScreen, bool showCursor) :
                   renderDevice{renderDevice},
                   fov{fov},
                   nearPlane{nearPlane},
                   farPlane{farPlane},
                   fullScreen{fullScreen},
                   showCursor{showCursor},
                   winHandle{nullptr},
                   winWidth{winWidth},
                   winHeight{winHeight},
                   resourcePath("../Content/") {
    winTitle = title;
    logger = Logger::getInstance();
}

Renderer::~Renderer() {

}

bool Renderer::initWindow() {

    // Configure GLFW
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    if (renderDevice == H3DRenderDevice::OpenGL4) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    if (fullScreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        winHandle = glfwCreateWindow(mode->width, mode->height, winTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        winHandle = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(), NULL, NULL);
    }

    if (winHandle == NULL) {
        glfwDestroyWindow(winHandle);

        winHandle = glfwCreateWindow(800, 50, "Unable to initialize GLFW", NULL, NULL);
        double startTime = glfwGetTime();
        while (glfwGetTime() - startTime < 5.0);
    }



    return false;
}

bool Renderer::init() {


    if(!initWindow()) {

    }

    if (!initEngine()) {

    }

    if (!initResources())

    return false;
}

bool Renderer::initEngine() {
    return false;
}

bool Renderer::initResources() {
    return false;
}

void Renderer::update() {

}

void Renderer::terminate() {

}



