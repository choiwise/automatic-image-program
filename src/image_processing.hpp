#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class ImageProcessor {
public:
    // 이미지 처리 함수들
    static cv::Mat adjustBrightnessContrast(const cv::Mat& image, double alpha, int beta);
    static cv::Mat adjustCurves(const cv::Mat& image, const std::vector<int>& curve);
    static cv::Mat resizeImage(const cv::Mat& image, int width, int height);
    static cv::Mat cropImage(const cv::Mat& image, int x, int y, int width, int height);
    static cv::Mat adjustHueSaturation(const cv::Mat& image, int hueShift, int saturationScale);
    static cv::Mat adjustVibrance(const cv::Mat& image, int vibranceLevel);

    // 메인 이미지 처리 함수
    static cv::Mat processImage(const cv::Mat& image);
};

#endif // IMAGE_PROCESSING_HPP
