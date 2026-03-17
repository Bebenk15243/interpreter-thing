#include "parser.h"
#include "debug.h"

#define WARN_FLAG 1
#define DEBUG_FLAG 1

lambda_exp *parse_lamdba(int *index, token *tokenarray) {
    lambda_exp *lambda = (lambda_exp *)malloc(sizeof(lambda_exp));
    // for paremeter parsing but needs a parenthesis
    if (tokenarray[(*index)++].token_id != LPAR) {
        PRINT_ERROR("parser: lambda where left parenthesis for parameter");
        exit(1);
    }

    // see if there are parameters?
    if (tokenarray[(*index)++].token_id == RPAR) {
        PRINT_DEBUG("ITS A THUNNNKKK");
        lambda->parameters = NULL;
        (*index)++;
    } else {
        // parse the parameters
        //  first counting the parameters
        PRINT_DEBUG("checking parameters");
        int length = 0;
        for (int param_index = *index; tokenarray[param_index].token_id != RPAR;
             param_index++) {
            if (tokenarray[param_index].token_id != VAR) {
                PRINT_ERROR("parser: cant parse a non-variable");
                exit(1);
            }
            length++;
        }
        lambda->parameters = (var_exp *)calloc(length, sizeof(var_exp));
        for (int param_index = 0; param_index < length; param_index++) {
            lambda->parameters[param_index].name =
                tokenarray[param_index].value;
        }
        PRINT_DEBUG("parameters are checked and assigned");
    }
    lambda->expression = parse(index, tokenarray); // parse the expression part
    return lambda;
}

expr_t *parse(int *index, token *tokenarray) {
    PRINT_DEBUG("started parsing");
    expr_t *exp = (expr_t *)malloc(sizeof(expr_t));
    while (tokenarray[*index].token_id != EOF_) {
        PRINT_DEBUG("in loop");
        switch (tokenarray[*index].token_id) {
        case LPAR:
            PRINT_DEBUG("LPAR parsed");
            *index += 1;
            if (tokenarray[*index].token_id == LAMBDA) {
                PRINT_DEBUG("found lambda");
                exp->type = LAMBDA;
                exp->expr = parse_lamdba(index, tokenarray);
                break;
            }
        }
        (*index)++;
    }
    return (void *)tokenarray;
}
