#define DEBUG if (!retcode){ write_failure("[FAIL]: %d\n", retcode); exit(-1);} else write_success("[ OK ]\n");

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef int (*test)(void);