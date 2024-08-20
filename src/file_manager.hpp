#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <vector>

class FileManager {
public:
    FileManager();
    ~FileManager();

    std::vector<std::string> listImageFiles(const std::string& directory);
    std::string getOutputDirectory();

    void setInputDirectory(const std::string& directory);
    void setOutputDirectory(const std::string& directory);

private:
    std::string inputDirectory;
    std::string outputDirectory;
};

#endif // FILE_MANAGER_HPP
