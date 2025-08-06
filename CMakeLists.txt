cmake_minimum_required(VERSION 3.31)
project(SimpleGl_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimpleGl_2 main.cpp)

set(GlInclude "include/") # GLFW,GLEW include folder (include/GLEW/,include/GLFW/)
include_directories(${GlInclude})
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/${GlInclude}/GLFW/glfw3.lib ${CMAKE_SOURCE_DIR}/${GlInclude}/GLEW/glew32s.lib OpenGL::GL)