#ifndef INCLUDE_SYNTAX_H
#define INCLUDE_SYNTAX_H

#include "parser.h"

typedef struct syntax_result_t
{
    int success;
    char *error_str;
} syntax_result, *syntax_result_ptr;

syntax_result syntax_check(parse_result parse);

#endif
