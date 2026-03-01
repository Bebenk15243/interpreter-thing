#include <stdio.h>
#include <stdlib.h>

enum tokens_ids { LPAR, RPAR, VAR, NUMBER, EOF_ };

typedef struct {
    int token_id;
    int *value;
} token;

// bufferspace
//  allocates space to give the tokens back
token *lexerer(char *);
void print_tokens(token *);
void free_token_array(token *);
