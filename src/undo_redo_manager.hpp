#ifndef UNDO_REDO_MANAGER_HPP
#define UNDO_REDO_MANAGER_HPP

#include <stack>
#include <opencv2/opencv.hpp>

class UndoRedoManager {
public:
    UndoRedoManager();
    ~UndoRedoManager();

    void addState(const cv::Mat& image);
    cv::Mat undo();
    cv::Mat redo();

    bool canUndo() const;
    bool canRedo() const;

private:
    std::stack<cv::Mat> undoStack;
    std::stack<cv::Mat> redoStack;
};

#endif // UNDO_REDO_MANAGER_HPP
