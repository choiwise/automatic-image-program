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
     static void adjustBrightness(cv::Mat& image, float brightness);
    static void adjustContrast(cv::Mat& image, float contrast);
    static void adjustSaturation(cv::Mat& image, float saturation);
    static void adjustGamma(cv::Mat& image, float gamma);
    static cv::Mat applyChanges(const cv::Mat& image, float brightness, float contrast, float saturation, float gamma);
    // 메인 이미지 처리 함수
    static cv::Mat processImage(const cv::Mat& image, double brightnessAlpha, int brightnessBeta, 
                            const std::vector<int>& curve, int hueShift, int saturationScale,
                            int vibranceLevel, int resizeWidth, int resizeHeight);

};

#endif // IMAGE_PROCESSING_HPP
