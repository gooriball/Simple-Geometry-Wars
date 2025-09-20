cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Geometry_Wars)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "Geometry Wars")
set(WINDOW_WIDTH 1440)
set(WINDOW_HEIGHT 900)

project(${PROJECT_NAME})


set(SOURCES
	src/main.cpp
	src/ConfigLoader.cpp
	src/EntityManager.cpp
	src/Game.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})


set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Build shared libraries" FORCE)

include(FetchContent)

FetchContent_Declare(
	SPDLOG
	GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
	GIT_TAG "v1.15.1"
)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "Build shared library" FORCE)
FetchContent_MakeAvailable(SPDLOG)


FetchContent_Declare(
	SFML
	GIT_REPOSITORY "https://github.com/SFML/SFML.git"
	GIT_TAG "2.6.1"
)
set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build SFML examples" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "Build SFML documentation" FORCE)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML Audio module" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML Network module" FORCE)
FetchContent_MakeAvailable(SFML)


FetchContent_Declare(
	IMGUI
	GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
	GIT_TAG "v1.91.8"
)
FetchContent_MakeAvailable(IMGUI)


set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(SFML_DIR ${sfml_BINARY_DIR})


FetchContent_Declare(
	IMGUI_SFML
	GIT_REPOSITORY "https://github.com/SFML/imgui-sfml.git"
	GIT_TAG "v2.6.1"
)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build ImGui-SFML examples" FORCE)
set(IMGUI_SFML_IMGUI_DEMO OFF CACHE BOOL "Build ImGui demo" FORCE)
FetchContent_MakeAvailable(IMGUI_SFML)

FetchContent_Declare(
	JSON
	GIT_REPOSITORY "https://github.com/nlohmann/json.git"
	GIT_TAG "v3.11.3"
)
set(JSON_BuildTests OFF CACHE BOOL "Build JSON tests" FORCE)
set(JSON_HeaderOnly OFF CACHE BOOL "Use header-only version" FORCE)
FetchContent_MakeAvailable(JSON)

set(LIBRARIES
	sfml-graphics
	sfml-window
	sfml-system
	ImGui-SFML
	spdlog::spdlog
	nlohmann_json::nlohmann_json
)


set(IMGUI_SOURCE_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_SOURCE_DIR ${imgui_sfml_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${IMGUI_SOURCE_DIR}
	${IMGUI_SFML_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
	WINDOW_NAME="${WINDOW_NAME}"
	WINDOW_WIDTH="${WINDOW_WIDTH}"
	WINDOW_HEIGHT="${WINDOW_HEIGHT}"
)