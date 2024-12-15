cmake_minimum_required(VERSION 3.14)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 17)

# Vulkan SDK 경로 설정
set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.3.290.0/Include")
set(Vulkan_LIB_DIR "C:/VulkanSDK/1.3.290.0/Lib")

# OpenCV 설치 경로 설정
set(OpenCV_DIR "C:/Users/xzxz3/Downloads/opencv/build")

# OpenCV 찾기
find_package(OpenCV REQUIRED)

# Vulkan 찾기
find_package(Vulkan REQUIRED)

include(FetchContent)

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
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glfw imgui stb)

# ImGui 소스 파일 설정
set(IMGUI_IMPL_SRC
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

# Shader 파일 설정
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_BINARY_DIR}/Debug")

file(COPY ${SHADER_SOURCE_DIR} DESTINATION ${SHADER_BINARY_DIR})

# 기본 이미지 파일 설정
set(DEFAULT_IMAGE_PATH "${CMAKE_SOURCE_DIR}/img/default.png")
set(OUTPUT_IMAGE_PATH "${CMAKE_BINARY_DIR}/img")

# 빌드 폴더에 img 디렉토리 생성 및 이미지 파일 복사
file(MAKE_DIRECTORY ${OUTPUT_IMAGE_PATH})
file(COPY ${DEFAULT_IMAGE_PATH} DESTINATION ${OUTPUT_IMAGE_PATH})

# include 디렉토리 설정
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${Vulkan_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${glfw_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/stb)

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
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Od /Zi")  # 디버그 모드
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2")  # 최적화된 릴리즈 모드
endif()

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
target_link_libraries(MyImageProcessor ${Vulkan_LIBRARIES} ${OpenCV_LIBS} glfw imgui)
target_include_directories(MyImageProcessor PRIVATE ${stb_SOURCE_DIR})

# OpenCV DLL 파일 복사 (윈도우 환경)
if (WIN32)
    add_custom_command(TARGET MyImageProcessor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OpenCV_DIR}/x64/vc16/bin"  # 수정된 dll 경로 확인
        $<TARGET_FILE_DIR:MyImageProcessor>)
endif()
