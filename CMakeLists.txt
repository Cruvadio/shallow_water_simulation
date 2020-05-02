cmake_minimum_required(VERSION 3.5)
project(main)

add_subdirectory (external)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    include/common.h
    src/glad.c
    src/main.cpp
    include/ShaderProgram.h
    src/ShaderProgram.cpp
    src/WaterShaderProgram.cpp
    src/Skybox.cpp
    src/Pool.cpp
    src/common.cpp
    src/Sphere.cpp
    )

set(ADDITIONAL_INCLUDE_DIRS
        dependencies/include/GLAD
        external/glm-0.9.7.1/
        external/
        include/
        )
set(ADDITIONAL_LIBRARY_DIRS
        dependencies/lib)   
set(ADDITIONAL_RUNTIME_LIBRARY_DIRS
        dependencies/bin)   

set (CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG}")

if(WIN32)
  set(ADDITIONAL_INCLUDE_DIRS 
        ${ADDITIONAL_INCLUDE_DIRS}
        dependencies/include
      )
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
else()
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
endif()

include_directories(${ADDITIONAL_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

add_executable(shallow_water ${SOURCE_FILES})

target_include_directories(shallow_water PRIVATE ${OPENGL_INCLUDE_DIR})
add_custom_command(TARGET shallow_water POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" "${PROJECT_BINARY_DIR}/shaders")
add_custom_command(TARGET shallow_water POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/textures" "${PROJECT_BINARY_DIR}/textures")
add_custom_command(TARGET shallow_water POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/models" "${PROJECT_BINARY_DIR}/models")

if(WIN32)
  add_custom_command(TARGET shallow_water POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/dependencies/bin" $<TARGET_FILE_DIR:main>)
  #set(CMAKE_MSVCIDE_RUN_PATH ${ADDITIONAL_RUNTIME_LIBRARY_DIRS})
  target_compile_options(shallow_water PRIVATE)
  target_link_libraries(shallow_water LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw3dll)
else()
  target_compile_options(shallow_water PRIVATE -Wnarrowing)
  target_link_libraries(shallow_water LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw rt dl)
endif()

