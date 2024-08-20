#ifndef VULKAN_RENDER_HPP
#define VULKAN_RENDER_HPP

#include <vulkan/vulkan.h>
#include <string>

class VulkanRender {
public:
    VulkanRender();
    ~VulkanRender();

    void initVulkan();
    void cleanup();
    void renderFrame();

    void loadTexture(const std::string& filePath);
    void scaleImage(float scaleFactor);
    
private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();

    // Vulkan 관련 변수들
    VkInstance instance;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    // 추가적인 Vulkan 리소스들...
};

#endif // VULKAN_RENDER_HPP
