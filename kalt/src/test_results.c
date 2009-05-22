#include "test_results.h"

#include <stdio.h>
#include <string.h>


test_results test_create_results()
{
    test_results results;
    results.result_count = 0;
    return results;
}

void test_add_result(test_results *results, char *desc, long int p, long int s, long int c, int time)
{
    results->results[results->result_count].desc = desc;
    results->results[results->result_count].p = p;
    results->results[results->result_count].s = s;
    results->results[results->result_count].c = c;
    results->results[results->result_count].time = time;
    results->result_count++;
}

void test_add_separator(test_results_ptr results)
{
    results->results[results->result_count].desc = "__sep__";
    results->result_count++;
}

// a helper function to print the line separator eg "+---------+-----+------+----+"
inline void print_separator(int desc_width, int p_width, int s_width, int c_width, int time_width)
{
    int index;
    printf("+");
    for (index = 0; index < desc_width+2; index++) printf("-");
    printf("+");
    for (index = 0; index < p_width+2; index++) printf("-");
    printf("+");
    for (index = 0; index < s_width+2; index++) printf("-");
    printf("+");
    for (index = 0; index < c_width+2; index++) printf("-");
    printf("+");
    for (index = 0; index < time_width+2; index++) printf("-");
    printf("+\n");
}

void test_print_results(test_results results)
{
    int index;
    int index2;
    char test_string[20];
    char format_string[] = "| %100s | %100d | %100d | %100d | %100d |\n"; // We only have the string literal here to measure the length

    // initial width is width of caption text
    int desc_width = 11;
    int p_width = 10;
    int s_width = 5;
    int c_width = 11;
    int time_width = 7;
    
    
    // calculate max length of each column
    for(index = 0; index < results.result_count; index++)
    {
        if (strcmp(results.results[index].desc, "__sep__") == 0) continue;
        if (strlen(results.results[index].desc) > desc_width) desc_width = strlen(results.results[index].desc);
        sprintf(test_string, "%ld", results.results[index].p);
        if (strlen(test_string) > p_width) p_width = strlen(test_string);
        sprintf(test_string, "%ld", results.results[index].s);
        if (strlen(test_string) > s_width) s_width = strlen(test_string);
        sprintf(test_string, "%ld", results.results[index].c);
        if (strlen(test_string) > c_width) c_width = strlen(test_string);
        sprintf(test_string, "%d", results.results[index].time);
        if (strlen(test_string) > time_width) time_width = strlen(test_string);
    }

    print_separator(desc_width, p_width, s_width, c_width, time_width);

    //print the captions
    sprintf(format_string, "| %%-%ds | %%%ds | %%%ds | %%%ds | %%%ds |\n", desc_width, p_width, s_width, c_width, time_width);
    printf(format_string, "description", "partitions", "swaps", "comparisons", "runtime");

    print_separator(desc_width, p_width, s_width, c_width, time_width);

    sprintf(format_string, "| %%-%ds | %%%dd | %%%dd | %%%dd | %%%dd |\n", desc_width, p_width, s_width, c_width, time_width);
    for(index = 0; index < results.result_count; index++)
    {
        if (strcmp(results.results[index].desc, "__sep__") == 0)
        {
            print_separator(desc_width, p_width, s_width, c_width, time_width);
        }
        else
        {
            printf(format_string, results.results[index].desc,  results.results[index].p,  results.results[index].s,  results.results[index].c,  results.results[index].time);
        }
    }

    print_separator(desc_width, p_width, s_width, c_width, time_width);
}

void test_dispose_results(test_results *results)
{
    results->result_count = 0;
}
