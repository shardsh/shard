#ifndef INCLUDE_SRC_TYPES_H_
#define INCLUDE_SRC_TYPES_H_

// util
#define UNUSED(x) ((void)(x))
#define PASS                                                                   \
  do {                                                                         \
  } while (0)

// array
#define ARRAY_SIZE(arr) (sizeof(arr) / (sizeof((arr)[0])))

// int types
typedef unsigned char u8;
typedef signed char i8;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned int u32;
typedef signed int i32;
typedef unsigned long long u64;
typedef signed long long i64;

// bool
#define bool int
#define true 1
#define false 0
#define SUCCESS 0;
#define FAILURE 1;

// debug
#define DEBUG 1
#define DEBUG_LEVEL 3
#ifdef DEBUG
#define LOG(type, level, msg, ...)                                             \
  if ((level) > DEBUG_LEVEL)                                                   \
  fprintf(stderr, "[" type "] " msg "\n", __VA_ARGS__)
#else
#define LOG(type, msg, ...)                                                    \
  do {                                                                         \
  } while (0)
#endif

#include "colors.h"

#define FATAL(msg, ...) LOG(BOLD_RED "FATAL" COLOR_RESET, 6, msg, __VA_ARGS__)
#define ERROR(msg, ...) LOG(RED "ERROR" COLOR_RESET, 5, msg, __VA_ARGS__)
#define IMPORTANT(msg, ...) LOG(YELLOW "!!!" COLOR_RESET, 4, msg, __VA_ARGS__)
#define INFO(msg, ...) LOG(GREEN "INFO" COLOR_RESET, 3, msg, __VA_ARGS__)
#define TRACE(msg, ...) LOG(BLUE "TRACE" COLOR_RESET, 2, msg, __VA_ARGS__)
#define MEMLOG(msg, ...) LOG(PURPLE "MEM" COLOR_RESET, 1, msg, __VA_ARGS__)

#endif // INCLUDE_SRC_TYPES_H_
