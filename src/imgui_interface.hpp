#ifndef IMGUI_INTERFACE_HPP
#define IMGUI_INTERFACE_HPP

#include "vulkan_render.hpp"
#include <imgui.h>

class ImGuiInterface {
public:
    ImGuiInterface();
    ~ImGuiInterface();

    void init(VulkanRender& vulkanRender);
    void render();
    void cleanup();

    // UI 요소들 추가
    void showMainMenu();
    void showImageSettings();
    void showLayerManager();

private:
    VulkanRender* vulkanRender;
    // 필요한 ImGui 관련 변수들...
};

#endif // IMGUI_INTERFACE_HPP
