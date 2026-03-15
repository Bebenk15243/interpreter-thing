#include "debug.h"

flag_types flags = {.debug = 0, .error = 1, .warning = 0};

void print_debug(char *str) {
    if (flags.debug)
        printf("DEBUG: %s\n", str);
}

void print_warning(char *str) {
    if (flags.warning)
        printf("WARNING: %s\n", str);
}

void print_error(char *str) {
    if (flags.error)
        printf("ERROR: %s\n", str);
}

void set_flag(char debug, char error, char warning) {
    flags.debug = debug;
    flags.error = error;
    flags.warning = warning;
}
