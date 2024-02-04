
add_library(BoostTE INTERFACE)
target_include_directories(BoostTE INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
