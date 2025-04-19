cmake_minimum_required(VERSION 3.20)
project(Shard)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(default_build_type "Debug")
    message(STATUS "No build type specified. Defaulting to '${default_build_type}'")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Shard Build Type: ${CMAKE_BUILD_TYPE}")

# Set the version for Shard
set(Shard_Version_Major 0)
set(Shard_Version_Minor 1)
set(Shard_Version_Patch 0)
set(Shard_Version_Status "-dev")

set(PROJECT_VERSION
    "${Shard_Version_Major}.${Shard_Version_Minor}.${Shard_Version_Patch}${Shard_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Define the executable
add_executable(shr ${SOURCES})

# Include directories
target_include_directories(shr
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

# Option for using readline
option(USE_READLINE "Find and link readline" OFF)

if(USE_READLINE)
    find_library(READLINE_LIB readline)

    if(NOT READLINE_LIB)
        message(FATAL_ERROR "Readline library not found")
    endif()

    find_path(READLINE_INCLUDE_DIR readline/readline.h)

    if(NOT READLINE_INCLUDE_DIR)
        message(FATAL_ERROR "Readline headers not found")
    endif()

    target_link_libraries(shr PRIVATE ${READLINE_LIB})
    target_include_directories(shr PRIVATE ${READLINE_INCLUDE_DIR})

    message(STATUS "Found Readline: ${READLINE_LIB}")
    message(STATUS "Readline includes: ${READLINE_INCLUDE_DIR}")
else()
    message(STATUS "Readline was not linked. To enable, pass -DUSE_READLINE=ON to CMake.")
endif()

# Generate version header
configure_file(
    ${PROJECT_SOURCE_DIR}/include/version.h.in
    ${PROJECT_BINARY_DIR}/generated/version.h
    @ONLY
)

target_include_directories(shr PRIVATE ${PROJECT_BINARY_DIR}/generated)


