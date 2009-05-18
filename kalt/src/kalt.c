#include <stdio.h>
#include "parser.h"
#include "syntax.h"

#define VERSION 1


int main(void)
{
    printf("Welcome to kalt - Krettek Algorithmus Tester version %d\n", VERSION);
    parse_result result = parse_tokenize("hallo = aljoshca(ali,foom = ali)");
    //parse_result result = parse_tokenize("foom = ");
    parse_dump(result);
    syntax_result syn_result = syntax_check(result);
    if (syn_result.success == 1)
    {
        printf("Syntax correct.\n");
    }
    else
    {
        printf("Syntax error: %s\n", syn_result.error_str);
    }
    /*parse_result subset = parse_create_subset(result,8,9);
    parse_dump(subset);
    subset = parse_create_subset(subset, 0,0);
    parse_dump(subset);
    parse_dispose_result(result);*/
    return 0;
}
