#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class FileManager {
public:
    // 파일 다이얼로그 표시 및 선택된 파일 반환
    static std::string fileDialog(const std::string& directoryPath = "");  // 기본 경로를 설정할 수 있도록 수정
    static std::string saveFileDialog(const std::string& defaultFileName = ""); // 멤버 함수 추가

    // 폴더에서 이미지 파일 목록 가져오기
    static std::vector<std::string> getImageFilesInFolder(const std::string& folderPath);
    
    // 파일 경로에서 파일 이름을 가져옴
    static std::string getFileName(const std::string& filePath);
    
    // 기본 이미지 설정 정보 가져오기
    struct ImageFileConfig {
        std::string path;
        std::string fileName;
        std::string filters;
        cv::Mat imageData;
    };

    static ImageFileConfig getDefaultImageConfig();

private:
    static bool isImageFile(const std::string& filePath);  // 이미지 파일 여부 확인 함수
};

#endif // FILE_MANAGER_HPP
