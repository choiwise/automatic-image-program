#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "vulkan_render.hpp"
#include "imgui_interface.hpp"
#include <iostream>

void check_vk_result(VkResult err) {
    if (err != VK_SUCCESS) {
        std::cerr << "Vulkan Error: " << err << std::endl;
        if (err < 0) {
            abort();
        }
    }
}

int main() {
    // GLFW 초기화
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Renderer 객체 생성 및 초기화
    Renderer renderer;

    try {
        renderer.initWindow();  // 창 초기화
        renderer.initVulkan();  // Vulkan 초기화
    } catch (const std::exception& e) {
        std::cerr << "Initialization failed: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    // ImGui 컨텍스트 초기화
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // ImGui 스타일 설정
    ImGui::StyleColorsDark();

    // ImGui GLFW 백엔드 초기화
    ImGui_ImplGlfw_InitForVulkan(renderer.getWindow(), true);

    // Vulkan 초기화 정보 설정
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
    init_info.CheckVkResultFn = check_vk_result;

    // ImGui Vulkan 백엔드 초기화
    ImGui_ImplVulkan_Init(&init_info);

    // 폰트 업로드
    VkCommandBuffer command_buffer = renderer.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture();
    renderer.endSingleTimeCommands(command_buffer);

    // 메인 루프
    while (!glfwWindowShouldClose(renderer.getWindow())) {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI 렌더링 함수 호출
        ImGuiInterface::renderUI(renderer);

        // Vulkan 프레임 렌더링
        try {
            renderer.drawFrame();
        } catch (const std::exception& e) {
            std::cerr << "Error during frame rendering: " << e.what() << std::endl;
            break;
        }

        // ImGui 그리기
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer.getCommandBuffer());
    }

    // 리소스 정리
    renderer.waitIdle();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    renderer.cleanup();

    return 0;
}
