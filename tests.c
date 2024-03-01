#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include "list.h"
#include "tests/tests.h"
#include "CTestify/ctestify.h"

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

int ptest_list_destroy(void){
    SET_TEST_SUITE_NAME(LIST_DESTROY);
    struct list listobj = list_init(256, sizeof(int32_t));
    indexpair ret = list_destroy(&listobj);
    ASSERT_FALSE(PLIST_DESTROY, ret.code);
    return 0;
}

int ptest_list_set(void){
    SET_TEST_SUITE_NAME(LIST_SET);
    struct list listobj = list_init(256, sizeof(int32_t));
    int32_t element = 871623;
    indexpair ret = list_set(&listobj, &element, 0);
    ASSERT_FALSE(PLIST_SET, ret.code);
    ASSERT_FALSE(PLIST_SETCMP, memcmp(listobj.ptr, &element, sizeof(int32_t)));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_get(void){
    SET_TEST_SUITE_NAME(LIST_GET);
    struct list listobj = list_init(256, sizeof(int32_t));
    int32_t element = 871623;
    list_set(&listobj, &element, 0);
    int32_t restored = 0;
    indexpair ret = list_get(&listobj, &restored, 0);
    ASSERT_FALSE(PLIST_GET, ret.code);
    ASSERT_EQ(PLIST_GETCMP, restored, element);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_add(void){
    SET_TEST_SUITE_NAME(LIST_ADD);
    struct list listobj = list_init(1, sizeof(int32_t));
    int32_t tmp = 123094;
    int32_t element = 348907;
    indexpair ret = list_add(&listobj, &tmp);
    ASSERT_FALSE(PLIST_ADD, ret.code);
    ret = list_add(&listobj, &element);
    ASSERT_FALSE(PLIST_ADD2, ret.code);
    EXPECT_EQ(PLIST_ADD3, listobj.elementcount, 2);
    EXPECT_EQ(PLIST_ADD4, listobj.listsz, listobj.elementsize * 2);
    EXPECT_EQ(PLIST_ADD5, listobj.allocated, listobj.elementsize * 5);\
    list_destroy(&listobj);
    return 0;
}

int ptest_list_addrange1(void){
    SET_TEST_SUITE_NAME(LIST_ADDRANGE1);
    struct list listobj = list_init(1, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    struct list addition = list_init(4, sizeof(int32_t));
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    int32_t find = 1;
    ASSERT_FALSE(PLIST_CONTAINS, list_contains(&listobj, &find));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_exists(void){
    SET_TEST_SUITE_NAME(LIST_EXISTS);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    ASSERT_FALSE(PLIST_EXISTS, list_exists(&listobj, pcomparer, &arr[1]));
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_find(void){
    SET_TEST_SUITE_NAME(LIST_FIND);
    struct list listobj = list_init(1, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 123;
    ASSERT_EQ(PLIST_FINDINDEX1, list_findindex1(&listobj, 2, 5, pcomparer, &find), 4);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findindex2(void){
    SET_TEST_SUITE_NAME(LIST_FINDINDEX2);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 123;
    ASSERT_EQ(PLIST_FINDINDEX1, list_findindex2(&listobj, 4, pcomparer, &find), 4);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findindex3(void){
    SET_TEST_SUITE_NAME(LIST_FINDINDEX3);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 123;
    ASSERT_EQ(PLIST_FINDINDEX1, list_findindex3(&listobj, pcomparer, &find), 4);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlast(void){
    SET_TEST_SUITE_NAME(LIST_FINDLAST);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[10] = {4, 7, 1, 9, 123, 5, 8675, 92478, 8, 5};
    list_addrange1(&listobj, &arr, 10);
    int32_t find = 8;
    int32_t restored = 0;
    ASSERT_FALSE(PLIST_FINDLAST, list_findlast(&listobj, &find, pcomparer, &restored));
    ASSERT_EQ(PLIST_FINDLAST_CHECK, restored, find);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlastindex1(void){
    SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX1);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_FINDLASTINDEX1, list_findlastindex1(&listobj, 2, 6, pcomparer, &find), 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlastindex2(void){
    SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX2);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_FINDLASTINDEX2, list_findlastindex2(&listobj, 2, pcomparer, &find), 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_findlastindex3(void){
    SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX3);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_FINDLASTINDEX3, list_findlastindex3(&listobj, pcomparer, &find), 5);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_foreach(void){
	SET_TEST_SUITE_NAME(LIST_FOREACH);
    struct list listobj = list_init(6, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_INDEXOF1, list_indexof1(&listobj, &find), 3);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_indexof2(void){
	SET_TEST_SUITE_NAME(LIST_INDEXOF2);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_INDEXOF2, list_indexof2(&listobj, &find, 2), 3);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_indexof3(void){
	SET_TEST_SUITE_NAME(LIST_INDEXOF3);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_INDEXOF3, list_indexof3(&listobj, &find, 2, 5), 3);
    CHECK_ALLOCATED
    return 0;
}

int ptest_list_insert(void){
	SET_TEST_SUITE_NAME(LIST_INSERT);
    struct list listobj = list_init(1, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_LASTINDEXOF1, list_lastindexof1(&listobj, &find), 5);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_lastindexof2(void){
	SET_TEST_SUITE_NAME(LIST_LASTINDEXOF2);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_LASTINDEXOF2, list_lastindexof2(&listobj, &find, 4), 5);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_lastindexof3(void){
	SET_TEST_SUITE_NAME(LIST_LASTINDEXOF3);
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_LASTINDEXOF3, list_lastindexof3(&listobj, &find, 4, 6), 5);
	CHECK_ALLOCATED
    return 0;
}

int ptest_list_remove(void){
	SET_TEST_SUITE_NAME(LIST_REMOVE);
    struct list listobj = list_init(1, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    ASSERT_EQ(PLIST_REMOVEALL, list_removeall(&listobj, &find, pcomparer), 2);
	CHECK_ALLOCATED
	return 0;
}

int ptest_list_removeat(void){
	SET_TEST_SUITE_NAME(LIST_REMOVEAT);
    struct list listobj = list_init(1, sizeof(int32_t));
    
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
    struct list listobj = list_init(1, sizeof(int32_t));
    
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

int ntest_list_destroy(void){
	SET_TEST_SUITE_NAME(LIST_DESTROY);
    indexpair ret = list_destroy((void*)0);
	ASSERT_EQ(NTEST_LIST_DESTROY, ret.code, NULLPTR);
	return 0;
}

int ntest_list_set(void){
	SET_TEST_SUITE_NAME(LIST_SET);
	list listobj = list_init(1, sizeof(int));
	int t = 5;
	indexpair ret = list_set((void*)0, &t, 0);
	ASSERT_EQ(NTEST_LIST_SET1, ret.code, NULLPTR);
    ret = list_set(&listobj, (void*)0, 0);
	ASSERT_EQ(NTEST_LIST_SET2, ret.code, NULLPTR);
    ret = list_set(&listobj, &t, 10);
	ASSERT_EQ(NTEST_LIST_SET3, ret.code, ARGBADRANGE);
    list_destroy(&listobj);
	return 0;
}

int ntest_list_get(void){
	list listobj = list_init(1, sizeof(int));
	int restore;
    indexpair ret = list_get((void*)0, &restore, 0);
	ASSERT_EQ(NTEST_LIST_GET1, ret.code, NULLPTR);
    ret = list_get(&listobj, (void*)0, 0);
	ASSERT_EQ(NTEST_LIST_GET2, ret.code, NULLPTR);
    ret = list_get(&listobj, &restore, 5);
	ASSERT_EQ(NTEST_LIST_GET3, ret.code, ARGBADRANGE);
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_add(void){
	SET_TEST_SUITE_NAME(LIST_ADD);
	list listobj = list_init(1, sizeof(int));
	
	int t = 5;
    indexpair ret = list_add((void*)0, &t);
	ASSERT_EQ(NTEST_LIST_ADD1, ret.code, NULLPTR);
    ret = list_add(&listobj, (void*)0);
	ASSERT_EQ(NTEST_LIST_ADD2, ret.code, NULLPTR);
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_addrange1(void){
	SET_TEST_SUITE_NAME(LIST_ADDRANGE1);
	list listobj = list_init(1, sizeof(int));
	
	int arr[3] = {0};
	ASSERT_TRUE(NTEST_LIST_ADDRANGE1_1, list_addrange1((void*)0, &arr, 3));
	ASSERT_TRUE(NTEST_LIST_ADDRANGE1_2, list_addrange1(&listobj, (void*)0, 3));
	ASSERT_TRUE(NTEST_LIST_ADDRANGE1_3, list_addrange1(&listobj, &arr, 0));
	CHECK_ALLOCATED
	return 0;
}

int ntest_list_addrange2(void){
    SET_TEST_SUITE_NAME(LIST_ADDRANGE2);
    struct list listobj, arr;
    
    ASSERT_TRUE(NTEST_LIST_ADDRANGE2_1, list_addrange2((void*)0, &arr));
    ASSERT_TRUE(NTEST_LIST_ADDRANGE2_2, list_addrange2(&listobj, (void*)0));
	return 0;
}

int ntest_list_clear(void){
	SET_TEST_SUITE_NAME(LIST_CLEAR);
	ASSERT_TRUE(NTEST_LIST_CLEAR, list_clear((void*)0));
	return 0;
}

int ntest_list_contains(void){
	SET_TEST_SUITE_NAME(LIST_CONTAINS);
	ASSERT_TRUE(NTEST_LIST_CONTAINS1, list_contains(0, (void*)1));
	ASSERT_TRUE(NTEST_LIST_CONTAINS2, list_contains((void*)1, 0));
	return 0;
}

int ntest_list_exists(void){
	SET_TEST_SUITE_NAME(LIST_EXISTS);
	ASSERT_TRUE(NTEST_LIST_EXISTS1, list_exists(0, (void*)1, (void*)1));
	ASSERT_TRUE(NTEST_LIST_EXISTS2, list_exists((void*)1, 0, (void*)1));
	ASSERT_TRUE(NTEST_LIST_EXISTS3, list_exists((void*)1, (void*)1, 0));
	return 0;
}

int ntest_list_find(void){
	SET_TEST_SUITE_NAME(LIST_FIND);
	list listobj = list_init(1, sizeof(int));
	
	int find = 5, restore = 6;
	ASSERT_TRUE(NTEST_LIST_FIND1, list_find(0, &find, pcomparer, &restore));
	ASSERT_TRUE(NTEST_LIST_FIND2, list_find(&listobj, 0, pcomparer, &restore));
	ASSERT_TRUE(NTEST_LIST_FIND3, list_find(&listobj, &find, 0, &restore));
	ASSERT_TRUE(NTEST_LIST_FIND4, list_find(&listobj, &find, pcomparer, 0));
	ASSERT_TRUE(NTEST_LIST_FIND5, list_find(&listobj, &find, pcomparer, &restore));
	restore = 10;
	return 0;
}

int ntest_list_findindex1(void){
	SET_TEST_SUITE_NAME(LIST_FINDINDEX1);
    list listobj = list_init(1, sizeof(int));
    
    int find = 5;
	ASSERT_FALSE(NTEST_LIST_FINDINDEX1_1, list_findindex1(0, 0, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX1_1, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX1_2, list_findindex1(&listobj, 0, 0, 0, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX1_2, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX1_3, list_findindex1(&listobj, 0, 0, pcomparer, 0));
	ASSERT_EQ(NTEST_LIST_FINDINDEX1_3, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX1_4, list_findindex1(&listobj, 5, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX1_4, errno, ARGBADRANGE);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX1_5, list_findindex1(&listobj, 5, 8, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX1_5, errno, ARGBADRANGE);
	return 0;
}

int ntest_list_findindex2(void){
	SET_TEST_SUITE_NAME(LIST_FINDINDEX2);
    list listobj = list_init(1, sizeof(int));
    
    int find = 5;
	ASSERT_FALSE(NTEST_LIST_FINDINDEX2_1, list_findindex2(0, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX2_1, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX2_2, list_findindex2(&listobj, 0, 0, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX2_2, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX2_3, list_findindex2(&listobj, 0, pcomparer, 0));
	ASSERT_EQ(NTEST_LIST_FINDINDEX2_3, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX2_4, list_findindex2(&listobj, 5, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX2_4, errno, ARGBADRANGE);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX2_5, list_findindex2(&listobj, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX2_5, errno, ITEMNOTFOUND);
	return 0;
}

int ntest_list_findindex3(void){
	SET_TEST_SUITE_NAME(LIST_FINDINDEX3);
    list listobj = list_init(1, sizeof(int));
    
    int find = 5;
	ASSERT_FALSE(NTEST_LIST_FINDINDEX3_1, list_findindex3(0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX3_1, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX3_2, list_findindex3(&listobj, 0, &find));
	ASSERT_EQ(NTEST_LIST_FINDINDEX3_2, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDINDEX3_3, list_findindex3(&listobj, pcomparer, 0));
	ASSERT_EQ(NTEST_LIST_FINDINDEX3_3, errno, NULLPTR);
	return 0;
}

int ntest_list_findlast(void){
	SET_TEST_SUITE_NAME(LIST_FINDLAST);
	list listobj = list_init(1, sizeof(int));
	
	int find = 5, restore = 6;
	ASSERT_TRUE(NTEST_LIST_FIND1, list_findlast(0, &find, pcomparer, &restore));
	ASSERT_TRUE(NTEST_LIST_FIND2, list_findlast(&listobj, 0, pcomparer, &restore));
	ASSERT_TRUE(NTEST_LIST_FIND3, list_findlast(&listobj, &find, 0, &restore));
	ASSERT_TRUE(NTEST_LIST_FIND4, list_findlast(&listobj, &find, pcomparer, 0));
	ASSERT_TRUE(NTEST_LIST_FIND5, list_findlast(&listobj, &find, pcomparer, &restore));
	restore = 10;
	return 0;
}

int ntest_list_findlastindex1(void){
	SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX1);
    list listobj = list_init(1, sizeof(int));
    
    int find = 5;
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX1_1, list_findlastindex1(0, 0, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX1_1, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX1_2, list_findlastindex1(&listobj, 0, 0, 0, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX1_2, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX1_3, list_findlastindex1(&listobj, 0, 0, pcomparer, 0));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX1_3, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX1_4, list_findlastindex1(&listobj, 5, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX1_4, errno, ARGBADRANGE);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX1_5, list_findlastindex1(&listobj, 5, 8, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX1_5, errno, ARGBADRANGE);
	return 0;
}

int ntest_list_findlastindex2(void){
	SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX2);
    list listobj = list_init(1, sizeof(int));
    
    int find = 5;
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX2_1, list_findlastindex2(0, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX2_1, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX2_2, list_findlastindex2(&listobj, 0, 0, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX2_2, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX2_3, list_findlastindex2(&listobj, 0, pcomparer, 0));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX2_3, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX2_4, list_findlastindex2(&listobj, 5, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX2_4, errno, ARGBADRANGE);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX2_5, list_findlastindex2(&listobj, 0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX2_5, errno, ITEMNOTFOUND);
	return 0;
}

int ntest_list_findlastindex3(void){
	SET_TEST_SUITE_NAME(LIST_FINDLASTINDEX3);
    list listobj = list_init(1, sizeof(int));
    
    int find = 5;
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX3_1, list_findlastindex3(0, pcomparer, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX3_1, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX3_2, list_findlastindex3(&listobj, 0, &find));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX3_2, errno, NULLPTR);
	ASSERT_FALSE(NTEST_LIST_FINDLASTINDEX3_3, list_findlastindex3(&listobj, pcomparer, 0));
	ASSERT_EQ(NTEST_LIST_FINDLASTINDEX3_3, errno, NULLPTR);
	return 0;
}

int ntest_list_foreach(void){
    SET_TEST_SUITE_NAME(LIST_FOREACH);
    list listobj = list_init(1, sizeof(int));
    
    ASSERT_FALSE(NTEST_LIST_FOREACH1, list_foreach(0, paction));
    ASSERT_FALSE(NTEST_LIST_FOREACH2, list_foreach(&listobj, 0));
    return 0;
}

int ntest_list_indexof1(void){
    SET_TEST_SUITE_NAME(LIST_INDEXOF1);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_FALSE(NTEST_LIST_INDEXOF1_1, list_indexof1(0, &item));
    ASSERT_FALSE(NTEST_LIST_INDEXOF1_2, list_indexof1(&listobj, 0));
    return 0;
}

int ntest_list_indexof2(void){
    SET_TEST_SUITE_NAME(LIST_INDEXOF2);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_FALSE(NTEST_LIST_INDEXOF2_1, list_indexof2(0, &item, 0));
    ASSERT_FALSE(NTEST_LIST_INDEXOF2_2, list_indexof2(&listobj, 0, 0));
    ASSERT_FALSE(NTEST_LIST_INDEXOF2_3, list_indexof2(&listobj, &item, 5));
    ASSERT_EQ(NTEST_LIST_INDEXOF2_4, errno, ARGBADRANGE);
    return 0;
}

int ntest_list_indexof3(void){
    SET_TEST_SUITE_NAME(LIST_INDEXOF3);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_FALSE(NTEST_LIST_INDEXOF3_1, list_indexof3(0, &item, 0, 0));
    ASSERT_FALSE(NTEST_LIST_INDEXOF3_2, list_indexof3(&listobj, 0, 0, 0));
    ASSERT_FALSE(NTEST_LIST_INDEXOF3_3, list_indexof3(&listobj, &item, 10, 5));
    ASSERT_FALSE(NTEST_LIST_INDEXOF3_4, list_indexof3(&listobj, &item, 5, 10));
    ASSERT_EQ(NTEST_LIST_INDEXOF3_5, errno, ARGBADRANGE);
    return 0;
}

int ntest_list_insert(void){
    SET_TEST_SUITE_NAME(LIST_INSERT);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_EQ(NTEST_LIST_INSERT1, list_insert(0, &item, 0), NULLPTR);
    ASSERT_EQ(NTEST_LIST_INSERT2, list_insert(&listobj, 0, 0), NULLPTR);
    ASSERT_EQ(NTEST_LIST_INSERT3, list_insert(&listobj, &item, 5), 0);
    ASSERT_EQ(NTEST_LIST_INSERT4, errno, ARGBADRANGE);
    return 0;
}

int ntest_list_lastindexof1(void){
    SET_TEST_SUITE_NAME(LIST_LASTINDEXOF1);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF1_1, list_lastindexof1(0, &item));
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF1_2, list_lastindexof1(&listobj, 0));
    return 0;
}

int ntest_list_lastindexof2(void){
    SET_TEST_SUITE_NAME(LIST_LASTINDEXOF2);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF2_1, list_lastindexof2(0, &item, 0));
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF2_2, list_lastindexof2(&listobj, 0, 0));
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF2_3, list_lastindexof2(&listobj, &item, 5));
    ASSERT_EQ(NTEST_LIST_LASTINDEXOF2_4, errno, ARGBADRANGE);
    return 0;
}

int ntest_list_lastindexof3(void){
    SET_TEST_SUITE_NAME(LIST_LASTINDEXOF3);
    list listobj = list_init(1, sizeof(int));
    int item = 5;
    
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF3_1, list_lastindexof3(0, &item, 0, 0));
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF3_2, list_lastindexof3(&listobj, 0, 0, 0));
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF3_3, list_lastindexof3(&listobj, &item, 10, 5));
    ASSERT_FALSE(NTEST_LIST_LASTINDEXOF3_4, list_lastindexof3(&listobj, &item, 5, 10));
    ASSERT_EQ(NTEST_LIST_LASTINDEXOF3_5, errno, ARGBADRANGE);
    return 0;
}

int ntest_list_remove(void){
    SET_TEST_SUITE_NAME(LIST_REMOVE);
    list listobj = list_init(1, sizeof(int));
    int find = 5;
    ASSERT_FALSE(NTEST_LIST_REMOVE1, list_remove(0, &find));
    ASSERT_FALSE(NTEST_LIST_REMOVE2, list_remove(&listobj, 0));
    return 0;
}

int ntest_list_removeall(void){
    SET_TEST_SUITE_NAME(LIST_REMOVEALL);
    list listobj = list_init(1, sizeof(int));
    int find = 5;
    ASSERT_FALSE(NTEST_LIST_REMOVEALL1, list_removeall(0, &find, pcomparer));
    ASSERT_FALSE(NTEST_LIST_REMOVEALL2, list_removeall(&listobj, 0, pcomparer));
    ASSERT_FALSE(NTEST_LIST_REMOVEALL3, list_removeall(&listobj, &find, 0));
    return 0;
}

int ntest_list_removeat(void){
    SET_TEST_SUITE_NAME(LIST_REMOVEAT);
    list listobj = list_init(1, sizeof(int));
    ASSERT_FALSE(NTEST_LIST_REMOVEAT1, list_removeat(0, 0));
    ASSERT_FALSE(NTEST_LIST_REMOVEAT2, list_removeat(&listobj, 5));
    return 0;
}

int ntest_list_reverse(void){
    SET_TEST_SUITE_NAME(LIST_REVERSE);
    struct list listobj = list_init(1, sizeof(int32_t));
    ASSERT_FALSE(NTEST_LIST_REVERSE1, list_reverse(0));
    ASSERT_FALSE(NTEST_LIST_REVERSE2, list_reverse(&listobj));
    return 0;
}

test tests[] = {
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
    ntest_list_reverse
};

int test_count = 58;

void test_main(void){
    for (int i = 0; i < test_count; i++){
        tests[i]();
    }
}
