#include "process.h"
#include "command.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin/builtin.h"
#include <common.h>

typedef int (*command_func)(char *, char **);

typedef struct {
  char *name;
  command_func func;
} command_handler;

command_handler commands[] = {{"exit", shr_exit}};

int run_command(char *name, char **args) {
  for (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
    if (strcmp(commands[i].name, name)) {
      return commands[i].func(name, args);
    }
  }
  execvp(name, args);
  perror("execvp");
  _exit(1);
};

process *shr_new_proc(command *cmd) {
  INFO("Creating new process", NULL);
  process *proc = malloc(sizeof(process));
  if (!proc)
    return NULL;

  proc->cmd = cmd;

  INFO("Setting file descriptors", NULL);
  proc->fd.stdin = STDIN_FILENO;
  proc->fd.stdout = STDOUT_FILENO;
  proc->fd.stderr = STDERR_FILENO;
  INFO("File descriptors set:\nstdin: %d\nstdout: %d\nstderr: %d",
       proc->fd.stdin, proc->fd.stdout, proc->fd.stderr);

  INFO("Forking", NULL);
  proc->pid = fork();
  if (proc->pid < 0) {
    ERROR("Error when forking", NULL);
    perror("fork");
    free(proc);
    return NULL;
  }
  return proc;
}

void shr_free_proc(process *proc) {
  if (proc->cmd) {
    MEMLOG("Freeing command '%s' for proc %d", proc->cmd->name, proc->pid);
    shr_free_command(proc->cmd);
  }
  MEMLOG("Freed proc %d", proc->pid);
  free(proc);
}

int shr_setup_redirection(process *proc) {
  if (proc->fd.stdin != STDIN_FILENO)
    dup2(proc->fd.stdin, STDIN_FILENO);
  if (proc->fd.stdout != STDOUT_FILENO)
    dup2(proc->fd.stdout, STDOUT_FILENO);
  if (proc->fd.stderr != STDERR_FILENO)
    dup2(proc->fd.stderr, STDERR_FILENO);
  return 0;
}

int shr_wait_proc(process *proc) {
  return waitpid(proc->pid, &proc->status, 0);
}

bool shr_proc_exited_successfully(process *proc) {
  return WIFEXITED(proc->status) && WEXITSTATUS(proc->status) == 0;
}

void shr_close_proc_fds(process *proc) {
  if (proc->fd.stdin != STDIN_FILENO)
    close(proc->fd.stdin);
  if (proc->fd.stdout != STDOUT_FILENO)
    close(proc->fd.stdout);
  if (proc->fd.stderr != STDERR_FILENO)
    close(proc->fd.stderr);
}

int shr_exec_proc(process *proc) {
  if (proc->pid == 0) {
    shr_setup_redirection(proc);
    run_command(proc->cmd->name, proc->cmd->args);
  }
  int status = shr_wait_proc(proc);
  return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}
