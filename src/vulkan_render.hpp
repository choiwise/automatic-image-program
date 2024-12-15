#ifndef VULKAN_RENDER_HPP
#define VULKAN_RENDER_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <optional>
#include <set>
#include <string>
#include <stdexcept>
#include <array>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "file_manager.hpp" 
#include "imgui_interface.hpp"


struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        // 위치 속성에 대한 정의 (Location 0)
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // 색상 속성에 대한 정의 (Location 1)
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
class Renderer {
public:
    
    void run();
    void initVulkanBase();
    void initImageRenderingPipeline();
    void recordInitialCommandBuffer(VkCommandBuffer commandBuffer);
    void recordEditorCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void initWindow();
    void cleanup();
   void drawFrame(ImGuiInterface& imguiInterface);
    void waitIdle();
    void createSurface();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();
    void updateDescriptorSets();
    VkExtent2D getSwapChainExtent() const;
    void updateUniformBuffer(uint32_t currentImage) ;
    void createUniformBuffers();
    VkDescriptorSet getDescriptorSet() ;
    void createTextureImage(const std::string& imagePath);
    void createTextureImageView();
    void createTextureSampler();
    void updateImageTexture(const cv::Mat& image);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
   const VkPipelineLayout& getPipelineLayout() const ;
    VkInstance getInstance() const;
    VkPhysicalDevice getPhysicalDevice() const;
    VkDevice getDevice() const;
    VkQueue getGraphicsQueue() const;
    VkCommandBuffer getCommandBuffer() const;
    uint32_t getQueueFamily() const;
    uint32_t getMinImageCount() const;
    uint32_t getImageCount() const;
    VkRenderPass getRenderPass() const;
      VkRenderPass getImGuiRenderPass() const;
    VkDescriptorPool getImGuiDescriptorPool() const;
    VkCommandBuffer getImGuiCommandBuffer() const;
    VkFramebuffer getImGuiFramebuffer(uint32_t index) const;
    void createImGuiRenderPass();
    void createImGuiDescriptorPool();
    void createImGuiCommandBuffers();
    void createImGuiFramebuffers();
    void createVertexBuffer();
    bool isImageLoaded() const;
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
                            VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkCommandBuffer beginSingleTimeCommands();
    void Renderer::endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool);
    GLFWwindow* getWindow() const;
    VkDescriptorPool getDescriptorPools() const;
    VkImage textureImage;
    VkImageView textureImageView;
    VkSampler textureSampler;
     VkImageView defaulttextureImageView;
    VkSampler defaultTextureSampler;
    VkDeviceMemory textureImageMemory;
    void updateTextureDescriptorSets();
    VkCommandPool getImGuiCommandPool() const;
    void submitImGuiCommandBuffer(VkCommandBuffer commandBuffer);
    void renderImGui(ImGuiInterface& imguiInterface);
    void handleSwapChainRecreation() { recreateSwapChain(); } 
    VkFence getInFlightFence() const { return inFlightFences[currentFrame]; }
    VkSemaphore getImageAvailableSemaphore() const { return imageAvailableSemaphores[currentFrame]; }
    VkSwapchainKHR getSwapChain() const { return swapChain; }
    int width=1280;
    int height=720;
    
     size_t currentFrame = 0;

     
       // UI와 관련된 기능
    void createDefaultBackground(); // 기본 흰 배경 생성
    void updateTexture(const std::string& filePath); // 로드한 이미지로 텍스처 갱신
    void createEmptyTexture(int width, int height, VkImage& image, VkDeviceMemory& imageMemory, VkImageView& imageView); 
    void createDefaultTexture();
    VkImage backgroundTextureImage;                // 배경 텍스처 이미지
    VkDeviceMemory backgroundTextureImageMemory;    // 배경 텍스처 이미지 메모리
    VkImageView backgroundTextureImageView;         // 배경 텍스처 이미지 뷰

    VkImage imageTextureImage;                      // 이미지 텍스처 이미지
    VkDeviceMemory imageTextureImageMemory;         // 이미지 텍스처 이미지 메모리
    VkImageView imageTextureImageView; 
    void createImageTexture(const std::string& imagePath);
     VkCommandBuffer beginImGuiSingleTimeCommandBuffer();
private:

    VkRenderPass imguiRenderPass;
    VkDescriptorPool imguiDescriptorPool;
    std::vector<VkCommandBuffer> imguiCommandBuffers;
    std::vector<VkFramebuffer> imguiFramebuffers;
    VkBuffer indexBuffer;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool  = VK_NULL_HANDLE;
    VkDescriptorSet descriptorSet;
    std::vector<VkDescriptorSet> descriptorSets;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    VkCommandPool imguicommandPool;
    std::vector<VkCommandBuffer> commandBuffers;
     VkImage defaultTextureImage;          
    VkDeviceMemory defaultTextureImageMemory; 
    VkImageView defaultTextureImageView;  
    VkDescriptorPool imguiPool;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    const int MAX_FRAMES_IN_FLIGHT = 2;
     VkImage defaultBackgroundImage;
    bool imageLoaded = false;  
  bool framebufferResized = false;
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    #ifdef NDEBUG
    const bool enableValidationLayers = false;
    #else
    const bool enableValidationLayers = true;
    #endif
     void setupDebugMessenger();  
    void createInstance();
   
    void createSurface(GLFWwindow* window);
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
    void createDefaultTextureSampler();
    void Renderer::createImGuiCommandPool();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void recreateSwapChain();
    void cleanupSwapChain();
    void createPipelineLayout();
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
   
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
 
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void framebufferResizeCallbackStatic(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

   const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};
std::vector<uint32_t> indices = {
    0, 1, 2,  // 첫 번째 삼각형 (꼭짓점 0, 1, 2)
    2, 3, 0   // 두 번째 삼각형 (꼭짓점 2, 3, 0)
};

};


#endif // VULKAN_RENDER_HPP
