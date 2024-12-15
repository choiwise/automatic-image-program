#ifndef IMGUI_INTERFACE_HPP
#define IMGUI_INTERFACE_HPP

#include <string>
#include <opencv2/opencv.hpp>
#include <imgui.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "image_processing.hpp"

class Renderer;

class ImGuiInterface {
public:
   
    enum class UIState {
        INITIAL,     // 초기 화면 (이미지 열기/새 문서 생성)
        EDITOR       // 메인 에디터 화면
    };

    ImGuiInterface(ImageProcessor& imageProcessor, Renderer& renderInstance);
    
    void initUI();
    void renderUI();
    void cleanupUI();
    void openImage(const std::string& filePath);
    void createNewDocument(int width, int height);
    bool isUIInitialized() const { return isInitialized; }
    ImGuiInterface::UIState getCurrentUIState() const { return currentState; }

private:
    void renderInitialUI();
    void renderEditorUI();

    UIState currentState = UIState::INITIAL;
    bool isInitialized = false;
    bool showInitialUI = true;
    ImageProcessor& imp;
    Renderer& renderer;
    cv::Mat currentImage;
    int newDocWidth = 1280;
    int newDocHeight = 720;
    std::string currentFilePath;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
};

#endif // IMGUI_INTERFACE_HPP