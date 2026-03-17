#include "parser.h"
#include "debug.h"

#define WARN_FLAG 1
#define DEBUG_FLAG 1

// WARNING: FIX FREE OF POINTERS AFTER *parse AS THEY ALREADY ALLOCATE MEMORY

void has_RPAR(int *index, token *tokenarray) {
    if (tokenarray[*index].token_id != RPAR) {
        PRINT_ERROR("expected a RPAR");
        exit(1);
    }
    (*index)++;
}
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
        has_RPAR(index, tokenarray);
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
        *index += length;
        PRINT_DEBUG("parameters are checked and assigned");
        has_RPAR(index, tokenarray);
    }
    lambda->expression = *parse(index, tokenarray); // parse the expression part
    has_RPAR(index, tokenarray);
    return lambda;
}

if_exp *parse_if(int *index, token *tokenarray) {
    if_exp *if_ = (if_exp *)malloc(sizeof(if_exp));
    (*index)++;
    expr_t *temp = parse(index, tokenarray);
    if_->condition = *temp;
    free(temp);

    temp = parse(index, tokenarray);
    if_->consequent = *temp;
    free(temp);

    temp = parse(index, tokenarray);
    if_->alternative = *temp;
    free(temp);

    has_RPAR(index, tokenarray);
    return if_;
}

application_exp *parse_application(int *index, token *tokenarray) {
    application_exp *app = (application_exp *)malloc(sizeof(application_exp));
    app->procedure = *parse(index, tokenarray);
    PRINT_DEBUG("found a procedure?: %s",
                ((var_exp *)app->procedure.expr)->name);

    // NOTE: realloc allong the way by multiples of 2
    int args_index = 0;
    // initial allocation (in this case 4)
    app->arg_count = 4; // TODO: make an macro definition for this
    app->args = (expr_t *)calloc(app->arg_count, sizeof(expr_t));
    // start counting
    while (tokenarray[*index].token_id != RPAR) {
        // if there is more memory necessary <- this word is too hard to write
        if (args_index == app->arg_count) {
            app->args = (expr_t *)realloc(app->args, app->arg_count * 2);
            app->arg_count *= 2;
        }
        app->args[args_index] = *parse(index, tokenarray);
        args_index++;
    }
    app->args = (expr_t *)realloc(app->args, args_index);
    app->arg_count = args_index;
    has_RPAR(index, tokenarray);
    return app;
}

expr_t *parse(int *index, token *tokenarray) {
    expr_t *exp = (expr_t *)malloc(sizeof(expr_t));
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
            exp->type = IF;
            exp->expr = parse_if(index, tokenarray);
            break;
        } else if (tokenarray[*index].token_id == VAR) {
            exp->type = APP;
            exp->expr = parse_application(index, tokenarray);
            break;
        } else if (tokenarray[*index].token_id == LPAR) {
            exp->type = APP;
            exp->expr = parse(index, tokenarray);
            break;
        }
    case RPAR:
        PRINT_ERROR("expected an expression");
        exit(1);
    case NUMBER:
        exp->type = NUMBER;
        exp->expr = malloc(sizeof(num_exp));
        ((num_exp *)exp->expr)->num = atoi((char *)tokenarray[*index].value);
        PRINT_DEBUG("found a number %d", ((num_exp *)exp->expr)->num);
        (*index)++;
        break;
    case VAR:
        exp->type = VAR;
        exp->expr = malloc(sizeof(var_exp));
        ((var_exp *)exp->expr)->name = (char *)tokenarray[*index].value;
        PRINT_DEBUG("found a variable: %s", ((var_exp *)exp->expr)->name);
        (*index)++;
        break;
    }
    return exp;
}
