#ifndef INCLUDE_SRC_TYPES_H_
#define INCLUDE_SRC_TYPES_H_

// util
#define UNUSED(x) ((void)(x))

// array
#define ARRAY_SIZE(arr) (sizeof(arr) / (sizeof((arr)[0])))
#define FOREACH(arr) for (size_t i = 0; i < ARRAY_SIZE(arr); i++)

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
typedef enum { false = 0, true = 1 } bool;
#define SUCCESS return false;
#define FAILURE return true;

// debug
#define DEBUG 1
#ifdef DEBUG
#define LOG(type, msg, ...) printf("[" type "] " msg "\n", ##__VA_ARGS__)
#else
#define LOG(type, msg, ...)                                                    \
  do {                                                                         \
  } while (0)
#endif

#define INFO(msg, ...) LOG("INFO", msg, __VA_ARGS__)
#define ERROR(msg, ...) LOG("ERROR", msg, __VA_ARGS__)
#define TRACE(msg, ...) LOG("TRACE", msg, __VA_ARGS__)

#endif // INCLUDE_SRC_TYPES_H_
