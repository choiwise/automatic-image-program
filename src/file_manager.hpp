#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "imgui.h"
class FileManager {
public:
    struct ImageFileConfig {
        std::string path;
        std::string fileName;
        std::string filters;
        cv::Mat imageData;

        ImageFileConfig() : path("."), fileName("default_image_name"), filters(".png,.jpg,.jpeg,.bmp") {}
    };//파일 선택 함수 

    // 기본 이미지 파일 구성 가져오기
    static ImageFileConfig getDefaultImageConfig();

    // 폴더 선택 대화상자 (임시, 실제 구현 필요)
    static std::vector<std::string> selectFoldersDialog();

    // 파일이 이미지 파일인지 확인하는 함수
    static bool isImageFile(const std::string& filePath);

    // 여러 폴더를 처리하는 함수
    static void processFolders(const std::vector<std::string>& folderPaths);

    // 특정 폴더 내의 이미지 파일들을 가져오는 함수
    static std::vector<std::string> getImageFilesInFolder(const std::string& folderPath);

    // 주어진 경로에서 폴더 이름을 추출하는 함수
    static std::string getFolderName(const std::string& folderPath);

    // 특정 디렉토리가 존재하지 않으면 생성하는 함수
    static void createDirectory(const std::string& directoryPath);

    // 파일 경로에서 파일 이름만 추출하는 함수
    static std::string getFileName(const std::string& filePath);

    // 폴더 내 이미지 파일들을 처리하는 함수
    static void processFolder(const std::string& folderPath);

    // 지정된 디렉토리의 파일 목록을 불러오는 함수
    static std::vector<std::string> getFilesInDirectory(const std::string& directoryPath);

    // ImGui를 사용해 파일 선택 다이얼로그를 구현하는 함수
    static std::string fileDialog(const std::string& directoryPath);
};

#endif // FILE_MANAGER_HPP
