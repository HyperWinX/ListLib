#pragma once
#include "positive_tests.h"
#include "negative_tests.h"
#define POST_DEBUG printf("\tPositive: "); if (retcode){write_failure("[FAIL]", retcode); exit(-1);} else write_success("[ OK ]");
#define NEGT_DEBUG printf("\tNegative: "); if (retcode){write_failure("[FAIL]", retcode); exit(-1);} else write_success("[ OK ]");

extern jmp_buf sigsegv_restore;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef int (*test)(void);