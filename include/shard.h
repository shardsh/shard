#ifndef INCLUDE_SHARD_H_
#define INCLUDE_SHARD_H_

#include <shard/colors.h>
#include <shard/command.h>
#include <shard/common.h>
#include <shard/lex.h>
#include <shard/process.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef SHR_INPUT_DEFAULT
#include <readline/readline.h>
static inline char *shr_input(const char *prompt) { return readline(prompt); }
#else
char *shr_input(const char *prompt);
#endif

#ifdef SHR_OUTPUT_DEFAULT
static inline void shr_output(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
#else
void shr_output(const char *fmt, ...);
#endif

#ifdef SHR_GETENV_DEFAULT
static inline char *shr_getenv(const char *name) { return getenv(name); }
#else
char *shr_getenv(const char *name);
#endif

#endif // INCLUDE_SHARD_H_
