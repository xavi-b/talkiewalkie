cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME talkiewalkie)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

# C++
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_CXX_FLAGS                 "-Wall")
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wpedantic")
endif()

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
    add_definitions(-DDEBUG)
else()
    message("RELEASE")
endif()

# deployqt
include("${CMAKE_CURRENT_SOURCE_DIR}/deployqt.cmake/deployqt.cmake")

# subdirectories
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/client)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/server)
