cmake_minimum_required(VERSION 3.15)
project(CampusFiber)

set(CMAKE_CXX_STANDARD 17)

include_directories(runtime)

# --- 1. 下载 SFML ---
include(FetchContent)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.x
)
FetchContent_MakeAvailable(sfml)

# --- 2. 添加源文件 (核心步骤) ---
# 注意：这里定义了程序的名字叫 CampusFiber
add_executable(CampusFiber
        main.cpp
        DataStructure.cpp    # 确保你的文件叫这个名字
        Visualizer.cpp       # 确保你的文件叫这个名字
        GraphSystem.cpp
        runtime/DataStructure.h
        runtime/Visualizer.h
        runtime/GraphSystem.h
)

# --- 3. 链接库 ---

target_link_libraries(CampusFiber PRIVATE sfml-graphics sfml-window sfml-system)

# --- 4. 复制字体文件 ---
file(COPY ${CMAKE_SOURCE_DIR}/font.ttf DESTINATION ${CMAKE_BINARY_DIR})