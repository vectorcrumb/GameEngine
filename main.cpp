#include <iostream>
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GLFW/glfw3.h>

#include "graphics/Renderer.h"
#include "scenery/SceneManager.h"


/**
 * If you get a weird Horde3DUtils linker error, delete all cmake-build-debug folders and reload cmake project in Clion.
 */


const int KEYS = 320;

Logger* mainLogger;

// Simple key state holder
bool gKeys[KEYS];
void setKeyState( int key, bool state ) {
    gKeys[key] = state;
}


// TODO: Send callbacks to I/O component
// GLFW callbacks
void error_callback(int error, const char* desc) {
    fprintf(stderr, "Error: %s\n", desc);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}


int main(){
    // Initialize logger first!
    mainLogger = Logger::getInstance();
    // Renderer object, interact with Horde3D and GLFW
    Renderer render = Renderer();
    render.init();
    render.configureGLFWCallbacks(error_callback, key_callback);
    h3dutDumpMessages();
    // Scene manager object
    SceneManager sceneManager = SceneManager();
    // Initialize to register basic scene
    sceneManager.init(render.winHandle);
    // TODO: Lazy load each resource from each scene?
    // Reload resources
//    sceneManager.reloadResources();



    // Time variables
    double prevTime = glfwGetTime(), currTime;
    double dt, lag = 0.0;

    // Main loop
    while (!glfwWindowShouldClose(render.winHandle)) {
        // Calculate current delta time
        currTime = glfwGetTime();
        dt = currTime - prevTime;
        prevTime = currTime;
        lag += dt;
        // Update key states
        for( int i = 32; i < KEYS; ++i ) {
            setKeyState( i, glfwGetKey(render.winHandle, i) == GLFW_PRESS );
        }
        // Run game loop logic
        sceneManager.getCurrentScene()->update(gKeys, dt);
        // Render next frame
        render.update(sceneManager.getCurrentScene()->cam);

    }
    // TODO: Cleanup scenemanager memory use
    // When exiting, cleanup
    render.terminate();

    return 0;
}