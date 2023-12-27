#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "../list.h"
#define CHECK_ALLOCATED if (listobj.ptr != NULL) free(listobj.ptr);

int comparer(void* ptr1, void* ptr2){
    if (*((int*)ptr1) != *((int*)ptr2)) return 1;
    else return 0;
}

int test_list_init(void){
    struct list listobj = {0};
    int retcode = 0;
    retcode = list_init(&listobj, 256, sizeof(int32_t));
    CHECK_ALLOCATED
    return retcode;
}

int test_list_destroy(void){
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    return list_destroy(&listobj);
}

int test_list_set(void){
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int32_t element = 871623;
    int retcode = list_set(&listobj, &element, 0);
    if (memcmp(listobj.ptr, &element, sizeof(int32_t))){
        CHECK_ALLOCATED
        return 1;
    } else {
        CHECK_ALLOCATED
        return retcode;
    }
}

int test_list_get(void){
    struct list listobj = {0};
    list_init(&listobj, 256, sizeof(int32_t));
    int32_t element = 871623;
    list_set(&listobj, &element, 0);
    int32_t restored = 0;
    int retcode = list_get(&listobj, &restored, 0);
    CHECK_ALLOCATED
    return retcode;
}

int test_list_add(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t tmp = 123094;
    int32_t element = 348907;
    int retcode = list_add(&listobj, &tmp);
    if (retcode){
        CHECK_ALLOCATED
        return retcode;
    }
    list_add(&listobj, &element);
    if (retcode){
        CHECK_ALLOCATED
        return retcode;
    }
    if (listobj.elementcount != 2 ||
        listobj.listsz != listobj.elementsize*2 ||
        listobj.allocated != listobj.elementsize*5){
            CHECK_ALLOCATED
            return retcode;
    }
    CHECK_ALLOCATED
    return retcode;
}

int test_list_addrange1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    int retcode = list_addrange1(&listobj, &arr, 4);
    if (retcode){
        CHECK_ALLOCATED
        return retcode;
    }
    int32_t restored[4] = {0};
    for (int i = 0; i < 4; i++)
        list_get(&listobj, &(restored[i]), i);
    if (memcmp(&arr, &restored, sizeof(int32_t)*4)){
        CHECK_ALLOCATED
        return 6;
    }
    CHECK_ALLOCATED
    return retcode;
}

int test_list_addrange2(void){
    srand(time(NULL));
    struct list listobj = {0};
    struct list addition = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    list_init(&addition, 4, sizeof(int32_t));
    for (int i = 0; i < 4; i++){
        int32_t num = rand();
        list_add(&addition, &num);
    }
    int retcode = list_addrange2(&listobj, &addition);
    if (retcode){
        CHECK_ALLOCATED
        return retcode;
    }
    if (memcmp(addition.ptr, listobj.ptr, sizeof(int32_t)*4)){
        CHECK_ALLOCATED
        return retcode;
    }
}

int test_list_clear(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    int retcode = list_clear(&listobj);
    if (listobj.elementcount != 0 ||
        listobj.listsz != 0){
            CHECK_ALLOCATED
            return retcode;
    }
    CHECK_ALLOCATED
    return retcode;
}

int test_list_contains(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    int32_t find = 1;
    signed int contains = list_contains(&listobj, &find);
    CHECK_ALLOCATED
    return contains;
}

int test_list_exists(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[4] = {4, 7, 1, 9};
    list_addrange1(&listobj, &arr, 4);
    signed int retcode = list_exists(&listobj, comparer, &arr[1]);
    CHECK_ALLOCATED
    return retcode;
}

int test_list_find(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 8675;
    int32_t restored = 0;
    uint32_t retcode = list_find(&listobj, &find, comparer, &restored);
    CHECK_ALLOCATED
    return retcode;
}

int test_list_findindex1(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 8675;
    uint32_t retcode = list_findindex1(&listobj, 2, 7, comparer, &find);
    CHECK_ALLOCATED
    if (retcode == 0 && errno != 0) return 1;
    else return 0;
}

int test_list_findindex2(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 8675;
    uint32_t retcode = list_findindex2(&listobj, 4, comparer, &find);
    CHECK_ALLOCATED
    if (retcode == 0 && errno != 0) return 1;
    else return 0;
}

int test_list_findindex3(void){
    struct list listobj = {0};
    list_init(&listobj, 1, sizeof(int32_t));
    int32_t arr[8] = {4, 7, 1, 9, 123, 5, 8675, 92478};
    list_addrange1(&listobj, &arr, 8);
    int32_t find = 8675;
    uint32_t retcode = list_findindex3(&listobj, comparer, &find);
    CHECK_ALLOCATED
    if (retcode == 0 && errno != 0) return 1;
    else return 0;
}

int test_list_findlast(void){
    
}

int test_list_findlastindex1(void){

}

int test_list_findlastindex2(void){

}

int test_list_findlastindex3(void){

}

int test_list_foreach(void){

}

int test_list_indexof1(void){

}

int test_list_indexof2(void){

}

int test_list_indexof3(void){

}

int test_list_insert(void){

}

int test_list_lastindexof1(void){

}

int test_list_lastindexof2(void){

}

int test_list_lastindexof3(void){

}

int test_list_remove(void){

}

int test_list_removeall(void){

}

int test_list_removeat(void){

}

int test_list_reverse(void){
    
}