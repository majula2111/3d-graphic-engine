//
// Created by Nikita Minaev on 14.12.2022.
//
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "lve_pipeline.h"

namespace lve {
    LvePipeline::LvePipeline(const std::string &vertFilepath, const std::string &fragFilepath) {
        createGraphicsPipeline(vertFilepath, fragFilepath);
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
    }

    VkShaderModule LvePipeline::createShaderModule(const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        return nullptr;
    }
}