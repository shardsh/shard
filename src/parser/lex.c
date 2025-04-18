#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"

#define MAX_TOKEN_SIZE 100
#define INITIAL_TOKEN_LIST_SIZE 10

const char *operators[] = {
    "+", "-", "*", "/", "%", "=", "==", "<", ">", "<=", ">=", "&&", "||", "!"};

int is_operator(const char *str) {
  for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
    if (strcmp(str, operators[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

void add_token(token **tokens, int *token_count, int *token_list_size,
               token_type type, const char *lexeme) {
  if (*token_count >= *token_list_size) {
    *token_list_size *= 2;
    *tokens = realloc(*tokens, *token_list_size * sizeof(token));
  }

  (*tokens)[*token_count].lexeme = strdup(lexeme);
  (*tokens)[*token_count].type = type;
  (*token_count)++;
}

token *shr_tokenize(const char *input, int *num_tokens) {
  int len = strlen(input);
  int token_count = 0;
  int token_list_size = INITIAL_TOKEN_LIST_SIZE;
  token *tokens = malloc(token_list_size * sizeof(token));

  int i = 0;
  while (i < len) {
    if (isspace(input[i])) {
      i++;
      continue;
    }

    if (isdigit(input[i]) ||
        (input[i] == '.' && i + 1 < len && isdigit(input[i + 1]))) {
      char num[MAX_TOKEN_SIZE] = {0};
      int j = 0;
      while (i < len && (isdigit(input[i]) || input[i] == '.')) {
        num[j++] = input[i++];
      }
      add_token(&tokens, &token_count, &token_list_size, NUMBER, num);
      continue;
    }

    if (input[i] == '"') {
      i++;
      char *str = malloc(1);
      int str_len = 0;
      while (i < len && input[i] != '"') {
        str = realloc(str, str_len + 2);
        str[str_len++] = input[i++];
      }
      str[str_len] = '\0';
      if (i < len)
        i++;
      add_token(&tokens, &token_count, &token_list_size, STRING, str);
      free(str);
      continue;
    }

    if (input[i] == '\'') {
      i++;
      char *str = malloc(1);
      int str_len = 0;
      while (i < len && input[i] != '\'') {
        str = realloc(str, str_len + 2);
        str[str_len++] = input[i++];
      }
      str[str_len] = '\0';
      if (i < len)
        i++;
      add_token(&tokens, &token_count, &token_list_size, STRING, str);
      free(str);
      continue;
    }

    if (isalpha(input[i]) || input[i] == '_') {
      char identifier[MAX_TOKEN_SIZE] = {0};
      int j = 0;
      while (i < len && (isalnum(input[i]) || input[i] == '_')) {
        identifier[j++] = input[i++];
      }
      add_token(&tokens, &token_count, &token_list_size, IDENT, identifier);
      continue;
    }

    if (i + 1 < len && is_operator((char[]){input[i], input[i + 1], '\0'})) {
      add_token(&tokens, &token_count, &token_list_size, OP,
                (char[]){input[i], input[i + 1], '\0'});
      i += 2;
      continue;
    }

    if (is_operator((char[]){input[i], '\0'})) {
      add_token(&tokens, &token_count, &token_list_size, OP,
                (char[]){input[i], '\0'});
      i++;
      continue;
    }

    printf("Unrecognized character: %c\n", input[i]);
    i++;
  }

  *num_tokens = token_count;
  return tokens;
}

void shr_free_tokens(token *tokens, int num_tokens) {
  for (int i = 0; i < num_tokens; i++) {
    free(tokens[i].lexeme);
  }
  free(tokens);
}

const char *shr_token_type_to_string(token_type tktype) {
  switch (tktype) {
  case IDENT:
    return "IDENTIFIER";
  case OP:
    return "OPERATOR";
  case STRING:
    return "STRING";
  case NUMBER:
    return "NUMBER";
  default:
    return "UNKNOWN";
  }
}
