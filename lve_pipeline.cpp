//
// Created by Nikita Minaev on 14.12.2022.
//
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "lve_pipeline.h"

namespace lve {
    LvePipeline::LvePipeline(const std::string &vertFilepath, const std::string &fragFilepath, const LveDevice &lveDevice) {
        vkDevice = lveDevice.device;
        vkViewport = createViewPort(lveDevice.swapChainExtent);
        scissor = createRect2D(lveDevice.swapChainExtent, {0, 0});

        createGraphicsPipeline(vertFilepath, fragFilepath);
        createPipelineLayout();
        createRenderPass(lveDevice.swapChainImageFormat);
    }

    LvePipeline::~LvePipeline() {
        vkDestroyPipeline(vkDevice, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(vkDevice, pipelineLayout, nullptr);
        vkDestroyRenderPass(vkDevice, renderPass, nullptr);
    }

    std::vector<char> LvePipeline::readFile(const std::string &filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary };

        if(!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void LvePipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath) {
        auto vertShaderCode = readFile(vertFilepath);
        auto fragShaderCode = readFile(fragFilepath);

        std::cout << "vert code size: " << vertShaderCode.size() << '\n';
        std::cout << "frag code size: " << fragShaderCode.size() << '\n';

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo shaderStages[] = {
                createShaderStageInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT),
                createShaderStageInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT),
        };

        VkPipelineDynamicStateCreateInfo dynamicState = createDynamicStateInfo();
        VkPipelineVertexInputStateCreateInfo vertexInputState = createVertexInputStateInfo();
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssemblyStateInfo();
        VkPipelineViewportStateCreateInfo viewPortState = createViewportStateInfo();
        VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizationStateInfo();
        VkPipelineMultisampleStateCreateInfo multisampling = createMultisampleStateInfo();
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = createColorBlendAttachmentState();
        VkPipelineColorBlendStateCreateInfo colorBlending = createColorBlendStateInfo(colorBlendAttachmentState);


        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;

        pipelineInfo.pVertexInputState = &vertexInputState;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewPortState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(vkDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(vkDevice, vertShaderModule, nullptr);
    }

    void LvePipeline::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if(vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void LvePipeline::createRenderPass(VkFormat imageFormat) {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = imageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    VkViewport LvePipeline::createViewPort(VkExtent2D extent2D) {
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) extent2D.width;
        viewport.height = (float) extent2D.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        return viewport;
    }

    VkRect2D createRect2D(VkExtent2D extent2D, VkOffset2D offset2D) {
        VkRect2D react2D;
        react2D.offset = offset2D;
        react2D.extent = extent2D;

        return react2D;
    }

    VkShaderModule LvePipeline::createShaderModule(const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if(vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

    VkPipelineShaderStageCreateInfo LvePipeline::createShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage) {
        VkPipelineShaderStageCreateInfo createShaderInfo{};
        createShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createShaderInfo.stage = stage;
        createShaderInfo.module = shaderModule;
        createShaderInfo.pName = "main";

        return createShaderInfo;
    }

    VkPipelineDynamicStateCreateInfo LvePipeline::createDynamicStateInfo() {
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateInfo.pDynamicStates = dynamicStates.data();

        return dynamicStateInfo;
    }

    VkPipelineVertexInputStateCreateInfo LvePipeline::createVertexInputStateInfo() {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        return vertexInputInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo LvePipeline::createInputAssemblyStateInfo() {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        return inputAssembly;
    }

    VkPipelineViewportStateCreateInfo LvePipeline::createViewportStateInfo() {
        VkPipelineViewportStateCreateInfo viewPortState{};
        viewPortState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewPortState.viewportCount = 1;
        viewPortState.pViewports = &vkViewport;
        viewPortState.scissorCount = 1;
        viewPortState.pScissors = &scissor;

        return viewPortState;
    }

    VkPipelineRasterizationStateCreateInfo LvePipeline::createRasterizationStateInfo() {
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        return rasterizer;
    }

    VkPipelineMultisampleStateCreateInfo LvePipeline::createMultisampleStateInfo() {
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        return multisampling;
    }

    VkPipelineColorBlendAttachmentState LvePipeline::createColorBlendAttachmentState() {
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        return colorBlendAttachment;
    }

    VkPipelineColorBlendStateCreateInfo LvePipeline::createColorBlendStateInfo(VkPipelineColorBlendAttachmentState colorBlendAttachment) {
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_TRUE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        return colorBlending;
    }
}