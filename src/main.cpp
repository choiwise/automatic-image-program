#include "vulkan_render.hpp"
#include "imgui_interface.hpp"
#include "image_processing.hpp"
#include <iostream>
// main.cpp
int main() {
    try {
        Renderer renderer;
        ImageProcessor imageProcessor;
        ImGuiInterface imguiInterface(imageProcessor, renderer);
        renderer.run();
        imguiInterface.initUI();

        if (imguiInterface.getCurrentUIState() == ImGuiInterface::UIState::EDITOR) {
            renderer.initImageRenderingPipeline();
        }

        while (!glfwWindowShouldClose(renderer.getWindow())) {
            glfwPollEvents();

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            renderer.drawFrame(imguiInterface);
        }

        renderer.waitIdle();
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}