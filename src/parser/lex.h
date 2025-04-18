#ifndef INCLUDE_PARSER_LEX_H_
#define INCLUDE_PARSER_LEX_H_

typedef enum { IDENT, OP, STRING, NUMBER } token_type;

typedef struct {
  char *lexeme;
  token_type type;
} token;

extern token *shr_tokenize(const char *input, int *num_tokens);
extern void shr_free_tokens(token *tokens, int num_tokens);
extern const char *shr_token_type_to_string(token_type tktype);

#endif // INCLUDE_PARSER_LEX_H_
