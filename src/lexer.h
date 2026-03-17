#ifndef LEXER_H_
#define LEXER_H_
#include "debug.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// applications cannot be detected by a lexer.
// NOTE:          0     1     2    3       4      5        6   7   8
enum tokens_ids { LPAR, RPAR, VAR, NUMBER, LAMBDA, DEFINE, IF, APP, EOF_ };
typedef struct {
    int token_id;
    void *value;
} token;

// bufferspace
//  allocates space to give the tokens back
token *lexerer(char *);
void print_tokens(token *);
void free_token_array(token *);

#endif
