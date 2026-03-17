#include "parser.h"
#include "debug.h"

#define WARN_FLAG 1
#define DEBUG_FLAG 1

lambda_exp *parse_lamdba(int *index, token *tokenarray) {
    lambda_exp *lambda = (lambda_exp *)malloc(sizeof(lambda_exp));
    // for paremeter parsing but needs a parenthesis
    (*index)++;
    if (tokenarray[(*index)++].token_id != LPAR) {
        PRINT_ERROR("parser: lambda where left parenthesis for parameter");
        exit(1);
    }

    // see if there are parameters?
    if (tokenarray[(*index)].token_id == RPAR) {
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
        PRINT_DEBUG("found %d parameters: ", length);
        for (int param_index = 0; param_index < length; param_index++) {
            lambda->parameters[param_index].name =
                tokenarray[param_index + *index].value;
            PRINT_DEBUG("parameter %d: %s", param_index + 1,
                        lambda->parameters[param_index].name);
        }
        PRINT_DEBUG("parameters are checked and assigned");
    }
    PRINT_WARN("program stops because its not ready yet");
    exit(0);
    lambda->expression = parse(index, tokenarray); // parse the expression part

    return lambda;
}

if_exp *parse_if(int *index, token *tokenarray) {
    if_exp *if_ = (if_exp *)malloc(sizeof(if_exp));
    (*index)++;
    if_->condition = parse(index, tokenarray);
    if_->consequent = parse(index, tokenarray);
    if_->alternative = parse(index, tokenarray);
    // check RPAR
    return if_;
}

expr_t *parse(int *index, token *tokenarray) {
    PRINT_DEBUG("started parsing");
    expr_t *exp = (expr_t *)malloc(sizeof(expr_t));
    while (tokenarray[*index].token_id != EOF_) {
        switch (tokenarray[*index].token_id) {
        case LPAR:
            PRINT_DEBUG("LPAR parsed");
            *index += 1;
            if (tokenarray[*index].token_id == LAMBDA) {
                PRINT_DEBUG("found lambda");
                exp->type = LAMBDA;
                exp->expr = parse_lamdba(index, tokenarray);
                break;
            } else if (tokenarray[*index].token_id == DEFINE) {
                PRINT_DEBUG("found define");
                break;
            } else if (tokenarray[*index].token_id == IF) {
                PRINT_DEBUG("found if");
                break;
            } else if (tokenarray[*index].token_id == VAR) {
                PRINT_DEBUG("found application");
                break;
            } else {
                PRINT_ERROR(
                    "error in lexer: cant parse this token in parer: %d",
                    tokenarray[*index].token_id);
                exit(1);
            }
        case RPAR:
            PRINT_ERROR("expected an expression");
            exit(1);
        }
        (*index)++;
    }
    return (void *)tokenarray;
}
