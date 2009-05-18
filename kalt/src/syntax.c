#include "syntax.h"
#include <stdio.h>

#include <malloc.h>

#define DEBUG 1

#ifdef DEBUG
#define DOUT(text) (printf("%s",text))
#else
#define DOUT(text) ()
#endif


syntax_result syntax_check(parse_result parse)
{
    syntax_result result;
    result.success = 1;
    result.error_str = "no error";
    if (parse.token_count <= 0)
    {
        return result;
    }
    if(parse.token_count == 1)
    {
        if (parse.tokens[0].type == TOKEN_IDENTIFIER)
        {
            return result;
        }
        else
        {
            result.success = 0;
            result.error_str = "error: identifier expected";
            return result;
        }
    }
    if (parse.tokens[0].type == TOKEN_IDENTIFIER && parse.tokens[1].type == TOKEN_ASSIGN)
    {
        if (parse.token_count < 3)
        {
            result.success = 0;
            result.error_str = "error: R-value expected";
            return result;
        }
        else
        {
            parse_result subset = parse_create_subset(parse,2, parse.token_count - 1);
            return syntax_check(subset);
        }
    }
    if (parse.tokens[0].type == TOKEN_IDENTIFIER && parse.tokens[1].type == TOKEN_LPAREN)
    {
        parse_result subset;
        int current_end = 2;
        int current_start = 2;
        int paren_depth = 0;
        int running = 1;
        while(current_end < parse.token_count && running)
        {
            parse_result subset;
            syntax_result temp_result;
            switch(parse.tokens[current_end].type)
            {
                case TOKEN_LPAREN:
                    {
                        paren_depth++;    
                    } break;
                case TOKEN_RPAREN:
                    {
                        if (paren_depth == 0)
                        {
                            subset = parse_create_subset(parse,current_start, current_end-1);
                            temp_result = syntax_check(subset);
                            if (temp_result.success == 0)
                                return temp_result;
                            running = 0;
                            current_end--;
                        }
                        else
                        {
                            paren_depth++;
                        }
                    } break;
                case TOKEN_COMMA:
                    {
                        if (paren_depth == 0)
                        {
                            subset = parse_create_subset(parse,current_start, current_end-1);
                            temp_result = syntax_check(subset);
                            if (temp_result.success == 0)
                                return temp_result;
                            current_start = current_end+1;
                        }
                    } break;
            }
            current_end++;
            if (paren_depth < 0)
            {
                result.success = 0;
                result.error_str = "error: unmatches right parenthesis";
                return result;
            }
        }
        if (current_end != parse.token_count-1)
        {
            result.success = 0;
            result.error_str = "error: invalid function call or mismatched parenthesis, or stuff after function call";
            return result;
        }
        return result;
    }   
    result.success = 0;
    result.error_str = "error: unexpected input";
    return result;
}
