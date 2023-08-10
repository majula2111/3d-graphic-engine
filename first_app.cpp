//
// Created by Nikita Minaev on 13.12.2022.
//
#include "first_app.h"

namespace lve {
    void FirstApp::run() {
        while(!lveWindow.shouldClose()) {
            glfwPollEvents();
            lvePipeline.drawFrame();
        }

        vkDeviceWaitIdle(lveDevice.device);
    }
}