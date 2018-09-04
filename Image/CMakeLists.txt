
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

set(EXECUTABLE_NAME "ImageBuilder")
project(${EXECUTABLE_NAME})

add_definitions ( -DEXEC_NAME="${EXECUTABLE_NAME}" )

file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SOURCES_CPP "src/*.cpp")
file(GLOB_RECURSE SOURCES_C "src/*.c")

set( SOURCES ${SOURCES_CPP} ${SOURCES_C} )
add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})

if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME})
endif()

if(UNIX AND NOT APPLE)
    set(LIBS ${LIBS} ${CMAKE_DL_LIBS})
endif()

if(APPLE)
    find_library(COREFOUNDATION CoreFoundation)
    set(LIBS ${LIBS} ${COREFOUNDATION})
endif()

target_link_libraries(${EXECUTABLE_NAME} ${LIBS})


