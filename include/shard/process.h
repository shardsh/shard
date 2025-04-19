#ifndef INCLUDE_SRC_PROCESS_H_
#define INCLUDE_SRC_PROCESS_H_

#include "command.h"
#include "shard/common.h"
#include <sys/types.h>

typedef struct {
  command *cmd;
  pid_t pid;
  struct {
    int stderr;
    int stdout;
    int stdin;
  } fd;
  int status;
} process;

extern process *shr_new_proc(command *cmd);
extern void shr_free_proc(process *proc);

extern int shr_setup_redirection(process *proc);
extern int shr_wait_proc(process *proc);
extern bool shr_proc_exited_successfully(process *proc);
extern void shr_close_proc_fds(process *proc);

extern int shr_exec_proc(process *proc);

#endif // INCLUDE_SRC_PROCESS_H_
