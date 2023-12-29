#ifndef NEGATIVE_TESTS_H
#define NEGATIVE_TESTS_H

void sigsegv_handler(int s);
int ntest_list_init(void);
int ntest_list_destroy(void);
int ntest_list_get(void);
int ntest_list_set(void);
int ntest_list_add(void);
int ntest_list_addrange1(void);
int ntest_list_addrange2(void);
int ntest_list_clear(void);
int ntest_list_contains(void);
int ntest_list_exists(void);
int ntest_list_find(void);
int ntest_list_findindex1(void);
int ntest_list_findindex2(void);
int ntest_list_findindex3(void);
int ntest_list_findlast(void);
int ntest_list_findlastindex1(void);
int ntest_list_findlastindex2(void);
int ntest_list_findlastindex3(void);
int ntest_list_foreach(void);
int ntest_list_indexof1(void);
int ntest_list_indexof2(void);
int ntest_list_indexof3(void);
int ntest_list_insert(void);
int ntest_list_lastindexof1(void);
int ntest_list_lastindexof2(void);
int ntest_list_lastindexof3(void);
int ntest_list_remove(void);
int ntest_list_removeall(void);
int ntest_list_removeat(void);
int ntest_list_reverse(void);

#endif