cmake_minimum_required(VERSION 3.26)

project(ImGuiWrapper.Library
        VERSION 0.0.1
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(external)

add_library(ImGuiWrapper INTERFACE)
target_include_directories(ImGuiWrapper INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
add_library(imw::imw ALIAS ImGuiWrapper)

target_link_libraries(ImGuiWrapper INTERFACE fmt::fmt-header-only)
target_link_libraries(ImGuiWrapper INTERFACE glfw)
target_link_libraries(ImGuiWrapper INTERFACE OpenGL::GL)
target_link_libraries(ImGuiWrapper INTERFACE imgui::imgui)
target_link_libraries(ImGuiWrapper INTERFACE implot::implot)
target_link_libraries(ImGuiWrapper INTERFACE BoostTE)

