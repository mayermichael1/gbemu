#include "../include/test_suite.h"

void
test_unit_list_append (test_unit_list *unit_data, test_unit data)
{
    test_unit_node *node = (test_unit_node*)malloc(sizeof(test_unit_node));
    node->data = data;

    ++unit_data->count;

    if (unit_data->head == NULL)
    {
        unit_data->head = node;
        unit_data->tail = node;
    }
    else
    {
        unit_data->tail->next = node;
        unit_data->tail = node;
    }
}

void
load_test_suite (test_unit_list *list, char *lib_name)
{
    char function_name[256] = "";
    char line_number_name[256] = "";
    int unit_counter = 0;

    void *lib = dlopen(lib_name, RTLD_NOW | RTLD_NODELETE);

    if(lib)
    {
        while(true)
        {
            test_unit unit;
            sprintf(function_name, "test_%i", unit_counter);
            sprintf(line_number_name, "ln_test_%i", unit_counter);

            unit.function= (test_function*)dlsym(lib, function_name);
            int *line_number = (int*)dlsym(lib, line_number_name);

            if (line_number != NULL)
            {
                unit.line_number = *line_number;
            }

            if (unit.function != NULL)
            {
                test_unit_list_append(list, unit);
            }
            else
            {
                break;
            }

            ++unit_counter;
        } 
        dlclose(lib);
    }
}

void
run_test_suite(test_unit_list list)
{
    test_unit_node *node = list.head;
    int test_count = 0;
    int passed_count = 0;

    int stdout_file = dup(STDOUT_FILENO);
    int null_file = open("/dev/null", 0);

    while( node != NULL)
    {

        fflush(stdout);
        dup2(null_file, STDOUT_FILENO);

        bool result = node->data.function();

        fflush(stdout);
        dup2(stdout_file, STDOUT_FILENO);

        printf(
            "function on line %i:\t %s\n", 
            node->data.line_number, 
            (result) ?  "PASSED" : "FAILED"
        );

        test_count++;
        if (result)
        {
            passed_count++;
        }

        node = node->next;
    }

    close(null_file);
    if (test_count > 0)
    {
        printf("%i of %i units PASSED\n", passed_count, test_count);
    }
}
