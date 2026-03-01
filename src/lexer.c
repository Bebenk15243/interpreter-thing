#include "lexer.h"
#include <string.h>

#define MAX_AMOUNT_TOKENS 16

token *lexerer(char *buffer) {
    // allocation token list to give later back to parser
    token *token_array = (token *)calloc(MAX_AMOUNT_TOKENS, sizeof(token));
    if (token_array == NULL) {
        printf("error, no memory");
        exit(1);
    }

    int token_index = 0;
    // index in our buffer to see what we have
    int buffer_index = 0;
    // marks the begin of the new token
    int begin_new_token = 0;

    // maybe implement a propper dfa
    while (buffer[buffer_index] != '\0') {
        switch (buffer[buffer_index]) {
        case '(':
            token_array[token_index++] = (token){LPAR, NULL};
            begin_new_token = token_index;
            break;
        case ')':
            token_array[token_index++] = (token){RPAR, NULL};
            begin_new_token = token_index;
            break;
        }
        buffer_index += 1;
    }
    token_array[token_index++] = (token){EOF_, NULL};
    return token_array;
}

void print_tokens(token *token_array) {
    int idx = -1;
    while (token_array[++idx].token_id != EOF_) {
        fprintf(stdout, "this is an %d with value %p\n",
                token_array[idx].token_id, token_array[idx].value);
    }
}

void free_token_array(token *token_array) {
    for (int idx = 0; idx < MAX_AMOUNT_TOKENS; idx++) {
        if (NULL != token_array[idx].value)
            free(token_array[idx].value);
    }
    free(token_array);
}
