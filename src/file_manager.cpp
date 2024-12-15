#include "file_manager.hpp"
#include <filesystem>
#include <iostream>
#include "imgui.h"

// 이미지 파일인지 확인하는 함수
bool FileManager::isImageFile(const std::string& filePath) {
    const std::vector<std::string> supportedExtensions = { ".jpg", ".jpeg", ".png", ".bmp" };
    std::string extension = std::filesystem::path(filePath).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end();
}

// 파일 다이얼로그 구현
std::string FileManager::fileDialog(const std::string& directoryPath) {
    std::string selectedFile;
    std::vector<std::string> files;

    // 지정된 경로에서 파일 목록 불러오기
    if (directoryPath.empty() || !std::filesystem::exists(directoryPath)) {
        std::cerr << "Error: Directory does not exist." << std::endl;
        return "";
    }

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }

    // ImGui를 사용하여 파일 다이얼로그 표시
    ImGui::Begin("File Dialog");
    if (ImGui::BeginListBox("Files")) {
        for (const auto& file : files) {
            if (ImGui::Selectable(file.c_str(), selectedFile == file)) {
                selectedFile = file;
            }
        }
        ImGui::EndListBox();
    }

    if (ImGui::Button("Select")) {
        ImGui::End();
        return directoryPath + "/" + selectedFile;
    }

    ImGui::End();
    return "";
}

// 이미지 파일 저장 다이얼로그 구현
std::string FileManager::saveFileDialog(const std::string& defaultFileName) {
    static char fileName[256];
    if (!defaultFileName.empty()) {
        strncpy(fileName, defaultFileName.c_str(), sizeof(fileName) - 1);
    }

    ImGui::Begin("Save File Dialog");
    ImGui::InputText("File Name", fileName, sizeof(fileName));

    if (ImGui::Button("Save")) {
        ImGui::End();
        return std::string(fileName);  // 선택한 파일 이름을 반환
    }

    ImGui::End();
    return "";
}

// 폴더에서 이미지 파일 목록을 가져오는 함수
std::vector<std::string> FileManager::getImageFilesInFolder(const std::string& folderPath) {
     std::vector<std::string> imageFiles;
    const std::vector<std::string> supportedExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".tiff"};

    // 지정된 폴더 경로가 유효한지 확인
    if (!std::filesystem::exists(folderPath)) {
        std::cerr << "Error: Folder does not exist - " << folderPath << std::endl;
        return imageFiles;
    }
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 이미지 확장자 필터링
            if (std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end()) {
                imageFiles.push_back(entry.path().string());
            }
        }
    }

    return imageFiles;
}

// 파일 경로에서 파일 이름만 추출
std::string FileManager::getFileName(const std::string& filePath) {
    return std::filesystem::path(filePath).filename().string();
}
