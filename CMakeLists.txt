cmake_minimum_required(VERSION 3.20)
project(Shard)

if(NOT CMAKE_BUILD_TYPE)
    set(default_build_type "Debug")
    message(STATUS "No build type specified. Defaulting to '${default_build_type}'")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Shard Build Type: ${CMAKE_BUILD_TYPE}")

set(Shard_Version_Major 0)
set(Shard_Version_Minor 1)
set(Shard_Version_Patch 0)
set(Shard_Version_Status "-dev")

set(PROJECT_VERSION
    "${Shard_Version_Major}.${Shard_Version_Minor}.${Shard_Version_Patch}${Shard_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

file(GLOB_RECURSE SOURCES "src/*.c")

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

    message(STATUS "Found Readline: ${READLINE_LIB}")
    message(STATUS "Readline includes: ${READLINE_INCLUDE_DIR}")
else()
    message(STATUS "Readline was not linked. To enable, pass -DUSE_READLINE=ON to CMake.")
endif()

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

add_library(shr_static STATIC ${SOURCES})
set_target_properties(shr_static PROPERTIES
    OUTPUT_NAME "shr"
    ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
)

add_library(shr_shared SHARED ${SOURCES})
set_target_properties(shr_shared PROPERTIES
    OUTPUT_NAME "shr"
    LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
)

target_include_directories(shr_static
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(shr_shared
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(shr_static PRIVATE ${PROJECT_BINARY_DIR}/generated)
target_include_directories(shr_shared PRIVATE ${PROJECT_BINARY_DIR}/generated)

if(USE_READLINE)
    target_link_libraries(shr_static PRIVATE ${READLINE_LIB})
    target_link_libraries(shr_shared PRIVATE ${READLINE_LIB})

    target_include_directories(shr_static PRIVATE ${READLINE_INCLUDE_DIR})
    target_include_directories(shr_shared PRIVATE ${READLINE_INCLUDE_DIR})
endif()

message(STATUS "Static library output: ${LIBRARY_OUTPUT_PATH}/libshr.a")
message(STATUS "Shared library output: ${LIBRARY_OUTPUT_PATH}/libshr.so")

add_executable(shr ${SOURCES})
target_include_directories(shr
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(shr PRIVATE shr_static)  
