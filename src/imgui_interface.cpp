#include "imgui_interface.hpp"
#include "file_manager.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "vulkan_render.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <algorithm>

cv::Mat ImGuiInterface::currentImage;
std::string ImGuiInterface::currentFilePath;
Renderer renderer;

void ImGuiInterface::initUI(GLFWwindow* window, Renderer& renderer) {
    // ImGui 초기화
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui::StyleColorsDark();

    // Vulkan 초기화 관련 코드
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = renderer.getInstance();
    init_info.PhysicalDevice = renderer.getPhysicalDevice();
    init_info.Device = renderer.getDevice();
    init_info.QueueFamily = renderer.getQueueFamily();
    init_info.Queue = renderer.getGraphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = renderer.getDescriptorPool();
    init_info.Allocator = nullptr;
    init_info.MinImageCount = renderer.getMinImageCount();
    init_info.ImageCount = renderer.getImageCount();
    init_info.CheckVkResultFn = [](VkResult err) {
        if (err != VK_SUCCESS) {
            throw std::runtime_error("Vulkan Error");
        }
    };

    ImGui_ImplVulkan_Init(&init_info);

    // ImGui 폰트 로드
    VkCommandBuffer command_buffer = renderer.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture();
    renderer.endSingleTimeCommands(command_buffer);
}

void ImGuiInterface::renderUI(Renderer& renderer) {
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // UI 메뉴들 렌더링
    showMainMenuBar();
    showImageProcessingUI();

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer.getCommandBuffer());
}

void ImGuiInterface::cleanupUI() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


std::string ImGuiInterface::openFileDialog() {
    std::string selectedFile;

    // 파일 선택 다이얼로그 열기
    if (ImGui::Button("Open File Dialog")) {
        selectedFile = FileManager::fileDialog("your/custom/path");
        if (!selectedFile.empty()) {
            std::cout << "Selected File: " << selectedFile << std::endl;
        }
    }

    return selectedFile;
}

std::string ImGuiInterface::saveFileDialog() {
    std::string savePath;

    // 파일 저장 다이얼로그 열기
    if (ImGui::Button("Save File Dialog")) {
        savePath = FileManager::fileDialog("your/custom/save/path");
        if (!savePath.empty()) {
            std::cout << "Save File: " << savePath << std::endl;
        }
    }

    return savePath;
}
void ImGuiInterface::showMainMenuBar() {
    // 여기에서 실제로 UI 메뉴바를 구현하는 코드를 추가해야 합니다.
    // 예시로 ImGui::BeginMainMenuBar()와 관련된 코드를 추가할 수 있습니다.
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // "File" 메뉴 항목을 추가합니다.
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                // "Open" 메뉴 항목이 선택되었을 때의 동작을 추가합니다.
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ImGuiInterface::showImageProcessingUI() {
    // 여기에서 실제로 UI를 구현하는 코드를 추가해야 합니다.
    // 예를 들어, 이미지 처리와 관련된 슬라이더, 버튼 등을 추가할 수 있습니다.
    ImGui::Begin("Image Processing");
    // 예: 밝기 조절 슬라이더
    static int brightness = 50;
    ImGui::SliderInt("Brightness", &brightness, 0, 100);
    ImGui::End();
}
