cmake_minimum_required(VERSION 3.14)
project(CiemnyLabirynt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opcjonalnie: kontrola wersji OpenGL (dla macOS można wymusić 3.3 core)
set(OPENGL_REQUIRED_VERSION 3.3)

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# GLAD (loader)
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v0.1.34
)
FetchContent_MakeAvailable(glad)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Executable
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.c src/*.cc)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# jeśli GLAD daje target glad::glad, użyj go; GLFW daje glfw
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm)

# Platform-specific: link OpenGL
if (APPLE)
  find_library(OPENGL_LIBRARY OpenGL)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARY})
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()

# Copy shaders/assets to build dir (ułatwia uruchamianie)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
