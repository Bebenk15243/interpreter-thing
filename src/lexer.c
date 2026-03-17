#include "lexer.h"
#include "debug.h"
#include <ctype.h>
#include <string.h>

#define MAX_AMOUNT_TOKENS 2048
#define MAX_VARIABLE_LENGTH 16
#define DEBUG_FLAG 0
#define WARN_FLAG 1

// this function checks wether the string between begin and end index could be a
// variable doestn check end_index
int is_variable(char *buffer, int begin_index, int end_index) {
    // this is done by eliminating what is not valid.
    // a variable cannot start with a space or a digit
    if (buffer[begin_index] == ' ' && isdigit(buffer[begin_index]))
        return 0;

    // there may not be spaces or parenthesis in the rest of the characters.
    for (int index = begin_index + 1; index < end_index; index++) {
        if (buffer[index] == '(' || buffer[index] == ')' ||
            buffer[index] == ' ') //  || buffer[index] == '')
            return 0;
    }
    return 1;
}

// determines the special forms of the language
int determine_token_special_token(char *value) {
    if (!strcmp(value, "lambda"))
        return LAMBDA;
    else if (!strcmp(value, "define"))
        return DEFINE;
    else if (!strcmp(value, "if"))
        return IF;
    else
        return VAR;
}

// push token variable that is before the buffer_index in buffer and starts on
//  begin_new_token. This is pushed to the token_array which has a free space at
//  token_index.
//  returns 1 if the tokenarray has to be incremebted.
int push_variable_token(char *buffer, token *token_array, int buffer_index,
                        int begin_new_token, int token_index) {
    // check if index is same, if so, no variable should be lexed.
    if (buffer_index != begin_new_token) {
        // check if variable between beginidx and index, not including
        //  the index!!
        if (is_variable(buffer, begin_new_token, buffer_index)) {
            // if variable, allocate space for the value
            char *value =
                (char *)calloc(buffer_index - begin_new_token, sizeof(char));
            if (value == NULL) {
                PRINT_ERROR("error, no memory");
                exit(1);
            }
            // copy the value from the buffer into the tokenarray
            //  dest, src, length
            memcpy(value, buffer + begin_new_token,
                   buffer_index - begin_new_token);
            token_array[token_index] =
                (token){determine_token_special_token(value), value};
            // debug statement
            PRINT_DEBUG("found value: %s", value);

            return 1;
        } else {
            PRINT_WARN("lexing: weird thing that shouldnt be reachable");
            return 0;
        }
    } else {
        return 0;
    }
}

token *lexerer(char *buffer) {
    // allocation token list to give later back to parser
    token *token_array = (token *)calloc(MAX_AMOUNT_TOKENS, sizeof(token));
    if (token_array == NULL) {
        PRINT_ERROR("error, no memory");
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
        case ')':
            token_index +=
                push_variable_token(buffer, token_array, buffer_index,
                                    begin_new_token, token_index);

            // check for parenthesis and log right token
            if (buffer[buffer_index] == '(') {
                token_array[token_index++] = (token){LPAR, NULL};
                PRINT_DEBUG("found token: [LPAR]");
            } else if (buffer[buffer_index] == ')') {
                token_array[token_index++] = (token){RPAR, NULL};
                PRINT_DEBUG("found token: [RPAR]");
            } else {
                PRINT_ERROR(
                    "lexing error, parenthesis expected but doesnt match");
                exit(1);
            }
            // update the begin token, all tokens including the bufferindex
            // are
            //  processed...
            begin_new_token = buffer_index + 1;
            break;

        case ' ':
            push_variable_token(buffer, token_array, buffer_index,
                                begin_new_token, token_index);
            token_index++;
            begin_new_token = buffer_index + 1;
            break;
        default:
            token_index;
        }
        // update loop
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
