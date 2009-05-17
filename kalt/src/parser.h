#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

typedef enum token_type_t
{
    TOKEN_IDENTIFIER, TOKEN_ASSIGN, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LSPAREN, TOKEN_RSPAREN, TOKEN_COMMA, TOKEN_NUMBER, TOKEN_NIL
} token_type;

typedef struct token_t
{
    token_type type;
    char *text;
} token, *token_ptr;

typedef struct parse_result_t
{
    int success;
    char *error_str;
    int token_count;
    token_ptr tokens;
} parse_result, *parse_result_ptr;

parse_result parse_tokenize(char *text);
void parse_dispose_result(parse_result);
void parse_dump(parse_result);

#endif

