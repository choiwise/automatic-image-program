#include "file_manager.hpp"
#include "image_processing.hpp" // 이미지 처리 함수 포함
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>


 std::vector<std::string> FileManager::selectFoldersDialog() {
    std::vector<std::string> selectedFolders;


    return selectedFolders;
}

const std::vector<std::string> supportedImageExtensions = { ".jpg", ".jpeg", ".png", ".bmp" };

// 파일이 이미지 파일인지 확인하는 함수
bool FileManager::isImageFile(const std::string& filePath) {
    std::string extension = std::filesystem::path(filePath).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // 확장자를 소문자로 변환

    return std::find(supportedImageExtensions.begin(), supportedImageExtensions.end(), extension) != supportedImageExtensions.end();
}

// 여러 폴더를 처리하는 함수
void FileManager::processFolders(const std::vector<std::string>& folderPaths) {
    for (const auto& folderPath : folderPaths) {
        processFolder(folderPath);
    }
}

// 특정 폴더 내의 이미지 파일들을 가져오는 함수
std::vector<std::string> FileManager::getImageFilesInFolder(const std::string& folderPath) {
    std::vector<std::string> imageFiles;
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && isImageFile(entry.path().string())) {
            imageFiles.push_back(entry.path().string());
        }
    }
    return imageFiles;
}

// 주어진 경로에서 폴더 이름을 추출합니다.
std::string FileManager::getFolderName(const std::string& folderPath) {
    return std::filesystem::path(folderPath).filename().string();
}

// 새로운 디렉토리를 생성합니다.
void FileManager::createDirectory(const std::string& directoryPath) {
    if (!std::filesystem::exists(directoryPath)) {
        std::filesystem::create_directory(directoryPath);
    }
}

// 파일 경로에서 파일 이름만 추출합니다.
std::string FileManager::getFileName(const std::string& filePath) {
    return std::filesystem::path(filePath).filename().string();
}

// 폴더 내 이미지 파일들을 처리하는 함수
void FileManager::processFolder(const std::string& folderPath) {
    // 이미지 파일 리스트를 가져옴
    std::vector<std::string> imageFiles = getImageFilesInFolder(folderPath);
    
    std::string outputFolder = folderPath + "/upto_" + getFolderName(folderPath);
    createDirectory(outputFolder);

    for (const auto& imageFile : imageFiles) {
        cv::Mat image = cv::imread(imageFile);
        if (image.empty()) {
            std::cerr << "Failed to load image: " << imageFile << std::endl;
            continue;
        }

        // 이미지 처리 (예: 이미지 업스케일링, 색상 조정 등)
        cv::Mat processedImage = ImageProcessor::processImage(image);

        // 결과 저장
        std::string outputFilePath = outputFolder + "/upto_" + getFileName(imageFile);
        cv::imwrite(outputFilePath, processedImage);
    }
}
FileManager::ImageFileConfig FileManager::getDefaultImageConfig() {
    ImageFileConfig config;
    config.path = "your/custom/path"; // 필요 시 커스터마이즈 가능
    config.fileName = "your_default_image_name";
    config.filters = ".png,.jpg,.jpeg,.bmp";
    
    // OpenCV를 사용하여 이미지 데이터 초기화
    config.imageData = cv::imread(config.path + "/" + config.fileName);

    return config;
}

// 지정된 디렉토리의 파일 목록을 불러오는 함수
std::vector<std::string> FileManager::getFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

// ImGui를 사용하여 파일 선택 다이얼로그를 구현하는 함수
std::string FileManager::fileDialog(const std::string& directoryPath) {
    static std::string selectedFile;
    std::vector<std::string> files = getFilesInDirectory(directoryPath);

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
        return selectedFile;
    }

    ImGui::End();
    return "";
}