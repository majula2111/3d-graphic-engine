//
// Created by Nikita Minaev on 14.12.2022.
//
#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "lve_device.h"

namespace lve {
    class LvePipeline {
    public:
        LvePipeline(const std::string& vertFilepath, const std::string& fragFilepath, const LveDevice &lveDevice);
        ~LvePipeline();

        VkDevice vkDevice;
        VkViewport vkViewport;
        VkRect2D scissor;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };
    private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

        void createPipelineLayout();

        void createRenderPass(VkFormat imageFormat);

        VkViewport createViewPort(VkExtent2D extent2D);

        VkRect2D createRect2D(VkExtent2D extent2D, VkOffset2D offset2D);

        VkShaderModule createShaderModule(const std::vector<char>& code);

        VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage);

        VkPipelineDynamicStateCreateInfo createDynamicStateInfo();

        VkPipelineViewportStateCreateInfo createViewportStateInfo();

        VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo();

        VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();

        VkPipelineRasterizationStateCreateInfo createRasterizationStateInfo();

        VkPipelineMultisampleStateCreateInfo createMultisampleStateInfo();

        VkPipelineColorBlendAttachmentState createColorBlendAttachmentState();

        VkPipelineColorBlendStateCreateInfo createColorBlendStateInfo(VkPipelineColorBlendAttachmentState colorBlendAttachment);
    };
}
