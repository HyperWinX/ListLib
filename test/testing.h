#pragma once

#define CHECK_ALLOCATED if (listobj.ptr) free(listobj.ptr);

extern jmp_buf sigsegv_restore;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef int (*test)(void);
