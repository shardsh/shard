#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <shard.h>

int main(int argc, char *argv[]) {
  printf("Welcome to Shard!\n\n");

  const char *input =
      "var1 = 42 + 3.14 && \"Hello World\" 'SingleQuotedString'";

  int num_tokens = 0;
  token *tokens = shr_tokenize(input, &num_tokens);
  for (int i = 0; i < num_tokens; i++) {
    printf("Token: %s, Type: %s\n", tokens[i].lexeme,
           shr_token_type_to_string(tokens[i].type));
  }

  shr_free_tokens(tokens, num_tokens);
  return EXIT_SUCCESS;
}
