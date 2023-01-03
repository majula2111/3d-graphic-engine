//
// Created by Nikita Minaev on 17.12.2022.
//
#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>

namespace lve {
    class LveDevice {
    public:
        LveDevice(GLFWwindow *window);
        ~LveDevice();

        GLFWwindow *window;

        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
    private:
        void createInstance();

        void setupDebugMessenger();

        void pickPhysicalDevice();

        void createLogicalDevice();

        void createSurface();

        bool checkValidationLayerSupport();

        struct QueueFamilyIndices {
            std::optional<uint32_t> _graphicsFamily;
            std::optional<uint32_t> _presentFamily;

            bool isComplete() {
                return _graphicsFamily.has_value() && _presentFamily.has_value();
            }

            std::optional<uint32_t> & graphicsFamily() { return _graphicsFamily; }
            std::optional<uint32_t> & presentFamily() { return _graphicsFamily == _presentFamily ? _graphicsFamily : _presentFamily; }
        };

        uint32_t rateDeviceSuitability(VkPhysicalDevice device) const;

        std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        VkDebugUtilsMessengerEXT debugMessenger{};

        std::vector<const char*> gerRequiredExtensions();

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

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice vkPhysicalDevice) const;

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
    };
}