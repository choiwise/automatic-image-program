cmake_minimum_required(VERSION 3.14)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 17)

# Vulkan SDK 경로 설정
set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.3.283.0/Include")
set(Vulkan_LIB_DIR "C:/VulkanSDK/1.3.283.0/Lib")

include(FetchContent)

# OpenCV 설치 경로 설정
set(OpenCV_DIR "C:/Users/xzxz3/Downloads/opencv/build")
find_package(OpenCV REQUIRED)

# 외부 라이브러리 다운로드 설정
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.5
)

FetchContent_MakeAvailable(glfw imgui)

# ImGui 소스 파일 설정
set(IMGUI_IMPL_SRC
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

# include 디렉토리 설정
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${Vulkan_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${glfw_SOURCE_DIR}/include)

add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# ImGui 라이브러리 생성
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_IMPL_SRC}
)


if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

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

# 실행 파일 생성
add_executable(MyImageProcessor ${SOURCES})


# 라이브러리 링크 설정
target_link_libraries(MyImageProcessor Vulkan::Vulkan ${OpenCV_LIBS} glfw imgui)

# OpenCV DLL 파일 복사 (윈도우 환경)
if (WIN32)
    add_custom_command(TARGET MyImageProcessor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OpenCV_DIR}/bin"
        $<TARGET_FILE_DIR:MyImageProcessor>)
endif()
