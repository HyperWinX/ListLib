#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include "list.h"
#include "tests/tests.h"

jmp_buf sigsegv_restore = {0};

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
    ptest_list_init,
    ptest_list_destroy,
    ptest_list_set,
    ptest_list_get,
    ptest_list_add,
    ptest_list_addrange1,
    ptest_list_addrange2,
    ptest_list_clear,
    ptest_list_contains,
    ptest_list_exists,
    ptest_list_find,
    ptest_list_findindex1,
    ptest_list_findindex2,
    ptest_list_findindex3,
    ptest_list_findlast,
    ptest_list_findlastindex1,
    ptest_list_findlastindex2,
    ptest_list_findlastindex3,
    ptest_list_foreach,
    ptest_list_indexof1,
    ptest_list_indexof2,
    ptest_list_indexof3,
    ptest_list_insert,
    ptest_list_lastindexof1,
    ptest_list_lastindexof2,
    ptest_list_lastindexof3,
    ptest_list_remove,
    ptest_list_removeall,
    ptest_list_removeat,
    ptest_list_reverse,
};

test negative_tests[] = {
    ntest_list_init,
    ntest_list_destroy,
    ntest_list_set,
    ntest_list_get,
    ntest_list_add,
    ntest_list_addrange1,
    ntest_list_addrange2,
    ntest_list_clear,
    ntest_list_contains,
    ntest_list_exists,
    ntest_list_find,
    ntest_list_findindex1,
    ntest_list_findindex2,
    ntest_list_findindex3,
    ntest_list_findlast,
    ntest_list_findlastindex1,
    ntest_list_findlastindex2,
    ntest_list_findlastindex3,
    ntest_list_foreach,
    ntest_list_indexof1,
    ntest_list_indexof2,
    ntest_list_indexof3,
    ntest_list_insert,
    ntest_list_lastindexof1,
    ntest_list_lastindexof2,
    ntest_list_lastindexof3,
    ntest_list_remove,
    ntest_list_removeall,
    ntest_list_removeat,
    ntest_list_reverse,
};

int positive_test_count = 29;

void write_failure(char* text, int retcode){
    printf(ANSI_COLOR_RED "%s: %d%s\n", text, retcode, ANSI_COLOR_RESET);
}

void write_success(char* text){
    printf(ANSI_COLOR_GREEN "%s%s\n", text, ANSI_COLOR_RESET);
}

void run_tests(void){
    printf("Running positive tests...\n");
    for (int i = 0; i < positive_test_count; i++){
        printf("[%d/%d] %s\n", i + 1, positive_test_count, test_strings[i]);
        int retcode = 0;
        retcode = positive_tests[i]();
        POST_DEBUG
        retcode = 0;
        retcode = negative_tests[i]();
        NEGT_DEBUG
    }
    printf(ANSI_COLOR_GREEN "All positive tests completed successfully!\n" ANSI_COLOR_RESET);
}

int main(){
    signal(SIGSEGV, sigsegv_handler);
    if (setjmp(sigsegv_restore)){
        printf("\tNegative: %s[ SIGSEGV ]\n", ANSI_COLOR_RED);
        printf("Restored from segmentation fault. Tests stopped.\n" ANSI_COLOR_RESET);
    }
    else
        run_tests();
    return -1;
}