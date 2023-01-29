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
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
    private:
        struct QueueFamilyIndices {
            std::optional<uint32_t> _graphicsFamily;
            std::optional<uint32_t> _presentFamily;

            bool isComplete() {
                return _graphicsFamily.has_value() && _presentFamily.has_value();
            }

            std::optional<uint32_t> & graphicsFamily() { return _graphicsFamily; }
            std::optional<uint32_t> & presentFamily() { return _graphicsFamily == _presentFamily ? _graphicsFamily : _presentFamily; }
        };

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char*> deviceExtensions = {
                "VK_KHR_portability_subset",
                "VK_KHR_swapchain",
        };

        VkDebugUtilsMessengerEXT debugMessenger{};

        void createInstance();

        void setupDebugMessenger();

        void pickPhysicalDevice();

        void createLogicalDevice();

        void createSurface();

        void createSwapChain();

        void createImageViews();

        bool checkValidationLayerSupport();

        bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;

        uint32_t rateDeviceSuitability(VkPhysicalDevice device) const;

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

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice) const;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
    };
}