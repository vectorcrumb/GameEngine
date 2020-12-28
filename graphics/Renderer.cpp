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

/**
 * Initializes rendering component.
 *
 * The function initializes GLFW, Horde3D and relevant resources (in that order). Any failure will prompt the function
 * to log and return false.
 *
 * @return true if all 3 initializations are successful, false otherwise.
 */
bool Renderer::init() {

    if(!initWindow()) {
        return false;
    }
    spdlog::debug("Window correctly initialized");

    if (!initEngine()) {
        return false;
    }
    spdlog::debug("Engine correctly initialized");

    if (!initResources()) {
        return false;
    }
    spdlog::debug("Resources correctly loaded");

    spdlog::info("Rendering component initialization successful.");
    return true;
}


/**
 * Initialize a GL window using GLFW.
 *
 * The function configures a few GLFW window hints and then creates a bordered or fullscreen window.
 * If the process fails, a log is issued and the function returns false. If successful, the context
 * and pointer is configured and the function returns true.
 *
 * @return true if window is created, false otherwise.
 */
bool Renderer::initWindow() {

    // Initialize GLFW
    if (!glfwInit()) {
        spdlog::critical("Couldn't initialize GLFW!");
        return false;
    }

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

    // Create a fullscreen or bordered window
    if (fullScreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        winHandle = glfwCreateWindow(mode->width, mode->height, winTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        winHandle = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(), NULL, NULL);
    }

    // If the window couldn't be created, log and error and return
    if (winHandle == nullptr) {
        glfwDestroyWindow(winHandle);

        winHandle = glfwCreateWindow(800, 50, "Unable to initialize GLFW", NULL, NULL);
        double startTime = glfwGetTime();
        while (glfwGetTime() - startTime < 5.0);
        spdlog::critical("Couldn't initialize GLFW window. Check for a OpenGL version {} compatible"
                      " graphics card", renderDevice == H3DRenderDevice::OpenGL2 ? "2.0" : "4.3");
        return false;
    }

    // Set context and configure a pointer to window (is this to avoid losing context?)
    glfwSetWindowUserPointer(winHandle, this);
    glfwMakeContextCurrent(winHandle);
    // Configure showing cursor on screen or not
    displayCursor(showCursor);
    // Configure sticky keys
    glfwSetInputMode(winHandle, GLFW_STICKY_KEYS, GL_TRUE);
    // Enable V-sync. TODO: Make this a constructor option.
    // To disable, set interval = 0.
    glfwSwapInterval(1);

    return true;
}


bool Renderer::initEngine() {

    // Initialize Horde3D engine
    if (!h3dInit((H3DRenderDevice::List) renderDevice)) {
        spdlog::critical("Unable to initialize Horde3D engine!");
        h3dutDumpMessages();
        return false;
    }

    // Configure engine
    h3dSetOption(H3DOptions::LoadTextures, 1);
    h3dSetOption(H3DOptions::TexCompression, 0);
    h3dSetOption(H3DOptions::MaxAnisotropy, 4);
    h3dSetOption(H3DOptions::ShadowMapSize, 2048);
    h3dSetOption(H3DOptions::FastAnimation, 1);
    h3dSetOption(H3DOptions::SampleCount, 0);
    h3dSetOption(H3DOptions::DumpFailedShaders, 1);

    return true;
}

bool Renderer::initResources() {
    // TODO: Resource loading should be dynamic?
    // TODO: Find out what each resource type does
    // TODO: Pipelines?
    // TODO: Resource manager!!

    // Add resources
    H3DRes forwardPipeRes = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
    // Load resources
    if(!h3dutLoadResourcesFromDisk(resourcePath.c_str())) {
        spdlog::critical("Failed to load resources from path {}", resourcePath.c_str());
        return false;
    }
    // Add camera
    cam = h3dAddCameraNode(H3DRootNode, "Camera", forwardPipeRes);
    // TODO: When configuring camera viewport, the current window width and height are needed!
    // This logic should be abstracted into another function!!
    // Setup viewport for camera
    h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, winWidth);
    h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, winHeight);
    h3dSetupCameraView(cam, 45.0f, (float) winWidth / winHeight, 0.5f, 2048.0f);
    h3dResizePipelineBuffers(forwardPipeRes, winWidth, winHeight);
    // Reposition camera
//    h3dSetNodeTransform(cam, 0, 0, 3.0f, 0, 0, 0, 1, 1, 1);

    return true;
}


/**
 * Updates GLFW and renders currently configured scene
 *
 * This function takes the currently configured scene based on H3DNode objects and renders a new frame.
 * It must be called after the logic function inside the game loop. Running glfwPollEvents at the end
 * leaves the latest inputs ready for the game logic function.
 */
void Renderer::update() {

    if (!cam) {
        spdlog::error("Camera pointer went null. Rendering will fail.");
        return;
    }
    // Render current scene
    h3dRender(cam);
    h3dFinalizeFrame();
    h3dutDumpMessages();
    // Update GLFW view
    glfwSwapBuffers(winHandle);
    // Poll GLFW events. Avoids hanging window
    glfwPollEvents();
}

/**
 * Terminates GLFW and Horde3D and cleans up window handler
 */
void Renderer::terminate() {
    // Release memory consumed by Horde3D and associated objects
    h3dRelease();
    // Destroy GLFW window and terminate
    glfwDestroyWindow(winHandle);
    winHandle = nullptr;
    glfwTerminate();
}

/**
 * Configure window cursor
 * @param makeCursorVisible true to show cursor, false otherwise
 */
void Renderer::displayCursor(bool makeCursorVisible) {
    glfwSetInputMode(winHandle, GLFW_CURSOR, makeCursorVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    showCursor = makeCursorVisible;
}

void Renderer::configureGLFWCallbacks(GLFWerrorfun errorCallback, GLFWkeyfun keyCallback) {
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(winHandle, keyCallback);
}

bool Renderer::reloadResources() {
    if(!h3dutLoadResourcesFromDisk(resourcePath.c_str())) {
        spdlog::critical("Failed to load resources from path {}", resourcePath.c_str());
        return false;
    }
    return true;
}

