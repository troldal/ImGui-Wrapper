cmake_minimum_required(VERSION 3.26)

project(ImGuiWrapper.Main
    VERSION 0.0.1
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)
option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)

add_subdirectory(imw)
add_subdirectory(demo)