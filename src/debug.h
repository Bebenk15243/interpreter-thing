#ifndef DEBUG_H_
#define DEBUG_H_
#include <stdio.h>

typedef struct {
    short debug : 1;
    short warning : 1;
    short error : 1;
} flag_types;

void print_debug(char *str);
void print_warning(char *str);
void print_error(char *str);

void set_flag(char debug, char warning, char error);

#endif
