#include <iostream>
#include <cmath>
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GLFW/glfw3.h>

#include "graphics/Renderer.h"


/**
 * If you get a weird Horde3DUtils linker error, delete all cmake-build-debug folders and reload cmake project in Clion.
 */


const int KEYS = 320;

Logger* mainLogger;

H3DNode model = 0, cam = 0;

// TODO: This should be in a math util function
double deg2rad(double deg) {
    return deg * M_PI / 180.0f;
}

// Simple key state holder
bool gKeys[KEYS];
void setKeyState( int key, bool state ) {
    gKeys[key] = state;
}

double gCamLateralRotation, gCamVerticalRotation;

// GLFW callbacks
void error_callback(int error, const char* desc) {
    fprintf(stderr, "Error: %s\n", desc);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void gameLoop(double deltat) {
    static double t = 0;
    // Increase animation time
    t += 10.0 * deltat;
    // Play animation
    h3dSetModelAnimParams(model, 0, t, 1.0f);
    h3dUpdateModel(model, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
    // Set new model position
    h3dSetNodeTransform(model, t * 10, 0, 0,
                        0, 0, 0,
                        1, 1, 1);
    // Camera rotation and position
    if(gKeys[GLFW_KEY_LEFT]) gCamLateralRotation -= 50.0 * deltat;
    if(gKeys[GLFW_KEY_RIGHT]) gCamLateralRotation += 50.0 * deltat;
    if(gKeys[GLFW_KEY_UP]) gCamVerticalRotation -= 50.0 * deltat;
    if(gKeys[GLFW_KEY_DOWN]) gCamVerticalRotation += 50.0 * deltat;
    double zCamPos = std::cos(deg2rad(gCamLateralRotation)) * 3.0f;
    double xCamPos = std::sin(deg2rad(gCamLateralRotation)) * 3.0f;
    // Set new camera rotation and position
    h3dSetNodeTransform(cam,
                        xCamPos, 0, zCamPos,
                        gCamVerticalRotation, gCamLateralRotation, 0,
                        1, 1, 1);
}


int main(){
    // Initialize logger first!
    mainLogger = Logger::getInstance();
    // Renderer object, interact with Horde3D and GLFW
    Renderer render = Renderer();
    render.init();
    render.configureGLFWCallbacks(error_callback, key_callback);
    h3dutDumpMessages();
    // Grab references to cam
    cam = render.cam;

    // Load resources
    H3DRes modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/man/man.scene.xml", 0);
    H3DRes animRes = h3dAddResource(H3DResTypes::Animation, "animations/man.anim", 0);
    H3DRes skyBoxRes = h3dAddResource(H3DResTypes::SceneGraph, "models/skybox/skybox.scene.xml", 0);
    render.reloadResources();

    // TODO: Should spawning of nodes work directly with the Horde3D API or should there be an abstraction?
    // Add the model to scene with animation
    model = h3dAddNodes(H3DRootNode, modelRes);
    h3dSetupModelAnimStage(model, 0, animRes, 0, "", false);
    // Add skybox to scene
    H3DNode sky = h3dAddNodes(H3DRootNode, skyBoxRes);
    h3dSetNodeTransform(sky, 0, 0, 0, 0, 0, 0, 210, 50, 210);
    h3dSetNodeFlags(sky, H3DNodeFlags::NoCastShadow, true);
    // Add a light source, with position, radius and color
    H3DNode light = h3dAddLightNode(H3DRootNode, "Light1",  0, "LIGHTING","SHADOWMAP" );
    h3dSetNodeTransform(light, 0, 20, 0, 0, 0, 0, 1, 1, 1);
    h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 50.0f);
    h3dSetNodeParamF(light, H3DLight::ColorF3, 0.0f, 2.0f);

    // Time variables
    double prevTime = glfwGetTime();
    double currTime = glfwGetTime();
    double dt = 0.0, lag = 0.0;

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
        gameLoop(dt);
        // Render next frame
        render.update();

    }

    // When exiting, cleanup
    render.terminate();

    return 0;
}