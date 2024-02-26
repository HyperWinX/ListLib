#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include "list.h"
#include "tests/tests.h"
#include "tests/ctestify.h"

jmp_buf sigsegv_restore = {0};

int pcomparer(void* ptr1, void* ptr2){
    if (*((int*)ptr1) != *((int*)ptr2)) return 1;
    else return 0;
}

void paction(void* ptr){
    *(int*)ptr = *(int*)ptr + 1;
}

void pprint(void* ptr){
	printf("%d\n", *(int*)ptr);
}

int ptest_list_init(void){
    SET_TEST_SUITE_NAME(LIST_INIT);
    struct list listobj = {0};
    ASSERT_FALSE(PLIST_INIT, list_init(&listobj, 256, sizeof(int32_t)));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_destroy(void){
    SET_TEST_SUITE_NAME(LIST_DESTROY);
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    ASSERT_FALSE(PLIST_DESTROY, list_destroy(&listobj));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_set(void){
    SET_TEST_SUITE_NAME(LIST_SET);
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int32_t element = 871623;
    ASSERT_FALSE(PLIST_SET, list_set(&listobj, &element, 0));
    EXPECT_FALSE(PLIST_SETCMP, memcmp(listobj.ptr, &element, sizeof(int32_t)));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_get(void){
    SET_TEST_SUITE_NAME(LIST_GET);
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int32_t element = 871623;
    list_set(&listobj, &element, 0);
    int32_t restored = 0;
    ASSERT_FALSE(PLIST_GET, list_get(&listobj, &restored, 0));
    EXPECT_EQ(PLIST_GETCMP, restored, element);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_add(void){
    SET_TEST_SUITE_NAME(LIST_ADD);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t tmp = 123094;
    int32_t element = 348907;
    ASSERT_FALSE(PLIST_ADD, list_add(&listobj, &tmp));
    ASSERT_FALSE(PLIST_ADD2, list_add(&listobj, &element));
    EXPECT_EQ(PLIST_ADD3, listobj.elementcount, 2);
    EXPECT_EQ(PLIST_ADD4, listobj.listsz, listobj.elementsize * 2);
    EXPECT_EQ(PLIST_ADD5, listobj.allocated, listobj.elementsize * 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_addrange1(void){
    SET_TEST_SUITE_NAME(LIST_ADDRANGE1);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    ASSERT_FALSE(PLIST_ADDRANGE, list_addrange1(&listobj, &arr, 4));
    int32_t restored[4] = {0};
    for (int i = 0; i < 4; i++){
        list_get(&listobj, &(restored[i]), i);
    }
    ASSERT_FALSE(LIST_ADDRANGE1_CHECK, memcmp(&arr, &restored, sizeof(int32_t) * 4));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_addrange2(void){
    SET_TEST_SUITE_NAME(LIST_ADDRANGE2);
    srand(time(NULL));
    struct list listobj = {0};
    struct list addition = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    list_init(&addition, 4, sizeof(int32_t));
    for (int i = 0; i < 4; i++){
        int32_t num = rand();
        list_add(&addition, &num);
    }
    ASSERT_FALSE(PLIST_ADDRANGE2_CHECK, list_addrange2(&listobj, &addition));
    ASSERT_FALSE(PLIST_ADDRANGE2_CHECK, memcmp(addition.ptr, listobj.ptr, sizeof(int32_t)*4));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_clear(void){
    SET_TEST_SUITE_NAME(LIST_CLEAR);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    ASSERT_FALSE(PLIST_CLEAR, list_clear(&listobj));
    ASSERT_EQ(PLIST_CLEAR_CHECK, listobj.elementcount, 0);
    ASSERT_EQ(PLIST_CLEAR_CHECK, listobj.listsz, 0);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_contains(void){
    SET_TEST_SUITE_NAME(LIST_CONTAINS);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    int32_t find = 1;
    ASSERT_FALSE(PLIST_CONTAINS, list_contains(&listobj, &find));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_exists(void){
    SET_TEST_SUITE_NAME(LIST_EXISTS);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    ASSERT_FALSE(PLIST_EXISTS, list_exists(&listobj, pcomparer, &arr[1]));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_find(void){
    SET_TEST_SUITE_NAME(LIST_FIND);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 8675;
    int32_t restored = 0;
    ASSERT_FALSE(PLIST_FIND, list_find(&listobj, &find, pcomparer, &restored));
    ASSERT_EQ(PLIST_FIND_CHECK, restored, find);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findindex1(void){
    SET_TEST_SUITE_NAME(LIST_FINDINDEX1);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 123;
    ASSERT_EQ(PLIST_FINDINDEX1, list_findindex1(&listobj, 2, 5, pcomparer, &find), 4);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findindex2(void){
    SET_TEST_SUITE_NAME(LIST_FINDINDEX2);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 123;
    ASSERT_EQ(PLIST_FINDINDEX1, list_findindex2(&listobj, 4, pcomparer, &find), 4);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findindex3(void){
    SET_TEST_SUITE_NAME(LIST_FINDINDEX3);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 123;
    ASSERT_EQ(PLIST_FINDINDEX1, list_findindex3(&listobj, pcomparer, &find), 4);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlast(void){
    SET_TEST_SUITE_NAME(LIST_FINDLAST);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[10] = {4, 7, 1, 9, 123, 5, 8675, 92478, 8, 5};
    list_addrange1(&listobj, &arr, 10);
    int32_t find = 8;
    int32_t restored = 0;
    ASSERT_FALSE(PLIST_FINDLAST, list_findlast(&listobj, &find, &restored, pcomparer));
    ASSERT_EQ(PLIST_FINDLAST_CHECK, restored, find);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlastindex1(void){
    SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX1);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_FINDLASTINDEX1, list_findlastindex1(&listobj, 2, 6, pcomparer, &find), 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlastindex2(void){
    SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX2);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_FINDLASTINDEX2, list_findlastindex2(&listobj, 2, pcomparer, &find), 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlastindex3(void){
    SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX3);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_FINDLASTINDEX3, list_findlastindex3(&listobj, pcomparer, &find), 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_foreach(void){
	SET_TEST_SUITE_NAME(LIST_FOREACH);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    for (int i = 0; i < 8; i++) arr[i]++;
    list_foreach(&listobj, paction);
	for (int i = 0; i < 8; i++){
		int restored = 0;
		list_get(&listobj, &restored, i);
		ASSERT_EQ(PLIST_FOREACH_CHECK, restored, arr[i]);
	}
    ASSERT_FALSE(PLIST_FOREACH_CHECK, memcmp(listobj.ptr, &arr, sizeof(arr)));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_indexof1(void){
	SET_TEST_SUITE_NAME(LIST_INDEXOF1);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_INDEXOF1, list_indexof1(&listobj, &find), 3);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_indexof2(void){
	SET_TEST_SUITE_NAME(LIST_INDEXOF2);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_INDEXOF2, list_indexof2(&listobj, &find, 2), 3);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_indexof3(void){
	SET_TEST_SUITE_NAME(LIST_INDEXOF3);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_INDEXOF3, list_indexof3(&listobj, &find, 2, 5), 3);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_insert(void){
	SET_TEST_SUITE_NAME(LIST_INSERT);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
	list_addrange1(&listobj, &arr, 8);
    int32_t test_val = 420, restored = 0;
    ASSERT_FALSE(PLIST_INSERT, list_insert(&listobj, &test_val, 2));
    list_get(&listobj, &restored, 2);
    ASSERT_EQ(PLIST_INSERT_CHECK, restored, 420);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_lastindexof1(void){
	SET_TEST_SUITE_NAME(LIST_LASTINDEXOF1);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_LASTINDEXOF1, list_lastindexof1(&listobj, &find), 5);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_lastindexof2(void){
	SET_TEST_SUITE_NAME(LIST_LASTINDEXOF2);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_LASTINDEXOF2, list_lastindexof2(&listobj, &find, 4), 5);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_lastindexof3(void){
	SET_TEST_SUITE_NAME(LIST_LASTINDEXOF3);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_LASTINDEXOF3, list_lastindexof3(&listobj, &find, 4, 6), 5);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_remove(void){
	SET_TEST_SUITE_NAME(LIST_REMOVE);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_FALSE(PLIST_REMOVE, list_remove(&listobj, &find));
    int check = 0;
    list_get(&listobj, &check, 3);
	ASSERT_NEQ(PLIST_REMOVE_CHECK, check, find);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_removeall(void){
	SET_TEST_SUITE_NAME(LIST_REMOVEALL);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_REMOVEALL, list_removeall(&listobj, &find, pcomparer), 2);
	CHECK_ALLOCATED
	return 0;
}

int ptest_list_removeat(void){
	SET_TEST_SUITE_NAME(LIST_REMOVEAT);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    ASSERT_FALSE(PLIST_REMOVEAT, list_removeat(&listobj, 3));
    int32_t check = 0;
    list_get(&listobj, &check, 3);
    ASSERT_EQ(PLIST_REMOVEAT_CHECK, check, 123);
	CHECK_ALLOCATED
	return 0;
}

int ptest_list_reverse(void){
	SET_TEST_SUITE_NAME(LIST_REVERSE);
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    ASSERT_FALSE(PLIST_REVERSE, list_reverse(&listobj));
    for (int i = 0; i < 8; i++){
        int check = 0;
        list_get(&listobj, &check, 7 - i);
        ASSERT_EQ(PLIST_REVERSE_CHECK, check, arr[i]);
    }
	CHECK_ALLOCATED
    return 0;
}

int ntest_list_init(void){
	SET_TEST_SUITE_NAME(LIST_INIT);
	struct list listobj = {0};
	ASSERT_TRUE(NTEST_LIST_INIT1, list_init((void*)0, 1, 1));
	ASSERT_TRUE(NTEST_LIST_INIT2, list_init(&listobj, 0, 1));
	ASSERT_TRUE(NTEST_LIST_INIT3, list_init(&listobj, 1, 0));
	return 0;
}

int ntest_list_destroy(void){
	SET_TEST_SUITE_NAME(LIST_DESTROY);
	ASSERT_TRUE(NTEST_LIST_DESTROY, list_destroy((void*)0));
	return 0;
}

int ntest_list_set(void){
	SET_TEST_SUITE_NAME(LIST_SET);
	struct list listobj;
	int t = 5;
	list_init(&listobj, 1, sizeof(int));
	ASSERT_TRUE(NTEST_LIST_SET1, list_set((void*)0, &t, 0));
	ASSERT_TRUE(NTEST_LIST_SET2, list_set(&listobj, (void*)0, 0));
	ASSERT_TRUE(NTEST_LIST_SET3, list_set(&listobj, &t, 10));
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_get(void){
	struct list listobj;
	list_init(&listobj, 1, sizeof(int));
	int restore;
	ASSERT_TRUE(NTEST_LIST_GET1, list_get((void*)0, &restore, 0));
	ASSERT_TRUE(NTEST_LIST_GET2, list_get(&listobj, (void*)0, 0));
	ASSERT_TRUE(NTEST_LIST_GET3, list_get(&listobj, &restore, 5));
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_add(void){
	SET_TEST_SUITE_NAME(LIST_ADD);
	struct list listobj;
	list_init(&listobj, 1, sizeof(int));
	int t = 5;
	ASSERT_TRUE(NTEST_LIST_ADD1, list_add((void*)0, &t));
	ASSERT_TRUE(NTEST_LIST_ADD2, list_add(&listobj, (void*)0));
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_addrange1(void){
	SET_TEST_SUITE_NAME(LIST_ADDRANGE1);
	struct list listobj;
	list_init(&listobj, 1, sizeof(int));
	int arr[3] = {0};
	ASSERT_TRUE(NTEST_LIST_ADDRANGE1_1, list_addrange1((void*)0, &arr, 3));
	ASSERT_TRUE(NTEST_LIST_ADDRANGE1_2, list_addrange1(&listobj, (void*)0, 3));
	ASSERT_TRUE(NTEST_LIST_ADDRANGE1_3, list_addrange1(&listobj, &arr, 0));
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_addrange2(void){
    SET_TEST_SUITE_NAME(LIST_ADDRANGE2);              struct list listobj, arr;
    list_init(&listobj, 1, sizeof(int));
    ASSERT_TRUE(NTEST_LIST_ADDRANGE2_1, list_addrange2((void*)0, &arr));
    ASSERT_TRUE(NTEST_LIST_ADDRANGE2_2, list_addrange2(&listobj, (void*)0));
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_clear(void){
	SET_TEST_SUITE_NAME(LIST_CLEAR);
	ASSERT_TRUE(NTEST_LIST_CLEAR, list_clear((void*)0));
	return 0;
}

test tests[] = {
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
	ntest_list_init,
	ntest_list_destroy,
	ntest_list_set,
	ntest_list_get,
	ntest_list_add,
	ntest_list_addrange1,
	ntest_list_addrange2,
	ntest_list_clear
};

int test_count = 38;

void test_main(void){
    for (int i = 0; i < test_count; i++){
        tests[i]();
    }
}
