#ifndef INCLUDE_SRC_COMMAND_H_
#define INCLUDE_SRC_COMMAND_H_

typedef struct {
  char *name;
  char **args;
} command;

extern command *shr_new_command(char *name, char **args, int num_args);
extern void shr_free_command(command *cmd);

#endif // INCLUDE_SRC_COMMAND_H_
