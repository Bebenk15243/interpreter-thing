#ifndef PARSER_H_
#define PARSER_H_
#include "debug.h"
#include "lexer.h"

typedef struct {
    void *expr;
    int type;
} expr_t;

typedef struct {
    int num;
} num_exp;

typedef struct {
    char *name;
} var_exp;

typedef struct {
    expr_t *condition;
    expr_t *consequent;
    expr_t *alternative;
} if_exp;

// (lambda (<pars>) <expr>)
typedef struct {
    int parameter_count;
    var_exp *parameters;
    expr_t *expression;
} lambda_exp;

// NOTE: dont need this for now...
/*
typedef struct {
    var_exp name;
    lambda_exp lambda;
    int type;
} define_exp;
*/

typedef struct {
    expr_t *procedure;
    int arg_count;
    expr_t **args;
} application_exp;

expr_t *parse(int *index, token *tokenarray);
void free_dispatch(expr_t *expr);

#endif
