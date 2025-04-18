#include <stddef.h>
#include <stdio.h>
// clang-format off
#include <readline/history.h>
#include <readline/readline.h>
// clang-format on
#include <stdlib.h>
#include <string.h>

#include <common.h>
#include <shard.h>

#include <limits.h>
#include <unistd.h>

#include "colors.h"
#include "command.h"
#include "parser/lex.h"
#include "process.h"
#include "version.h"

int main(int argc, char *argv[]) {
  printf("Welcome to Shard!\n");
  printf("Version: " SHARD_VERSION_SHORT "\n\n");

  while (1) {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    printf("\n" BOLD_BLUE "%s" COLOR_RESET " at " GREEN "%s" COLOR_RESET "\n",
           getenv("USER"), cwd);

    char *prompt = BOLD_CYAN ">> " COLOR_RESET;
    char *input = readline(prompt);
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
