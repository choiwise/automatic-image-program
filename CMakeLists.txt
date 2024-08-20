cmake_minimum_required(VERSION 3.14)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 17)

# Vulkan SDK 경로 설정 
set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.3.283.0/Include")
set(Vulkan_LIB_DIR "C:/VulkanSDK/1.3.283.0/Lib")

include(FetchContent)

# OpenCV 설치 경로 설정
set(OpenCV_DIR "C:/Users/xzxz3/source/repos/vcpkg/packages/opencv_x64-windows/share/opencv")  # OpenCV 설치 경로


# 외부 라이브러리 다운로드 설정
# GLFW 설정 (Doxygen 비활성화)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)

FetchContent_MakeAvailable(imgui glfw)

# Vulkan 라이브러리 찾기
find_package(Vulkan REQUIRED)

# 소스 파일 설정
set(SOURCES
    src/main.cpp
    src/vulkan_render.cpp 
    src/vulkan_render.hpp
    src/imgui_interface.cpp 
    src/imgui_interface.hpp
    src/image_processing.cpp    
    src/image_processing.hpp
    src/layer_manager.cpp  
    src/layer_manager.hpp
    src/file_manager.cpp  
    src/file_manager.hpp
    src/undo_redo_manager.cpp     
    src/undo_redo_manager.hpp
)

# include 디렉토리 설정
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${Vulkan_INCLUDE_DIR})

# OpenCV 포함 디렉토리 설정
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${imgui_SOURCE_DIR})
include_directories(${glfw_SOURCE_DIR}/include)
# 실행 파일 생성
add_executable(MyImageProcessor ${SOURCES})

# 라이브러리 링크 설정
target_link_libraries(MyImageProcessor Vulkan::Vulkan ${OpenCV_LIBS} glfw imgui)
