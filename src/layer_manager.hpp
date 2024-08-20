#ifndef LAYER_MANAGER_HPP
#define LAYER_MANAGER_HPP

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

class LayerManager {
public:
    LayerManager();
    ~LayerManager();

    void addLayer(const std::string& name, const cv::Mat& image);
    void removeLayer(int index);
    cv::Mat getMergedImage();

    void setLayerVisibility(int index, bool visible);

private:
    struct Layer {
        std::string name;
        cv::Mat image;
        bool visible;
    };

    std::vector<Layer> layers;
};

#endif // LAYER_MANAGER_HPP
