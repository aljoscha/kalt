#include "parser.h"
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_IDENTIFIER_LENGTH 200
#define MAX_TOKENS 1000

#define PARSING_NUMBER 1
#define PARSING_IDENTIFIER 2



parse_result parse_tokenize(char *text)
{
    token tokens[MAX_TOKENS];
    parse_result result;
    int token_count = 0;
    int text_position = 0;
    int parsed_position = 0;
    char current_parse[21];
    int currently_parsing = 0;
    while (text[text_position] != '\0')
    {
        if (parsed_position > 0)
        {
            if (!(isalpha(text[text_position]) || isdigit(text[text_position])))
            {
                current_parse[parsed_position] = '\0';
                tokens[token_count].text = malloc(parsed_position+1);
                if (currently_parsing == PARSING_NUMBER)
                    tokens[token_count].type = TOKEN_NUMBER;
                else
                    tokens[token_count].type = TOKEN_IDENTIFIER;
                strcpy(tokens[token_count].text, current_parse);
                token_count++;
                currently_parsing = 0;
                parsed_position = 0;
            } 
        }
        if(text[text_position] == '=')
        {
            tokens[token_count].type = TOKEN_ASSIGN;
            tokens[token_count].text = NULL;
            token_count++;
            text_position++;
            continue;
        }
        if(text[text_position] == '(')
        {
            tokens[token_count].type = TOKEN_LPAREN;
            tokens[token_count].text = NULL;
            token_count++;
            text_position++;
            continue;
        }
        if(text[text_position] == ')')
        {
            tokens[token_count].type = TOKEN_RPAREN;
            tokens[token_count].text = NULL;
            token_count++;
            text_position++;
            continue;
        }
        if(text[text_position] == '[')
        {
            tokens[token_count].type = TOKEN_LSPAREN;
            tokens[token_count].text = NULL;
            token_count++;
            text_position++;
            continue;
        }
        if(text[text_position] == ']')
        {
            tokens[token_count].type = TOKEN_RSPAREN;
            tokens[token_count].text = NULL;
            token_count++;
            text_position++;
            continue;
        }
        if(text[text_position] == ',')
        {
            tokens[token_count].type = TOKEN_COMMA;
            tokens[token_count].text = NULL;
            token_count++;
            text_position++;
            continue;
        }
        if(text[text_position] == ' ') // finalize the currently parsed token if there is one
        {
            text_position++; 
            continue;       
        }
        if(isalpha(text[text_position]))
        {
            if(currently_parsing == PARSING_NUMBER) //wrong character
            {
                result.success = 0;
                result.error_str = "invalid character, number expected";
                result.token_count = 0;
                result.tokens = NULL;
                return result;
            }
            currently_parsing = PARSING_IDENTIFIER;
            current_parse[parsed_position] = text[text_position];
            parsed_position++;
            text_position++;
            continue;
        }
        if(isdigit(text[text_position]))
        {
            if(currently_parsing == PARSING_IDENTIFIER) //wrong character
            {
                result.success = 0;
                result.error_str = "invalid character, alphanumeric expected";
                result.token_count = 0;
                result.tokens = NULL;
                return result;
            }
            currently_parsing = PARSING_NUMBER;
            current_parse[parsed_position] = text[text_position];
            parsed_position++;
            text_position++;
            continue;
        }

    }
    result.success = 1;
    result.error_str = "no error";
    result.token_count = token_count;
    result.tokens = malloc(sizeof(token)*token_count);
    memcpy(result.tokens, tokens, sizeof(token)*token_count);
    return result;
}

void parse_dispose_result(parse_result parse)
{
    int index;
    for (index = 0; index < parse.token_count; index++)
    {
        if(parse.tokens[index].text != NULL)
            free(parse.tokens[index].text);
    }
    free(parse.tokens);
}

void parse_dump(parse_result parse)
{
    int index;
    printf("Dumping parse result...\n");
    if(parse.success == 1)
        printf("parse was successfull: %s\n", parse.error_str);
    else
    {
        printf("parse was not successfull: %s\n", parse.error_str);
        return;
    }
    for (index = 0; index < parse.token_count; index++)
    {
        switch(parse.tokens[index].type)
        {
            case TOKEN_IDENTIFIER:
                {
                    printf("IDENTIFIER: %s\n", parse.tokens[index].text);
                } break;
            case TOKEN_NUMBER:
                {
                    printf("NUMBER: %s\n", parse.tokens[index].text);
                } break;
            case TOKEN_ASSIGN:
                {
                    printf("ASSIGN\n");
                } break;
            case TOKEN_LPAREN:
                {
                    printf("LPAREN\n");
                } break;
            case TOKEN_RPAREN:
                {
                    printf("RPAREN\n");
                } break;
            case TOKEN_LSPAREN:
                {
                    printf("LSPAREN\n");
                } break;
            case TOKEN_RSPAREN:
                {
                    printf("RSPAREN\n");
                } break;
            case TOKEN_COMMA:
                {
                    printf("COMMA\n");
                } break;
            case TOKEN_NIL:
                {
                    printf("NIL\n");
                } break;
        }
    }    
}
