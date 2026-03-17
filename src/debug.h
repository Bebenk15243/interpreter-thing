//*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;
//*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;
//*;*;                                                        *;*;
//*;*;              ... for easier debugging ...              *;*;
//*;*;                     Pepijn Simoens                     *;*;
//*;*;                       March 2026                       *;*;
//*;*;                        debug.h                         *;*;
//*;*;                                                        *;*;
//*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;
//*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;

#ifndef DEBUG_H_
#define DEBUG_H_
#include <stdio.h>

// this code expects DEBUG_FLAG and WARN_FLAG defined (either 1 or 0)
// are defined for obvious reasons

// turns the text bold and prepends [debug]
#define PRINT_DEBUG(args...)                                                   \
    while (DEBUG_FLAG) {                                                       \
        printf("\e[1m[DEBUG]: ");                                              \
        printf(args);                                                          \
        printf("\e[0m\n");                                                     \
        break;                                                                 \
    }

// bold yellow and prepends [WARN]
#define PRINT_WARN(args...)                                                    \
    while (WARN_FLAG) {                                                        \
        printf("\e[1m\e[33m[WARN] : ");                                        \
        printf(args);                                                          \
        printf("\e[0m\n");                                                     \
        break;                                                                 \
    }

// bold red and prepends [ERROR]
// this has no flag because an error should always be visible
#define PRINT_ERROR(args...)                                                   \
    do {                                                                       \
        fprintf(stderr, "\e[1m\e[31m[ERROR]: ");                               \
        fprintf(stderr, args);                                                 \
        fprintf(stderr, "\e[0m\n");                                            \
    } while (0)

#endif
