#include "memory.h"

void free_frame(frame_t frame)
{
}

/*  */
frame_t *extend_environment(frame_t *env)
{
    frame_t *new_frame = (frame_t *)malloc(sizeof(frame_t));
    if (new_frame != NULL)
    {
        new_frame->actuals = NULL, new_frame->next_frame = NULL, new_frame->next_frame = env, new_frame->points = 1;
        return new_frame;
    }

    fprintf(stderr, "memory.c: extend_environment: out of memory");
    exit(1);
    return NULL;
}

/*  */
frame_t *contract_environment(frame_t *env)
{
    if (env->points == 1)
    {
        frame_t *next_frame = env->next_frame;
    }
}

// /* environment */
// struct frame extend_environment(struct frame *env)
// {
//     struct frame new_env = {
//         .next_frame = env,
//         .actuals = NULL,
//         .variables = NULL,
//     };
//     return new_env;
// }
//
// // adds a new binding of a variable to a value or in other words, from variables to an actual value/piece of data
// struct frame add_binding(struct frame *env, char variables, struct actuals_s actual)
// {
//     struct node new_vars = {
//         .data = variables,
//         .next = &(env->variables)};
//
//     env->variables = (struct frame){
//         .data = variables,
//         .next = &(env->variables)};
//
//     env->actuals = (struct node){
//         .data = actual,
//         .next = env.actuals,
//     };
//     return *env;
// }
//
