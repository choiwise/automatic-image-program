#include "imgui_interface.hpp"
#include "vulkan_render.hpp"
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <iostream>
#include "file_manager.hpp"

ImGuiInterface::ImGuiInterface(ImageProcessor& imageProcessor, Renderer& renderInstance)
    : imp(imageProcessor), renderer(renderInstance) {
    currentState = UIState::INITIAL;
    std::cout << "ImGuiInterface constructed with INITIAL state" << std::endl;
}
void ImGuiInterface::initUI() {
    if (isInitialized) return;
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForVulkan(renderer.getWindow(), true)) {
        throw std::runtime_error("Failed to initialize ImGui GLFW backend!");
    }

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = renderer.getInstance();
    init_info.PhysicalDevice = renderer.getPhysicalDevice();
    init_info.Device = renderer.getDevice();
    init_info.QueueFamily = renderer.getQueueFamily();
    init_info.Queue = renderer.getGraphicsQueue();
    init_info.DescriptorPool = renderer.getImGuiDescriptorPool();
    init_info.MinImageCount = renderer.getMinImageCount();
    init_info.ImageCount = renderer.getImageCount();
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.RenderPass = renderer.getImGuiRenderPass();

    if (!ImGui_ImplVulkan_Init(&init_info)) {
        throw std::runtime_error("Failed to initialize ImGui Vulkan backend!");
    }

    isInitialized = true;
}

void ImGuiInterface::renderUI() {
    if (!isInitialized) {
        std::cout << "ImGui not initialized, skipping render" << std::endl;
        return;
    }
     
    try {
        std::cout << "Rendering UI in state: " << static_cast<int>(currentState) << std::endl;
        
        switch (currentState) {
            case UIState::INITIAL:
                std::cout << "Rendering initial UI" << std::endl;
                renderInitialUI();
                break;
            case UIState::EDITOR:
                std::cout << "Rendering editor UI" << std::endl;
                renderEditorUI();
                break;
            default:
                std::cout << "Unknown UI state" << std::endl;
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in renderUI: " << e.what() << std::endl;
    }
}

void ImGuiInterface::renderInitialUI() {
     std::cout << "Starting renderInitialUI" << std::endl;

    // 전체 화면 크기 가져오기
    int width, height;
    glfwGetFramebufferSize(renderer.getWindow(), &width, &height);
    std::cout << "Window size: " << width << "x" << height << std::endl;

    // 윈도우 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));
    
    // 디버그 정보 출력
    std::cout << "Setting up main window" << std::endl;

    // 메인 윈도우 시작
    bool open = true;
    ImGui::Begin("Initial UI", &open, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse);

    // 버튼 위치 계산
    float centerX = width * 0.5f;
    float centerY = height * 0.5f;
    ImGui::SetCursorPos(ImVec2(centerX - 200, centerY - 50));

    // Open Image 버튼
    std::cout << "Adding Open Image button" << std::endl;
    if (ImGui::Button("Open Image", ImVec2(400, 50))) {
        std::cout << "Open Image button clicked" << std::endl;
        std::string filePath = FileManager::fileDialog("");
        if (!filePath.empty()) {
            openImage(filePath);
            currentState = UIState::EDITOR;
        }
    }

    // New Document 버튼
    ImGui::SetCursorPos(ImVec2(centerX - 200, centerY + 10));
    std::cout << "Adding New Document button" << std::endl;
    if (ImGui::Button("New Document", ImVec2(400, 50))) {
        std::cout << "New Document button clicked" << std::endl;
        ImGui::OpenPopup("New Document Settings");
    }

    // New Document 팝업
    if (ImGui::BeginPopupModal("New Document Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputInt("Width", &newDocWidth);
        ImGui::InputInt("Height", &newDocHeight);

        if (ImGui::Button("Create", ImVec2(120, 0))) {
            createNewDocument(newDocWidth, newDocHeight);
            currentState = UIState::EDITOR;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();

    std::cout << "Finished renderInitialUI" << std::endl;
}

void ImGuiInterface::renderEditorUI() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                std::string filePath = FileManager::fileDialog(currentFilePath);
                if (!filePath.empty()) {
                    openImage(filePath);
                }
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                // 저장 구현
            }
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                // 다른 이름으로 저장 구현
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                glfwSetWindowShouldClose(renderer.getWindow(), GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Tools");
    // 도구 버튼들 구현
    ImGui::End();

    ImGui::Begin("Adjustments");
    static float brightness = 0.0f;
    static float contrast = 1.0f;
    static float saturation = 1.0f;

    if (ImGui::SliderFloat("Brightness", &brightness, -1.0f, 1.0f)) {
        imp.adjustBrightness(currentImage, brightness);
    }
    if (ImGui::SliderFloat("Contrast", &contrast, 0.5f, 2.0f)) {
        imp.adjustContrast(currentImage, contrast);
    }
    if (ImGui::SliderFloat("Saturation", &saturation, 0.0f, 2.0f)) {
        imp.adjustSaturation(currentImage, saturation);
    }
    ImGui::End();

    ImGui::Begin("Image View");
    // 이미지 표시 구현
    ImGui::End();
}

void ImGuiInterface::openImage(const std::string& filePath) {
    cv::Mat img = cv::imread(filePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        throw std::runtime_error("Failed to load image!");
    }
    currentImage = img;
    renderer.updateTexture(filePath);
    currentFilePath = filePath;
}

void ImGuiInterface::createNewDocument(int width, int height) {
    currentImage = cv::Mat::zeros(height, width, CV_8UC3);
    currentImage = cv::Scalar(255, 255, 255);  // 흰색 배경
      VkImage newTextureImage;
    VkDeviceMemory newTextureMemory;
    VkImageView newTextureImageView;
     renderer.createEmptyTexture(width, height, newTextureImage, newTextureMemory, newTextureImageView);
}

void ImGuiInterface::cleanupUI() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}