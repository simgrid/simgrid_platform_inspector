
cmake_minimum_required(VERSION 3.2)
message(STATUS "Cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

project(simgrid-platform-explorer)

# Disable annoying warnings
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")

add_definitions("-Wall -Wno-unused-variable -Wno-unused-private-field")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# Find SimGrid, and Boost
find_package(SimGrid REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

# include directories
include_directories(/usr/local/include/ /opt/local/include/ ${SimGrid_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

# Executable
add_executable(simgrid_platform_explorer src/main.cpp)
target_link_libraries(simgrid_platform_explorer
        ${SimGrid_LIBRARY}
        ${Boost_LIBRARIES}
        )

# Custom target for the .so file
add_custom_target(shared_object_file ALL
        DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/sample_platform.so)

# Custom command for the .so file
add_custom_command(
        OUTPUT sample_platform.so
        COMMAND g++ -fPIC --std=c++17 -Wno-deprecated-builtins -I${SimGrid_INCLUDE_DIR} -I${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src/sample_platform.cpp -c -o ${CMAKE_CURRENT_BINARY_DIR}/sample_platform.o
	COMMAND g++ -fPIC -shared -fPIC ${CMAKE_CURRENT_BINARY_DIR}/sample_platform.o -o sample_platform.so ${SimGrid_LIBRARY}
        DEPENDS ${CMAKE_SOURCE_DIR}/src/sample_platform.cpp
        )

install(TARGETS simgrid_platform_explorer DESTINATION bin)
