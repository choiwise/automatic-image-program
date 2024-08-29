// imgui_interface.hpp

#ifndef IMGUI_INTERFACE_HPP
#define IMGUI_INTERFACE_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include "vulkan_render.hpp" // Renderer 클래스 포함

class ImGuiInterface {
public:
    // UI 초기화 및 렌더링 관련 함수들
   static void initUI(GLFWwindow* window, Renderer& renderer);
    static void renderUI(Renderer& renderer);
    static void cleanupUI();

    // 파일 다이얼로그를 여는 함수들
    static std::string openFileDialog();
    static std::string saveFileDialog();
    // 이미지 파일 열기와 저장 관련 함수들

    static void saveImage(const std::string& filePath, const cv::Mat& image);

    // 이미지 처리 관련 함수들
    static cv::Mat processImage(const cv::Mat& image);
    static cv::Mat adjustBrightnessContrast(const cv::Mat& image, double alpha, int beta);

    // UI 다이얼로그 관련 함수들
    static void showMainMenuBar();
    static void showImageProcessingUI();
    static void showBrightnessContrastDialog();

    // 폴더 및 파일 관리 관련 함수들
    static std::string selectFoldersDialog();
    static std::vector<std::string> getImageFilesInFolder(const std::string& folderPath);
    static std::string getFolderName(const std::string& folderPath);
    static void createDirectory(const std::string& directoryPath);
    static std::string getFileName(const std::string& filePath);
    static void processFolder(const std::string& folderPath);
    static void processSelectedFolders(const std::vector<std::string>& folders);

    // 키보드 입력 처리 함수
    static void processInput(GLFWwindow* window);

    // 이미지 및 파일 경로 관리 함수들
    static void openImage(const std::string& filePath);
    static void saveCurrentImage();

private:
    // 전역 변수로 사용할 현재 이미지 및 파일 경로
    static cv::Mat currentImage;
    static std::string currentFilePath;


  
};

#endif // IMGUI_INTERFACE_HPP
