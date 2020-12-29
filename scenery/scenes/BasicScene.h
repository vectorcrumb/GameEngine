//
// Created by lucas on 12/28/20.
//

#ifndef GAMEENGINE_BASICSCENE_H
#define GAMEENGINE_BASICSCENE_H

#include "Scene.h"
#include <cmath>


class BasicScene : public Scene {
public:
    BasicScene() : Scene("Scene_1") {

        pipeline = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
        cam = h3dAddCameraNode(H3DRootNode, "Camera", pipeline);

        skyBox = h3dAddResource(H3DResTypes::SceneGraph, "models/skybox/skybox.scene.xml", 0);
        modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/man/man.scene.xml", 0);
        modelAnimationRes = h3dAddResource(H3DResTypes::Animation, "animations/man.anim", 0);

    }

    bool configure() override {
        model = h3dAddNodes(H3DRootNode, modelRes);
        h3dSetupModelAnimStage(model, 0, modelAnimationRes, 0, "", false);

        sky = h3dAddNodes(H3DRootNode, skyBox);
        h3dSetNodeTransform(sky, 0, 0, 0, 0, 0, 0, 210, 50, 210);
        h3dSetNodeFlags(sky, H3DNodeFlags::NoCastShadow, true);

        light = h3dAddLightNode(H3DRootNode, "Light1",  0, "LIGHTING","SHADOWMAP" );
        h3dSetNodeTransform(light, 0, 20, 0, 0, 0, 0, 1, 1, 1);
        h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 50.0f);
        h3dSetNodeParamF(light, H3DLight::ColorF3, 0.0f, 2.0f);
    }

    bool configureCamera(GLFWwindow* windowHandler) override {
        int width, height;
        if (windowHandler) {
            glfwGetWindowSize(windowHandler, &width, &height);
        } else {
            return false;
        }

        // Setup viewport for camera
        h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
        h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
        h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, width);
        h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, height);
        h3dSetupCameraView(cam, 45.0f, (float) width / height, 0.5f, 2048.0f);
        h3dResizePipelineBuffers(pipeline, width, height);
        // Reposition camera
        h3dSetNodeTransform(cam, 0, 0, 3.0f, 0, 0, 0, 1, 1, 1);

        return true;
    }

    // TODO: This should be in a math util function
    double deg2rad(double deg) {
        return deg * M_PI / 180.0f;
    }

    bool update(bool* keys, double dt) override {
        t += 10.0 * dt;
        h3dSetModelAnimParams(model, 0, t, 1.0f);
        h3dUpdateModel(model, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
        h3dSetNodeTransform(model, t * 10, 0, 0,
                            0, 0, 0,
                            1, 1, 1);
        if (keys[GLFW_KEY_LEFT]) gCamLateralRotation -= 50.0 * dt;
        if (keys[GLFW_KEY_RIGHT]) gCamLateralRotation += 50.0 * dt;
        if (keys[GLFW_KEY_UP]) gCamVerticalRotation -= 50.0 * dt;
        if (keys[GLFW_KEY_DOWN]) gCamVerticalRotation += 50.0 * dt;

        double zCamPos = std::cos(deg2rad(gCamLateralRotation)) * 3.0f;
        double xCamPos = std::sin(deg2rad(gCamLateralRotation)) * 3.0f;
        // Set new camera rotation and position
        h3dSetNodeTransform(cam,
                            xCamPos, 0, zCamPos,
                            gCamVerticalRotation, gCamLateralRotation, 0,
                            1, 1, 1);
    }

private:
    H3DRes skyBox;
    H3DRes modelRes, modelAnimationRes;
    H3DRes pipeline;
    H3DNode sky;
    H3DNode model;
    H3DNode light;
    double t = 0.0;
    double gCamLateralRotation = 0.0, gCamVerticalRotation = 0.0;
};



#endif //GAMEENGINE_BASICSCENE_H
