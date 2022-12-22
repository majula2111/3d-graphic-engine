//
// Created by Nikita Minaev on 14.12.2022.
//
#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace lve {
    class LvePipeline {
    public:
        LvePipeline(const std::string& vertFilepath, const std::string& fragFilepath);
    private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
        VkShaderModule createShaderModule(const std::vector<char>& code);
    };
}
