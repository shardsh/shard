#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include <common.h>
#include <string.h>

command *shr_new_command(char *name, char **args, int num_args) {
  INFO("Creating new command: %s", name);

  MEMLOG("Allocated for command '%s'", name);
  command *cmd = malloc(sizeof(command));
  if (!cmd)
    return NULL;

  cmd->name = strdup(name);
  if (!cmd->name) {
    free(cmd);
    return NULL;
  }

  cmd->args = malloc((num_args + 1) * sizeof(char *));
  if (!cmd->args) {
    free(cmd->name);
    free(cmd);
    return NULL;
  }

  for (int i = 0; i < num_args; i++) {
    cmd->args[i] = strdup(args[i]);
    if (!cmd->args[i]) {
      for (int j = 0; j < i; j++) {
        free(cmd->args[j]);
      }
      free(cmd->args);
      free(cmd->name);
      free(cmd);
      return NULL;
    }
  }
  cmd->args[num_args] = NULL;

  return cmd;
}

void shr_free_command(command *cmd) {
  MEMLOG("Freeing command: %s", cmd->name);

  if (cmd->name) {
    free(cmd->name);
  }

  MEMLOG("Freed name", NULL);

  if (cmd->args) {
    for (size_t i = 0; cmd->args[i] != NULL; i++) {
      MEMLOG("Freed arg: %s", cmd->args[i]);
      free(cmd->args[i]);
    }
    free(cmd->args);
  }

  MEMLOG("Freed args", NULL);
  free(cmd);
}
