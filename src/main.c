#include "lexer.h"
#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 2047

void args_handle(const int argc, const char **argv, char **file_name) {
    if (argc == 1) {
        return;
    }
    printf("you got: %d args\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'f':
            case 'F':
                *file_name = argv[i + 1];
                printf("%s\n", *file_name);
                break;

            default:
                printf("I DONT KNOW %s", argv[i]);
            }
        }
    }
}

int main(const int argc, const char **argv) {
    char *file_name = NULL; // NUlL to detect when -f flag is found
    args_handle(argc, argv, &file_name);

    // input?
    char input_buffer[BUFFERSIZE] = {'\0'};
    if (file_name != NULL) {
        FILE *file = fopen(file_name, "r");
        if (file == NULL) {
            printf("error while opening %s\n", file_name);
            exit(1);
        }

        // reads in the code from the given file
        //  works by reading character by character
        //  if nothing new is read, the loop breaks;
        int old_idx, idx = 0;
        do {
            old_idx = idx;
            fscanf(file, "%c", input_buffer + idx);
            if (input_buffer[idx] == '\n')
                input_buffer[idx] = ' ';
            idx = strlen(input_buffer);
        } while (old_idx != idx);

        token *token_array = lexerer(input_buffer);
        print_tokens(token_array);
        free_token_array(token_array);
        return 0;

    } else {
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
}
