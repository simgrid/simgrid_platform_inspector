
cmake_minimum_required(VERSION 3.2)
message(STATUS "Cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

project(platform-creator)

# Disable annoying warnings
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")

add_definitions("-Wall -Wno-unused-variable -Wno-unused-private-field")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# Find SimGrid, and Boost
find_package(SimGrid REQUIRED)
find_package(Boost REQUIRED)

# include directories
include_directories(include/ /usr/local/include/ /opt/local/include/ ${SimGrid_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

# source files
set(SOURCE_FILES
        src/main.cpp
        )

# generating the executable
add_executable(example_simgrid_platform_creator ${SOURCE_FILES})

target_link_libraries(example_simgrid_platform_creator
        ${SimGrid_LIBRARY}
        ${Boost_LIBRARIES}
        )

install(TARGETS example_simgrid_platform_creator DESTINATION bin)
