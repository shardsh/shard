#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shard/common.h"
#include "shard/lex.h"

#define MAX_TOKEN_SIZE 100
#define INITIAL_TOKEN_LIST_SIZE 10

const char *operators[] = {"=",  "||", "&&", ">",  "0>", "1>",
                           "2>", "|",  "|0", "|1", "|2", ";"};

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

    if (input[i] == '#') {
      while (input[i] != '\n') {
        i++;
      }
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

    if (input[i] == '$') {
      if (i + 1 < len && input[i + 1] == '*') {
        add_token(&tokens, &token_count, &token_list_size, VAR, "$*");
        i += 2;
        continue;
      }
      if (i + 1 < len && (isalpha(input[i + 1]) || input[i + 1] == '_')) {
        i++;
        char identifier[MAX_TOKEN_SIZE] = {0};
        int j = 0;
        while (i < len && (isalnum(input[i]) || input[i] == '_' ||
                           input[i] == '*' || input[i] == '#')) {
          identifier[j++] = input[i++];
        }
        add_token(&tokens, &token_count, &token_list_size, VAR, identifier);
        continue;
      }
    }

    if (input[i] == '@' && i + 1 < len &&
        (isalpha(input[i + 1]) || input[i + 1] == '_')) {
      i++;
      char identifier[MAX_TOKEN_SIZE] = {0};
      int j = 0;
      while (i < len && (isalnum(input[i]) || input[i] == '_')) {
        identifier[j++] = input[i++];
      }
      add_token(&tokens, &token_count, &token_list_size, JOB, identifier);
      continue;
    }

    if ((input[i] == '-' && i + 1 < len && (input[i + 1] == '-'))) {
      i++;
      char identifier[MAX_TOKEN_SIZE] = {0};
      int j = 0;
      while (i < len && (isalnum(input[i]) || input[i] == '_')) {
        identifier[j++] = input[i++];
      }
      add_token(&tokens, &token_count, &token_list_size, FLAG, identifier);
      continue;
    }

    if (input[i] == '-' && i + 1 < len &&
        (isalpha(input[i + 1]) || input[i + 1] == '_')) {
      i++;
      char identifier[MAX_TOKEN_SIZE] = {0};
      int j = 0;
      while (i < len && (isalnum(input[i]) || input[i] == '_')) {
        identifier[j++] = input[i++];
      }
      add_token(&tokens, &token_count, &token_list_size, FLAG, identifier);
      continue;
    }

    if (isalpha(input[i]) || input[i] == '_') {
      char identifier[MAX_TOKEN_SIZE] = {0};
      int j = 0;
      while (i < len && (isalnum(input[i]) || input[i] == '_')) {
        identifier[j++] = input[i++];
      }
      add_token(&tokens, &token_count, &token_list_size, CMD, identifier);
      continue;
    }

    char op2[3] = {input[i], input[i + 1], '\0'};
    if (i + 1 < len && is_operator(op2)) {
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

    ERROR("Unrecognized character: %c\n", input[i]);
    i++;
  }

  *num_tokens = token_count;
  return tokens;
}

void shr_free_tokens(token *tokens, int num_tokens) {
  for (int i = 0; i < num_tokens; i++) {
    MEMLOG("Freed token lexeme: %s", tokens[i].lexeme);
    free(tokens[i].lexeme);
  }
  MEMLOG("Freeing tokens", NULL);
  free(tokens);
}

const char *shr_token_type_to_string(token_type tktype) {
  switch (tktype) {
  case VAR:
    return "VARIABLE";
  case JOB:
    return "JOB";
  case OP:
    return "OPERATOR";
  case STRING:
    return "STRING";
  case NUMBER:
    return "NUMBER";
  case CMD:
    return "COMMAND";
  default:
    return "UNKNOWN";
  }
}

#include <shard/colors.h>

char *shr_colorize_tokens(token *tokens, int num_tokens) {
  size_t buf_size = 1024;
  char *colorized_output = malloc(buf_size);
  if (!colorized_output)
    return NULL;

  colorized_output[0] = '\0';
  size_t len = 0;

  for (int i = 0; i < num_tokens; i++) {
    const char *color;
    const char *prefix = " ";
    const char *suffix = " ";
    switch (tokens[i].type) {
    case VAR:
      color = BLUE;
      prefix = "$";
      break;
    case CMD:
      color = WHITE;
      break;
    case FLAG:
      prefix = "-";
      color = HIGH_INTENSTY_TEXT_PURPLE;
      break;
    case STRING:
      prefix = "\"";
      suffix = "\"";
      color = GREEN;
      break;
    case NUMBER:
      color = PURPLE;
      break;
    case OP:
      color = RED;
      break;
    default:
      color = WHITE;
      break;
    }

    const char *reset = COLOR_RESET;

    size_t needed =
        strlen(color) + strlen(tokens[i].lexeme) + strlen(reset) + 2;

    if (len + needed >= buf_size) {
      buf_size *= 2;
      char *tmp = realloc(colorized_output, buf_size);
      if (!tmp) {
        free(colorized_output);
        return NULL;
      }
      colorized_output = tmp;
    }

    snprintf(colorized_output + len, buf_size - len, "%s%s%s%s%s ", color,
             prefix, tokens[i].lexeme, suffix, reset);
    len += strlen(color) + strlen(tokens[i].lexeme) + strlen(reset) + 1;
  }

  return colorized_output;
}
