#pragma once
#include "positive_tests.h"
#define DEBUG if (retcode){ write_failure("[FAIL]", retcode); exit(-1);} else write_success("[ OK ]");

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef int (*test)(void);