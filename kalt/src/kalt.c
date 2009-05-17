#include <stdio.h>
#include "parser.h"

#define VERSION 1


int main(void)
{
    printf("Welcome to kalt - Krettek Algorithmus Tester version %d\n", VERSION);
    parse_result result = parse_tokenize("= test1 = [1a   ,2,3]");
    parse_dump(result);
    return 0;
}
