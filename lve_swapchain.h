//
// Created by Nitika on 11.08.2023.
//
#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include "lve_device.h"

namespace lve {
    class LveSwapChain {
    public:
        LveSwapChain(GLFWwindow *w, const LveDevice &lveDevice);
        ~LveSwapChain();

        VkExtent2D swapChainExtent;
        VkFormat swapChainImageFormat;
        std::vector<VkImageView> swapChainImageViews;
        VkSwapchainKHR swapChain;

    private:
        GLFWwindow *window;

        const LveDevice *device;

        std::vector<VkImage> swapChainImages;

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        void createSwapChain();

        void createImageViews();

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice) const;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };
}
