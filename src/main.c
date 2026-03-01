#include "lexer.h"
#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 2047

int main(const int argc, const char **argv) {
    char input_buffer[BUFFERSIZE] = {'\0'};
    int running = 1;
    while (running) {
        fputs("> ", stdout);
        if (NULL == fgets(input_buffer, BUFFERSIZE, stdin)) {
            break;
        }
        if (strcmp(input_buffer, "(exit)\n") == 0) {
            running = 0;
            continue;
        }
        token *token_array = lexerer(input_buffer);
        print_tokens(token_array);
        free_token_array(token_array);
    }
    fputs("exiting...\n", stdout);
    return 0;
}
