cmake_minimum_required(VERSION 3.16)

project(CampusFiber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 未指定构建类型时默认为 Release，避免单配置生成器（Makefile/Ninja）下无优化
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---------------------------------------------------------------------------
# 依赖：SFML 2.6.1
#   1) 若存在 libs/SFML-2.6.1（本机开发用的完整源码树），直接使用，无需联网
#   2) 否则用 FetchContent 自动下载，使 clone 后可直接构建
#
#   国内网络下载慢或失败时，可换成镜像：
#     cmake -B build -DSFML_GIT_REPOSITORY=https://gitee.com/mirrors/SFML.git
# ---------------------------------------------------------------------------
set(SFML_GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    CACHE STRING "获取 SFML 源码所用的 git 仓库地址")
set(SFML_GIT_TAG "2.6.1" CACHE STRING "SFML 版本标签")

set(SFML_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML-2.6.1")

# 只构建链接用到的 graphics/window/system，跳过 audio/network/examples/tests/doc。
# 两个分支都设置，本地源码树同样受益（少编译大量无关模块）。
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)

if(EXISTS "${SFML_LOCAL_DIR}/CMakeLists.txt")
    message(STATUS "使用本地 SFML: ${SFML_LOCAL_DIR}")
    add_subdirectory("${SFML_LOCAL_DIR}" "${CMAKE_BINARY_DIR}/sfml")
else()
    message(STATUS "未找到本地 SFML，将从 ${SFML_GIT_REPOSITORY} 下载 ${SFML_GIT_TAG}")
    include(FetchContent)

    FetchContent_Declare(sfml
            GIT_REPOSITORY "${SFML_GIT_REPOSITORY}"
            GIT_TAG        "${SFML_GIT_TAG}"
            GIT_SHALLOW    ON)
    FetchContent_MakeAvailable(sfml)
endif()

# ---------------------------------------------------------------------------
# 目标程序
# ---------------------------------------------------------------------------
set(PROJECT_SOURCES
        main.cpp
        DataStructure.cpp
        Visualizer.cpp
        GraphSystem.cpp)

set(PROJECT_HEADERS
        runtime/DataStructure.h
        runtime/Visualizer.h
        runtime/GraphSystem.h)

add_executable(CampusFiber ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(CampusFiber PRIVATE runtime)

target_link_libraries(CampusFiber PRIVATE sfml-graphics sfml-window sfml-system)

# ---------------------------------------------------------------------------
# MinGW 兼容性修补
#   SFML 2.6.1 自带的 extlibs/libfreetype.a 是旧版 MinGW 编译的，其中引用了
#   `_setjmp`。而 MinGW-w64 GCC 13 之后不再导出该符号（改为 __intrinsic_setjmp
#   等内部名字），于是链接时报 undefined reference to `_setjmp`。
#   这里用链接器别名把 `_setjmp` 指向仍然存在的 `setjmp`，语义等价。
#   仅在确实存在该符号时才添加，避免影响其他平台。
# ---------------------------------------------------------------------------
if(MINGW)
    include(CheckCSourceCompiles)
    set(CMAKE_REQUIRED_LIBRARIES msvcrt)
    check_c_source_compiles("
        extern int setjmp(void*);
        int main(void) { return (int)(long)&setjmp; }
    " MINGW_HAS_SETJMP)
    unset(CMAKE_REQUIRED_LIBRARIES)

    if(MINGW_HAS_SETJMP)
        set(SFML_SETJMP_ALIAS "-Wl,--defsym=_setjmp=setjmp")

        # 最终可执行文件：静态 SFML 时 freetype 被链进 exe，需要在这里补符号
        target_link_options(CampusFiber PRIVATE "${SFML_SETJMP_ALIAS}")

        # 动态 SFML 时 freetype 被链进 sfml-graphics 这个 DLL，
        # 补符号必须加在产生该 DLL 的那次链接上，否则容器库先一步链接失败。
        foreach(_sfml_target sfml-graphics sfml-window sfml-system)
            if(TARGET ${_sfml_target})
                get_target_property(_sfml_type ${_sfml_target} TYPE)
                if(_sfml_type STREQUAL "SHARED_LIBRARY")
                    target_link_options(${_sfml_target} PRIVATE "${SFML_SETJMP_ALIAS}")
                endif()
            endif()
        endforeach()
    endif()
endif()

# ---------------------------------------------------------------------------
# 运行时资源：字体和数据文件需要和可执行文件放在同一目录
# ---------------------------------------------------------------------------
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/font.ttf" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data.txt" DESTINATION "${CMAKE_BINARY_DIR}")

# Windows 下若 SFML 以动态库形式构建，需把 DLL 复制到可执行文件旁，否则无法启动
if(WIN32)
    foreach(_sfml_target sfml-graphics sfml-window sfml-system)
        get_target_property(_sfml_type ${_sfml_target} TYPE)
        if(_sfml_type STREQUAL "SHARED_LIBRARY")
            add_custom_command(TARGET CampusFiber POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            $<TARGET_FILE:${_sfml_target}>
                            $<TARGET_FILE_DIR:CampusFiber>
                    COMMENT "复制 ${_sfml_target} 运行库")
        endif()
    endforeach()
endif()
