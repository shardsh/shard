cmake_minimum_required(VERSION 3.20)
project(Shard)

if(NOT CMAKE_BUILD_TYPE)
	set(default_build_type "Debug")
	message(STATUS "Set the build type to `${default_build_type}` as none was specified.")
	set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "Choose the build type." FORCE)
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

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(
	shard
	${SOURCES}
)

target_include_directories(
	shard
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	PRIVATE ${PROJECT_SOURCE_DIR}/src
)

find_library(READLINE_LIB readline)
if(NOT READLINE_LIB)
	message(FATAL_ERROR "Readline library not found")
endif()

find_path(READLINE_INCLUDE_DIR readline/readline.h)
if(NOT READLINE_INCLUDE_DIR)
	message(FATAL_ERROR "Readline headers not found")
endif()

target_link_libraries(shard PRIVATE ${READLINE_LIB})
target_include_directories(shard PRIVATE ${READLINE_INCLUDE_DIR})

message(STATUS "Found Readline: ${READLINE_LIB}")
message(STATUS "Readline includes: ${READLINE_INCLUDE_DIR}")

configure_file(
  ${PROJECT_SOURCE_DIR}/include/version.h.in
  ${PROJECT_BINARY_DIR}/generated/version.h
  @ONLY
)

target_include_directories(shard PRIVATE ${PROJECT_BINARY_DIR}/generated)


