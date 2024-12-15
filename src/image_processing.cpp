#include "image_processing.hpp"

// 밝기 및 명암 조정
cv::Mat ImageProcessor::adjustBrightnessContrast(const cv::Mat& image, double alpha, int beta) {
    cv::Mat result;
    image.convertTo(result, -1, alpha, beta);  // alpha는 명암 대비, beta는 밝기
    return result;
}

// 커브 조정 (포토샵의 Curve 기능과 유사)
cv::Mat ImageProcessor::adjustCurves(const cv::Mat& image, const std::vector<int>& curve) {
    cv::Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; ++i) {
        lut.at<uchar>(i) = cv::saturate_cast<uchar>(curve[i]);
    }
    cv::Mat result;
    cv::LUT(image, lut, result);
    return result;
}

// 이미지 크기 조정
cv::Mat ImageProcessor::resizeImage(const cv::Mat& image, int width, int height) {
    cv::Mat result;
    cv::resize(image, result, cv::Size(width, height));
    return result;
}

// 이미지 크롭
cv::Mat ImageProcessor::cropImage(const cv::Mat& image, int x, int y, int width, int height) {
    cv::Rect roi(x, y, width, height);
    return image(roi);
}

// 색조 및 채도 조정
cv::Mat ImageProcessor::adjustHueSaturation(const cv::Mat& image, int hueShift, int saturationScale) {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    for (int i = 0; i < hsv.rows; i++) {
        for (int j = 0; j < hsv.cols; j++) {
            hsv.at<cv::Vec3b>(i, j)[0] = (hsv.at<cv::Vec3b>(i, j)[0] + hueShift) % 180;
            hsv.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(hsv.at<cv::Vec3b>(i, j)[1] * saturationScale);
        }
    }

    cv::Mat result;
    cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR);
    return result;
}

// Vibrance 조정
cv::Mat ImageProcessor::adjustVibrance(const cv::Mat& image, int vibranceLevel) {
    cv::Mat lab;
    cv::cvtColor(image, lab, cv::COLOR_BGR2Lab);

    for (int i = 0; i < lab.rows; i++) {
        for (int j = 0; j < lab.cols; j++) {
            int l = lab.at<cv::Vec3b>(i, j)[0];
            int a = lab.at<cv::Vec3b>(i, j)[1];
            int b = lab.at<cv::Vec3b>(i, j)[2];

            if (a > 128 || b > 128) {  // 특정 색조만 Vibrance 적용
                lab.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(a + vibranceLevel);
                lab.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(b + vibranceLevel);
            }
        }
    }

    cv::Mat result;
    cv::cvtColor(lab, result, cv::COLOR_Lab2BGR);
    return result;
}

// 메인 이미지 처리 함수
cv::Mat ImageProcessor::processImage(const cv::Mat& image, double brightnessAlpha, int brightnessBeta, 
                                     const std::vector<int>& curve, int hueShift, int saturationScale,
                                     int vibranceLevel, int resizeWidth, int resizeHeight) 
{
    cv::Mat processedImage = image.clone();

    // 밝기 및 명암 조정
    processedImage = adjustBrightnessContrast(processedImage, brightnessAlpha, brightnessBeta);

    // 커브 조정
    if (!curve.empty()) {
        processedImage = adjustCurves(processedImage, curve);
    }

    // 색조 및 채도 조정
    processedImage = adjustHueSaturation(processedImage, hueShift, saturationScale);

    // Vibrance 조정
    processedImage = adjustVibrance(processedImage, vibranceLevel);

    // 이미지 크기 조정
    if (resizeWidth > 0 && resizeHeight > 0) {
        processedImage = resizeImage(processedImage, resizeWidth, resizeHeight);
    }

    return processedImage;
}

void ImageProcessor::adjustBrightness(cv::Mat& image, float brightness) {
    image.convertTo(image, -1, 1, brightness * 255);
}

void ImageProcessor::adjustContrast(cv::Mat& image, float contrast) {
    image.convertTo(image, -1, contrast, 0);
}

void ImageProcessor::adjustSaturation(cv::Mat& image, float saturation) {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    channels[1].convertTo(channels[1], -1, saturation, 0);
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, image, cv::COLOR_HSV2BGR);
}

void ImageProcessor::adjustGamma(cv::Mat& image, float gamma) {
    cv::Mat lut(1, 256, CV_8U);
    for (int i = 0; i < 256; i++) {
        lut.at<uchar>(i) = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }
    cv::LUT(image, lut, image);
}

cv::Mat ImageProcessor::applyChanges(const cv::Mat& image, float brightness, float contrast, float saturation, float gamma) {
    cv::Mat result = image.clone();
    adjustBrightness(result, brightness);
    adjustContrast(result, contrast);
    adjustSaturation(result, saturation);
    adjustGamma(result, gamma);
    return result;
}

