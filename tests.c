#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tests/tests.h"


char* test_strings[] = {
    "List initialization...              ",
    "List destroy...                     ",
    "List value set...                   ",
    "List value get...                   ",
    "List add value...                   ",
    "List addrange1...                   ",
    "List addrange2...                   ",
    "List clear...                       ",
    "List contains element...            ",
    "List element exists...              ",
    "List find element using predicate...",
    "List find index 1 using predicate...",
    "List find index 2 using predicate...",
    "List find index 3 using predicate...",
    "List find last element using pred...",
    "List find last index 1 using pred...",
    "List find last index 2 using pred...",
    "List find last index 3 using pred...",
    "List execute action for each element",
    "List index of element 1...          ",
    "List index of element 2...          ",
    "List index of element 3...          ",
    "List insert element...              ",
    "List last index of element 1...     ",
    "List last index of element 2...     ",
    "List last index of element 3...     ",
    "List remove element...              ",
    "List remove all using predicate...  ",
    "List remove element at index...     ",
    "List reverse..."
};

test positive_tests[] = {
    test_list_init,
    test_list_destroy,
    test_list_set,
    test_list_get,
    test_list_add,
    test_list_addrange1,
    test_list_addrange2,
    test_list_clear,
    test_list_contains,
    test_list_exists,
    test_list_find,
    test_list_findindex1,
    test_list_findindex2,
    test_list_findindex3,
    test_list_findlast,
    test_list_findlastindex1,
    test_list_findlastindex2,
    test_list_findlastindex3,
    test_list_foreach,
    test_list_indexof1,
    test_list_indexof2,
    test_list_indexof3,
    test_list_insert,
    test_list_lastindexof1,
    test_list_lastindexof2,
    test_list_lastindexof3,
    test_list_remove,
    test_list_removeall,
    test_list_removeat,
    test_list_reverse,
};

int positive_test_count = 29;

void write_failure(char* text, int retcode){
    printf(ANSI_COLOR_RED "%s: %d%s\n", text, retcode, ANSI_COLOR_RESET);
}

void write_success(char* text){
    printf(ANSI_COLOR_GREEN "%s%s\n", text, ANSI_COLOR_RESET);
}

void run_positive_tests(void){
    printf("Running positive tests...\n");
    for (int i = 0; i < positive_test_count; i++){
        printf("[%d/%d] %s", i + 1, positive_test_count, test_strings[i]);
        int retcode = 0;
        retcode = positive_tests[i]();
        DEBUG
    }
}

int main(){
    run_positive_tests();
}