
add_library(imgui STATIC imgui.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
    imgui_demo.cpp)
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/backends>)
target_link_libraries(imgui PUBLIC OpenGL::GL)
target_link_libraries(imgui PUBLIC glfw)
add_library(imgui::imgui ALIAS imgui)


