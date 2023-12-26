#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define DEBUG if(retcode != NOERR){write_failure("[FAIL]", retcode); return -1;}else write_success("[ OK ]");

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void write_failure(char* text, int retcode){
    printf(ANSI_COLOR_RED "%s: %d%s\n", text, retcode, ANSI_COLOR_RESET);
}

void write_success(char* text){
    printf(ANSI_COLOR_GREEN "%s%s\n", text, ANSI_COLOR_RESET);
}

int main(){
    struct list listobj;
    int retcode = 0;
    printf("Running list initialization...         ");
    retcode = list_init(&listobj, 1024*1024*256, sizeof(int32_t));
    DEBUG
    printf("Running out of bounds initialization...");
    list_destroy(&listobj);
    retcode = list_init(&listobj, 0, sizeof(int32_t));
    if (retcode != NULLINITSIZE){
        write_failure("[FAIL]\n", retcode);
        return -1;
    }
    retcode = 0;
    retcode = list_init(&listobj, 4, 0);
    if (retcode != NULLINITSIZE){
        write_failure("[FAIL]\n", retcode);
        return -1;
    }
    retcode = 0;
    retcode = list_init(NULL, 4, sizeof(int32_t));
    if (retcode != NULLPTR){
        write_failure("[FAIL]\n", retcode);
        return -1;
    }
    retcode = 0;
    DEBUG
    list_init(&listobj, 1024*1024*256, sizeof(int32_t));
    printf("Running list_add() test...             ");
    for (uint32_t i = 0; i < 1024*1024*256; i++){
        retcode = list_add(&listobj, &i);
        if (retcode != NOERR) DEBUG
    }
    DEBUG
    printf("Running data verification...           ");
    for (uint32_t i = 0; i < 1024*1024*256; i++){
        int tmp = 0;
        retcode = list_get(&listobj, &tmp, i);
        if (retcode != NOERR) DEBUG
        if (tmp != i) {write_failure("[FAIL]\n", tmp);return -1;}
    }
    DEBUG
    printf("Running list_set() test...             ");
    for (uint32_t i = 0; i < 1024*1024*256; i++){
        uint32_t b = i + 1;
        retcode = list_set(&listobj, &b, i);
        if (retcode != NOERR) DEBUG
    }
    DEBUG
    printf("Running list_clear() test...           ");
    retcode = list_clear(&listobj);
    DEBUG
    printf("Running list_addrange1() test...       ");
    int32_t* buf = (int32_t*)calloc(sizeof(int32_t), 40);
    for (int32_t i = 0; i < 40; i++) buf[i] = i;
    retcode = list_addrange1(&listobj, buf, 40);
    DEBUG
    struct list tmp;
    list_init(&tmp, 40, sizeof(uint32_t));
}