#include "parser.h"

#define WARN_FLAG 1
#define DEBUG_FLAG 1

// TODO:
// - free for all ast structures

// checks if there is an RPAR token to close the expression.
// also updates the index
void has_RPAR(int *index, token *tokenarray) {
    if (tokenarray[*index].token_id != RPAR) {
        PRINT_ERROR("expected a RPAR");
        exit(1);
    }
    (*index)++;
}

// lambdas look like this: (lambda (<param(s)>) <expr>)
// lpar and lambda should be parsed by the parse function.
lambda_exp *parse_lamdba(int *index, token *tokenarray) {
    lambda_exp *lambda = (lambda_exp *)malloc(sizeof(lambda_exp));
    // for paremeter parsing but needs a parenthesis
    (*index)++;

    // checks if correct syntax is used: an opening parenthesis for the
    // parameter(s)
    if (tokenarray[(*index)++].token_id != LPAR) {
        PRINT_ERROR("parser: lambda where left parenthesis for parameter");
        exit(1);
    }

    // see if there are parameters?
    if (tokenarray[(*index)].token_id == RPAR) {
        PRINT_DEBUG("ITS A THUNNNKKK");
        lambda->parameters = NULL;
        // I think I wrote this because it also updates the index but not sure
        // TODO: check why this is here
        has_RPAR(index, tokenarray);
    } else {
        // if no RPAR is found,
        // parse the parameters
        // parameters are just a list of variables
        // (arg1 arg2 arg3 ...)
        //
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
        // allocating space for the parameters and copy in to the array
        lambda->parameters = (var_exp *)calloc(length, sizeof(var_exp));
        PRINT_DEBUG("found %d parameters: ", length);
        for (int param_index = 0; param_index < length; param_index++) {
            lambda->parameters[param_index].name =
                tokenarray[param_index + *index].value;
            PRINT_DEBUG("parameter %d: %s", param_index + 1,
                        lambda->parameters[param_index].name);
        }
        // update the index
        *index += length;
        PRINT_DEBUG("parameters are checked and assigned");

        // end of the parameter list is an `)`
        has_RPAR(index, tokenarray);
    }
    // the expression part will be parsed recursive, not the best option but
    // certainly the easiest...
    lambda->expression = parse(index, tokenarray);

    // lambda expressions end with `)`
    has_RPAR(index, tokenarray);
    return lambda;
}

void free_lambda(lambda_exp *lambda) {
    PRINT_DEBUG("freeeing lambda expr");
    free(lambda->parameters);
    free_dispatch(lambda->expression);
    free(lambda);
}

// if expressions: (if <condition> <consequent> <alternative>)
if_exp *parse_if(int *index, token *tokenarray) {
    // make space for if expression
    //  also, as if is a keyword in C, I had to use if_ which I find goofy but
    //  its only for this function
    if_exp *if_ = (if_exp *)malloc(sizeof(if_exp));
    (*index)++;

    // The opening Lpar and if does not have to be parsed, done by parse
    // function.
    // next is the condition, this can be an expression so we use parse recursif
    if_->condition = parse(index, tokenarray);
    if_->consequent = parse(index, tokenarray);
    if_->alternative = parse(index, tokenarray);

    // ending RPAR of the if expression
    has_RPAR(index, tokenarray);
    return if_;
}

void free_if(if_exp *if_) {
    PRINT_DEBUG("free if expr");
    // TODO: yaaaaa hyperrecursion
    free_dispatch(if_->condition);
    free_dispatch(if_->consequent);
    free_dispatch(if_->alternative);
    free(if_);
}

// aplications are expressions without the special tokens in the begin:
//  (<var> [<params>]*)
application_exp *parse_application(int *index, token *tokenarray) {
    application_exp *app = (application_exp *)malloc(sizeof(application_exp));
    // parsing the procedure part, which can be another application or lambda or
    // anything realy.
    //  ((<var>) 1) has to work
    app->procedure = parse(index, tokenarray);

    // Parameters can be expressions.
    // they are parsed using a loop.
    // a realloc is used as there is nothing to say how many arguments there are
    // given some space is allocated and will expand when too small. this is
    // accompished using 2 variables, one the args index and one the arg count
    // the count is how much space is allocated and the index to index into this
    // array.
    int args_index = 0;
    // initial allocation (in this case 4)
    app->arg_count = 4; // TODO: make an macro definition for this
    app->args = (expr_t *)calloc(app->arg_count, sizeof(expr_t));
    // start counting
    while (tokenarray[*index].token_id != RPAR) {
        // if there is more memory necessary <- this word is too hard to write
        //  then reallocate space
        if (args_index == app->arg_count) {
            // NOTE: realloc allong the way by multiples of 2
            app->args = (expr_t *)realloc(app->args, app->arg_count * 2);
            app->arg_count *= 2;
        }
        app->args[args_index] = *parse(index, tokenarray);
        args_index++;
    }

    // resize to just fit the amount of arguments
    app->args = (expr_t *)realloc(app->args, args_index);
    PRINT_DEBUG("amount of args %d", args_index);
    app->arg_count = args_index;

    // applications also end with a RPAR
    has_RPAR(index, tokenarray);
    return app;
}

void free_application(application_exp *app) {
    free_dispatch(app->procedure);
    PRINT_DEBUG("freeing arguments");
    if (app->arg_count)
        free_dispatch(app->args);
    free(app);
}

// the big boi, should parse anything
// gives back an expression... this makes it handy to use recursively altho not
// the most efficient should be rewritten so labels and gotos can be used
expr_t *parse(int *index, token *tokenarray) {
    // this is pretty straight forward
    expr_t *exp = (expr_t *)malloc(sizeof(expr_t));
    switch (tokenarray[*index].token_id) {
    case LPAR:
        // TODO: this case can be refactored so only a single swich statement is
        // used...
        PRINT_DEBUG("LPAR parsed");
        *index += 1;
        if (tokenarray[*index].token_id == LAMBDA) {
            PRINT_DEBUG("found lambda");
            exp->type = LAMBDA;
            exp->expr = parse_lamdba(index, tokenarray);
            break;
        } else if (tokenarray[*index].token_id == DEFINE) {
            PRINT_DEBUG("found define");
            // TODO: implement the define
            // NOTE: as a functional bro I dont think we need this, prove me
            // wrong... go nuts
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
            // expression in expression, never know what it is ...
            PRINT_DEBUG("nested shit");
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
        ((var_exp *)exp->expr)->name =
            calloc(strlen(tokenarray[*index].value), sizeof(char));

        memcpy(((var_exp *)exp->expr)->name, tokenarray[*index].value,
               strlen(tokenarray[*index].value));

        PRINT_DEBUG("found a variable: %s", ((var_exp *)exp->expr)->name);
        (*index)++;
        break;
    }
    return exp;
}

void free_dispatch(expr_t *expr) {
    switch (expr->type) {
    case LAMBDA:
        free_lambda(expr->expr);
        free(expr);
        break;
    case NUMBER:
        free(expr->expr);
        free(expr);
        break;
    case APP:
        free_application(expr->expr);
        free(expr);
        break;
    case IF:
        free_if(expr->expr);
        free(expr);
        break;
    case VAR:
        PRINT_DEBUG("freeing a variable");
        PRINT_DEBUG("variable: %s", ((var_exp *)expr->expr)->name);
        free((char *)((var_exp *)expr->expr)->name);
        PRINT_DEBUG("freeing exp");
        free(expr->expr);
        free(expr);
        break;
    default:
        PRINT_WARN("not recognised token %d", expr->type);
        break;
    }
}
