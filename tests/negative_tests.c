#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include "tests.h"
#include "../list.h"

#define CHECK_ALLOCATED if (listobj.ptr != NULL) free(listobj.ptr);
#define NDEBUG if(retcode==NOERR){CHECK_ALLOCATED return 1;}
#define ERRNDEBUG if(errno==NOERR){CHECK_ALLOCATED return 1;}

void sigsegv_handler(int s){
    switch(s){
        case SIGSEGV:
            longjmp(sigsegv_restore, 1);
            break;
    }
}

int ncomparer(void* ptr1, void* ptr2){
    if (*((int*)ptr1) != *((int*)ptr2)) return 1;
    else return 0;
}

void naction(void* ptr){
    int t = *(int*)ptr + 1;
}

int ntest_list_init(void){
    struct list listobj = {0};
    int retcode = 0;
    retcode = list_init(NULL, 256, sizeof(int32_t));
    NDEBUG
    retcode = list_init(&listobj, 0, sizeof(int32_t));
    NDEBUG
    retcode = list_init(&listobj, 256, 0);
    NDEBUG
    return 0;
}

int ntest_list_destroy(void){
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int retcode = list_destroy(NULL);
    NDEBUG
    return 0;
}

int ntest_list_set(void){
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int32_t element = 871623;
    int retcode = list_set(NULL, &element, 0);
    NDEBUG
    retcode = list_set(&listobj, NULL, 0);
    NDEBUG
    retcode = list_set(&listobj, &element, 8996888);
    NDEBUG
    return 0;
}

int ntest_list_get(void){
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int32_t element = 871623;
    int retcode = list_get(NULL, &element, 0);
    NDEBUG
    retcode = list_get(&listobj, NULL, 0);
    NDEBUG
    retcode = list_get(&listobj, &element, 8996888);
    NDEBUG
    return 0;
}

int ntest_list_add(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t tmp = 123094;
    int32_t element = 348907;
    int retcode = list_add(NULL, &tmp);
    NDEBUG
    retcode = list_add(&listobj, NULL);
    NDEBUG
    return 0;
}

int ntest_list_addrange1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    int retcode = list_addrange1(NULL, &arr, 4);
    NDEBUG
    retcode = list_addrange1(&listobj, NULL, 4);
    NDEBUG
    retcode = list_addrange1(&listobj, &arr, 0);
    NDEBUG
    return 0;
}

int ntest_list_addrange2(void){
    struct list listobj = {0};
    struct list addition = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int retcode = list_addrange2(NULL, &addition);
    NDEBUG
    retcode = list_addrange2(&listobj, NULL);
    NDEBUG
    return 0;
}

int ntest_list_clear(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int retcode = list_clear(NULL);
    NDEBUG
    return 0;
}

int ntest_list_contains(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 1;
    signed int retcode = list_contains(NULL, &find);
    NDEBUG
    retcode = list_contains(&listobj, NULL);
    NDEBUG
    return 0;
}

int ntest_list_exists(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    signed int retcode = list_exists(NULL, ncomparer, &arr[1]);
    NDEBUG
    retcode = list_exists(&listobj, NULL, &arr[1]);
    NDEBUG
    retcode = list_exists(&listobj, ncomparer, NULL);
    NDEBUG
    return 0;
}

int ntest_list_find(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 8675;
    int32_t restored = 0;
    uint32_t retcode = list_find(NULL, &find, ncomparer, &restored);
    NDEBUG
    retcode = list_find(&listobj, NULL, ncomparer, &restored);
    NDEBUG
    retcode = list_find(&listobj, &find, NULL, &restored);
    NDEBUG
    retcode = list_find(&listobj, &find, ncomparer, NULL);
    NDEBUG
    return 0;
}

int ntest_list_findindex1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 8675;
    errno = 0;
    uint32_t retcode = list_findindex1(NULL, 2, 7, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findindex1(&listobj, 0, 7, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findindex1(&listobj, 2, 0, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findindex1(&listobj, 2, 7, NULL, &find);
    ERRNDEBUG
    retcode = list_findindex1(&listobj, 2, 7, ncomparer, NULL);
    ERRNDEBUG
    return 0;
}

int ntest_list_findindex2(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 8675;
    errno = 0;
    uint32_t retcode = list_findindex2(NULL, 4, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findindex2(&listobj, 0, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findindex2(&listobj, 2, NULL, &find);
    ERRNDEBUG
    retcode = list_findindex2(&listobj, 2, ncomparer, NULL);
    ERRNDEBUG
    return 0;
}

int ntest_list_findindex3(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 8675;
    errno = 0;
    uint32_t retcode = list_findindex3(NULL, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findindex3(&listobj, NULL, &find);
    ERRNDEBUG
    retcode = list_findindex3(&listobj, ncomparer, NULL);
    ERRNDEBUG
    return 0;
}

int ntest_list_findlast(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 8;
    int32_t restored = 0;
    uint32_t retcode = list_findlast(NULL, &find, &restored, ncomparer);
    NDEBUG
    retcode = list_findlast(&listobj, NULL, &restored, ncomparer);
    NDEBUG
    retcode = list_findlast(&listobj, &find, NULL, ncomparer);
    NDEBUG
    retcode = list_findlast(&listobj, &find, &restored, NULL);
    NDEBUG
    return 0;
}

int ntest_list_findlastindex1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t find = 5;
    uint32_t retcode = list_findlastindex1(NULL, 2, 6, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findlastindex1(&listobj, 0, 6, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findlastindex1(&listobj, 2, 0, ncomparer, &find);
    ERRNDEBUG
    retcode = list_findlastindex1(&listobj, 2, 6, NULL, &find);
    ERRNDEBUG
    retcode = list_findlastindex1(&listobj, 2, 6, ncomparer, NULL);
    ERRNDEBUG
    return 0;
}

int ntest_list_findlastindex2(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    uint32_t retcode = list_findlastindex2(&listobj, 2, ncomparer, &find);
    CHECK_ALLOCATED
    if (retcode == 5) return 0;
    if (retcode == -1) return errno;
}

int ntest_list_findlastindex3(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    uint32_t retcode = list_findlastindex3(&listobj, ncomparer, &find);
    CHECK_ALLOCATED
    errno = 0;
    if (retcode == 5) return 0;
    if (retcode == -1) return errno;
}

int ntest_list_foreach(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    list_foreach(&listobj, naction);
}

int ntest_list_indexof1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    errno = 0;
    uint32_t retcode = list_indexof1(&listobj, &find);
    if (retcode != 3 && errno != 0) return 1;
    else return 0;
}

int ntest_list_indexof2(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    errno = 0;
    uint32_t retcode = list_indexof2(&listobj, &find, 2);
    if (retcode != 3 && errno != 0) return 1;
    else return 0;
}

int ntest_list_indexof3(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    errno = 0;
    uint32_t retcode = list_indexof3(&listobj, &find, 2, 5);
    if (retcode != 3 && errno != 0) return 1;
    else return 0;
}

int ntest_list_insert(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
}

int ntest_list_lastindexof1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    errno = 0;
    uint32_t retcode = list_lastindexof1(&listobj, &find);
    if (retcode != 5 && errno != 0) return 1;
    else return 0;
}

int ntest_list_lastindexof2(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    errno = 0;
    uint32_t retcode = list_lastindexof2(&listobj, &find, 4);
    if (retcode != 5 && errno != 0) return 1;
    else return 0;
}

int ntest_list_lastindexof3(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    errno = 0;
    uint32_t retcode = list_lastindexof3(&listobj, &find, 4, 6);
    if (retcode != 5 && errno != 0) return 1;
    else return 0;
}

int ntest_list_remove(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    signed int retcode = list_remove(&listobj, &find);
    int check = 0;
    list_get(&listobj, &check, 3);
    if (check != find) return 0;
    else return 1;
}

int ntest_list_removeall(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 5;
    uint32_t retcode = list_removeall(&listobj, &find, ncomparer);
    if (retcode != 2) return 1;
    else return 0;
}

int ntest_list_removeat(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    signed int retcode = list_removeat(&listobj, 3);
    int32_t check = 0;
    list_get(&listobj, &check, 3);
    if (check != 5) return 0;
    else return 1;
}

int ntest_list_reverse(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 5, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int retcode = list_reverse(&listobj);
    for (int i = 0; i < 8; i++){
        int check = 0;
        list_get(&listobj, &check, 7 - i);
        if (check != arr[i]) return 1;
    }
    return 0;
}