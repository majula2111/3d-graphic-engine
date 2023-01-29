//
// Created by Nikita Minaev on 13.12.2022.
//
#pragma once

#include "lve_window.h"
#include "lve_device.h"
#include "lve_pipeline.h"

namespace lve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 680;
        static constexpr int HEIGHT = 420;

        void run();
    private:
        LveWindow lveWindow{WIDTH, HEIGHT, "hello vulkan!"};
        LveDevice lveDevice{lveWindow.window()};
        LvePipeline lvePipeline{
            "/Users/majula/CLionProjects/3DGraphicEngine/shaders/simple_shader.vert.spv",
            "/Users/majula/CLionProjects/3DGraphicEngine/shaders/simple_shader.frag.spv",
            lveDevice
        };
    };
}
