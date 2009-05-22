#ifndef INCLUDE_TEST_RES_H
#define INCLUDE_TEST_RES_H

#define MAX_RESULTS 20

typedef struct test_results_t
{
    int result_count;
    struct
    {
        char *desc;
        long int p;
        long int s;
        long int c;
        int time;
    } results[MAX_RESULTS];
} test_results, *test_results_ptr;

test_results test_create_results();
void test_add_result(test_results_ptr results, char *desc, long int p, long int s, long int c, int time);
void test_print_results(test_results results);
void test_dispose_results(test_results_ptr results);
#endif
