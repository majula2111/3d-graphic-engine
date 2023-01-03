//
// Created by Nikita Minaev on 13.12.2022.
//
#include "lve_window.h"

namespace lve {
    LveWindow::LveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{std::move(name)} {
        initWindow();
    }

    LveWindow::~LveWindow() {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void LveWindow::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
}