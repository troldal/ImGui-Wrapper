cmake_minimum_required(VERSION 3.26)

project(ImGuiWrapper.Demo
        VERSION 0.0.1
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(NOMINMAX)

add_executable(Starter
        01-Starter/main.cpp
)
target_link_libraries(Starter PRIVATE imw::imw)


add_executable(FileExplorer
        02-FileExplorer/main.cpp
)
target_link_libraries(FileExplorer PRIVATE imw::imw)

add_executable(Plotter
        03-Plotter/main.cpp
)
target_link_libraries(Plotter PRIVATE imw::imw)

