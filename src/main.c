#include <stddef.h>
#include <stdio.h>
// clang-format off
#include <readline/history.h>
#include <readline/readline.h>
// clang-format on
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "version.h"

#define SHR_INPUT_DEFAULT 1
#define SHR_OUTPUT_DEFAULT 1
#define SHR_GETENV_DEFAULT 1
#include <shard.h>

int main(int argc, char *argv[]) {
  shr_output("Welcome to Shard!\n");
  shr_output("Version: " SHARD_VERSION_SHORT "\n\n");

  while (1) {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    char *user;
    if (getenv("USER")) {
      user = shr_getenv("USER");
    } else {
      user = "guest";
    }

    shr_output("\n" BOLD_BLUE "%s" COLOR_RESET " at " GREEN "%s" COLOR_RESET
               "\n",
               user, cwd);

    char *prompt = BOLD_CYAN ">> " COLOR_RESET;
    char *input = shr_input(prompt);
    if (!input) {
      break;
    }

    if (strlen(input) == 0) {
      free(input);
      continue;
    }

    add_history(input);

    int num_tokens = 0;
    token *tokens = shr_tokenize(input, &num_tokens);

    if (num_tokens == 0) {
      shr_free_tokens(tokens, num_tokens);
      free(input);
      continue;
    }

    char *cmd_name = tokens[0].lexeme;

    char **args = malloc((num_tokens) * sizeof(char *));
    if (!args) {
      perror("malloc");
      free(input);
      shr_free_tokens(tokens, num_tokens);
      continue;
    }

    for (size_t i = 0; i < num_tokens; i++) {
      args[i] = tokens[i].lexeme;
      IMPORTANT("Arg %lu for '%s': %s", i, cmd_name, args[i]);
    }

    command *cmd = shr_new_command(cmd_name, args, num_tokens - 1);
    process *proc = shr_new_proc(cmd);

    INFO("Created process %d", proc->pid);
    shr_exec_proc(proc);
    INFO("Process ended with status %d", proc->status);

    shr_free_proc(proc);
    shr_free_tokens(tokens, num_tokens);
    free(args);
    free(input);
  }

  return EXIT_SUCCESS;
}
