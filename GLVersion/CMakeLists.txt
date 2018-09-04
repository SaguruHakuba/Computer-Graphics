
if(WIN32)
    cmake_minimum_required(VERSION 3.6) #for VS options
else()
    cmake_minimum_required(VERSION 2.6)
endif()


# Enable debug symbols by default, must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

set(EXECUTABLE_NAME "GLVersion")
project(${EXECUTABLE_NAME})

set( SOURCEFILES "src/main.cpp" "src/gl3w/gl3w.c" )
include_directories("${PROJECT_BINARY_DIR}")
add_executable(${EXECUTABLE_NAME} ${SOURCEFILES} )


find_package( OpenGL REQUIRED )
set(LIBS ${OPENGL_gl_LIBRARY})

if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME})
    set(SFML_ROOT "C:/dev/SFML-2.5.0")
endif()

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  set(LIBS ${LIBS} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

if(WIN32)
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${SFML_INCLUDE_DIR}/../bin"
        "${PROJECT_BINARY_DIR}" )
endif()

if(UNIX AND NOT APPLE)
    set(LIBS ${LIBS} ${CMAKE_DL_LIBS})
endif()

if(APPLE)
    find_library(COREFOUNDATION CoreFoundation)
    set(LIBS ${LIBS} ${COREFOUNDATION})
endif()

target_link_libraries(${EXECUTABLE_NAME} ${LIBS})


