//
// Created by lucas on 12/28/20.
//

#ifndef GAMEENGINE_BASICSCENE_H
#define GAMEENGINE_BASICSCENE_H

#include "Scene.h"
#include <cmath>
#include <iostream>
#include <thread>
#include <functional>
#include <Horde3DOverlays.h>

#include "../../util/Util.h"
#include "../../util/MathUtil.h"

class BasicScene : public Scene {
public:
    BasicScene() : Scene("Scene_1") {

        pipeline = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
        cam = h3dAddCameraNode(H3DRootNode, "Camera", pipeline);

        skyBoxRes = h3dAddResource(H3DResTypes::SceneGraph, "models/skybox/skybox.scene.xml", 0);
        modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/man/man.scene.xml", 0);
        modelAnimationRes = h3dAddResource(H3DResTypes::Animation, "animations/man.anim", 0);

        terrainRes = h3dAddResource(H3DResTypes::SceneGraph,  "terrains/terrain1/terrain1.scene.xml", 0 );

        logoRes = h3dAddResource(H3DResTypes::Material, "overlays/logo.material.xml", 0);

        debugTimeFlag = currentMillis();
    }

    void reportCameraCoordinates() {
        spdlog::trace("Camera position @ time {:.2f} is x:{:.1f}, y:{:.1f}, z:{:.1f}", t, gCamPosX, gCamPosY, gCamPosZ);
    }

    bool configure() override {
        model = h3dAddNodes(H3DRootNode, modelRes);
        h3dSetupModelAnimStage(model, 0, modelAnimationRes, 0, "", false);

        sky = h3dAddNodes(H3DRootNode, skyBoxRes);
        h3dSetNodeTransform(sky, 0, 0, 0, 0, 0, 0, 1000, 1000, 1000);
        h3dSetNodeFlags(sky, H3DNodeFlags::NoCastShadow, true);

        light = h3dAddLightNode(H3DRootNode, "Light1",  0, "LIGHTING","SHADOWMAP" );
        h3dSetNodeTransform(light, 0, 20, 50, -30, 0, 0, 1, 1, 1);
        h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 200);
        h3dSetNodeParamF(light, H3DLight::FovF, 0, 90);
        h3dSetNodeParamF(light, H3DLight::ShadowSplitLambdaF, 0, 0.9f );
        h3dSetNodeParamF(light, H3DLight::ShadowMapBiasF, 0, 0.001f );
        h3dSetNodeParamF(light, H3DLight::ColorF3, 0, 0.9f );
        h3dSetNodeParamF(light, H3DLight::ColorF3, 1, 0.7f );
        h3dSetNodeParamF(light, H3DLight::ColorF3, 2, 0.75f );
        h3dSetNodeParamI(light, H3DLight::ShadowMapCountI, 3 );

        terrain = h3dAddNodes(H3DRootNode, terrainRes);
        H3DRes matRes = h3dFindResource(H3DResTypes::Material, "terrains/terrain1/terrain1.material.xml");
        h3dSetMaterialUniform(matRes, "sunDir", 1, -1, 0, 0);

        return true;
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
        h3dSetNodeTransform(cam, gCamPosX, gCamPosY, gCamPosZ, 0, 0, 0, 1, 1, 1);
        // Configure orthographic frustrum on camera instead of perspective
//        h3dSetNodeParamI(cam, H3DCamera::OrthoI, 1);

        return true;
    }



    bool update(bool* keys, double dt) override {

        if (currentMillis() - debugTimeFlag > debugInterval) {
            reportCameraCoordinates();
            debugTimeFlag = currentMillis();
        }

        t += 10.0 * dt;
        h3dSetModelAnimParams(model, 0, t, 1.0f);
        h3dUpdateModel(model, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
        h3dSetNodeTransform(model, t * 10, 0, 0,
                            0, 0, 0,
                            1, 1, 1);
        if (keys[GLFW_KEY_J]) gCamLateralRotation += rotScale * dt;
        if (keys[GLFW_KEY_L]) gCamLateralRotation -= rotScale * dt;
        if (keys[GLFW_KEY_I]) gCamVerticalRotation += rotScale * dt;
        if (keys[GLFW_KEY_K]) gCamVerticalRotation -= rotScale * dt;
        if (keys[GLFW_KEY_U]) gCamRollRotation += rotScale * dt;
        if (keys[GLFW_KEY_O]) gCamRollRotation -= rotScale * dt;

        if (keys[GLFW_KEY_W]) gCamPosX += posScale * dt;
        if (keys[GLFW_KEY_S]) gCamPosX -= posScale * dt;
        if (keys[GLFW_KEY_A]) gCamPosZ += posScale * dt;
        if (keys[GLFW_KEY_D]) gCamPosZ -= posScale * dt;
        if (keys[GLFW_KEY_Q]) gCamPosY += posScale * dt;
        if (keys[GLFW_KEY_E]) gCamPosY -= posScale * dt;

        // Set new camera rotation and position
        h3dSetNodeTransform(cam,
                            gCamPosX, gCamPosY, gCamPosZ,
                            gCamVerticalRotation, gCamLateralRotation, gCamRollRotation,
                            1, 1, 1);

        const float ww = (float) h3dGetNodeParamI(cam, H3DCamera::ViewportWidthI) /
                         (float) h3dGetNodeParamI(cam, H3DCamera::ViewportHeightI);

        const float ovLogo[] = {
            ww - 0.29f, 0.87f, 0, 1,
            ww - 0.29f, 0.97f, 0, 0,
            ww - 0.03f, 0.97f, 1, 0,
            ww - 0.03f, 0.87f, 1, 1
        };

        h3dShowOverlays(ovLogo, 4, 1.f, 1.f, 1.f, 1.f, logoRes, 0);


    }




private:
    H3DRes skyBoxRes, modelRes, modelAnimationRes, pipeline, terrainRes, logoRes;
    H3DNode sky, model, light, terrain;

    double t = 0.0;
    double gCamLateralRotation = 0.0, gCamVerticalRotation = 0.0, gCamRollRotation = 0.0;
    double gCamPosX = 420, gCamPosY = 120, gCamPosZ = 510;
    const double posScale = 100.0;
    const double rotScale = 50.0;
};



#endif //GAMEENGINE_BASICSCENE_H
