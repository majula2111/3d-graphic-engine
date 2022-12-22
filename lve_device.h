//
// Created by Nikita Minaev on 17.12.2022.
//
#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace lve {
    class LveDevice {
    public:
        LveDevice();
        ~LveDevice();

        VkInstance instance;
    private:
        void createInstance();

        bool checkValidationLayerSupport();

        std::vector<const char*> gerRequiredExtensions();

        void setupDebugMessenger();

        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance vkInstance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger
        );

        VkResult DestroyDebugUtilsMessengerEXT(
            VkInstance vkInstance,
            VkDebugUtilsMessengerEXT pDebugMessenger,
            const VkAllocationCallbacks* pAllocator
        );

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        VkDebugUtilsMessengerEXT debugMessenger;

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
    };
}