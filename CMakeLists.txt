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

# Include ImGui directory
include_directories(${salamesh_SOURCE_DIR}/include/imgui)


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
)

# ULTIMAILLE

# Get development version of ultimaille directly from git repository 
include(FetchContent)
FetchContent_Declare(
  ultimaille
  GIT_REPOSITORY https://github.com/ssloy/ultimaille
  GIT_TAG        v2.0.0
)
FetchContent_MakeAvailable(ultimaille)

# Add ultimaille directory as include
include_directories(${ultimaille_SOURCE_DIR})
link_libraries(${CMAKE_DL_LIBS} ultimaille m $<$<BOOL:${OpenMP_CXX_FOUND}>:OpenMP::OpenMP_CXX>)

add_library(hex_collapse_plugin SHARED
  hex_collapse.cpp hex_collapse.h
  interface.cpp
  ${IMGUI_SOURCES}
)

# Generate JSON settings file after build
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/settings.json "
{
	\"modules\": [
		\"${CMAKE_CURRENT_BINARY_DIR}\"
	]
}
")

# Get the build directory of the fetched content
get_property(salamesh_build_dir DIRECTORY ${salamesh_SOURCE_DIR} PROPERTY BINARY_DIR)
message(STATUS "Salamesh build directory: ${salamesh_build_dir}")

# Add custom target to build plugin & run salamesh with the plugin
add_custom_target(run_salamesh
  # COMMAND ${salamesh_build_dir}/salamesh
  COMMAND ${salamesh_build_dir}/salamesh '{"settings_path": \"${CMAKE_CURRENT_BINARY_DIR}/settings.json\"}'
  DEPENDS hex_collapse_plugin salamesh
  COMMENT "Running Salamesh with current plugin"
)