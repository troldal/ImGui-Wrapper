

add_library(implot STATIC implot.cpp implot_items.cpp implot_demo.cpp)
target_include_directories(implot PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_link_libraries(implot PUBLIC imgui)
add_library(implot::implot ALIAS implot)
