cmake_minimum_required(VERSION 3.10)
project(hex_collapse_plugin)

# Get development version of salamesh directly from git repository 
include(FetchContent)
FetchContent_Declare(
  salamesh
  GIT_REPOSITORY https://github.com/lbinria/salamesh.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(salamesh)

# Add salamesh directory as include
include_directories(${salamesh_SOURCE_DIR}/core)
# Link to salamesh core
link_libraries(salamesh_core)

include_directories(${salamesh_SOURCE_DIR}/include/imgui)
# target_include_directories(salamesh PRIVATE include/imgui)


set(IMGUI_SOURCES
  ${salamesh_SOURCE_DIR}/include/imgui/imgui.cpp
  ${salamesh_SOURCE_DIR}/include/imgui/imgui.h

  ${salamesh_SOURCE_DIR}/include/imgui/imgui_demo.cpp
  ${salamesh_SOURCE_DIR}/include/imgui/imgui_draw.cpp
  ${salamesh_SOURCE_DIR}/include/imgui/imgui_widgets.cpp
  ${salamesh_SOURCE_DIR}/include/imgui/imgui_tables.cpp

  ${salamesh_SOURCE_DIR}/include/imgui/backends/imgui_impl_glfw.h
  ${salamesh_SOURCE_DIR}/include/imgui/backends/imgui_impl_glfw.cpp
  ${salamesh_SOURCE_DIR}/include/imgui/backends/imgui_impl_opengl3.h
  ${salamesh_SOURCE_DIR}/include/imgui/backends/imgui_impl_opengl3.cpp
  ${salamesh_SOURCE_DIR}/include/imgui/backends/imgui_impl_opengl3_loader.h
  # ${salamesh_SOURCE_DIR}/include/imgui/imgui.cpp
  # ${salamesh_SOURCE_DIR}/include/imgui/imgui_draw.cpp
  # ${salamesh_SOURCE_DIR}/include/imgui/imgui_widgets.cpp
  # ${salamesh_SOURCE_DIR}/include/imgui/imgui_demo.cpp
)

add_library(hex_collapse_plugin SHARED
  hex_collapse.cpp hex_collapse.h
  interface.cpp
  ${IMGUI_SOURCES}
)