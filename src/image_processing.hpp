#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessing {
public:
    ImageProcessing();
    ~ImageProcessing();

    cv::Mat loadImage(const std::string& filePath);
    void saveImage(const std::string& filePath, const cv::Mat& image);

    cv::Mat applyScale(const cv::Mat& image, float scaleFactor);
    cv::Mat applyCrop(const cv::Mat& image, int x, int y, int width, int height);

private:
    // 필요한 멤버 변수들...
};

#endif // IMAGE_PROCESSING_HPP
