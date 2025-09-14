#ifndef TEST_SUITE
#define TEST_SUITE

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define TEST_FUNCTION(name) bool(name)(void)
typedef TEST_FUNCTION(test_function);

#define UNIT_COUNTED_(counter) \
    int ln_test_##counter = __LINE__ ; \
    TEST_FUNCTION(test_##counter) 

#define UNIT_COUNTED(counter) UNIT_COUNTED_(counter)

#define UNIT() UNIT_COUNTED(__COUNTER__)

TEST_FUNCTION(stub)
{
    printf("this is a stub function\n");
    __builtin_trap();
    return false;
}

typedef struct
{
    int line_number;
    test_function *function;
}
test_unit; 

typedef struct test_unit_node test_unit_node;
struct test_unit_node
{
    test_unit data;
    test_unit_node *next;
};

typedef struct
{
    test_unit_node *head;
    test_unit_node *tail;
    int count;
}
test_unit_list;

#endif
