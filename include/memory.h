#ifndef _DATASTRUCTURES_H
#define _DATASTRUCTURES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// types of the programming language
enum lisp_type
{
    BOOL,
    NUMBER,
    STRING,
    PROCEDURE,
    SYMBOL,
};

// actual value representation
struct actuals_s
{
    enum lisp_type type;
    union
    {
        // bools, numbers
        uint64_t value;

        // strings and symbols
        struct
        {
            uint16_t length;
            char *data;
        };

        // procedure
        struct
        {
            // env
            // param
            // body
            // TODO
        };

    } data;
};

// linked list lol
typedef struct node
{
    void *data;
    struct node *next;
} node_t;

// environment
// representation of the scope and uderlying scopes
typedef struct frame
{
    struct frame *next_frame;
    node_t *variables; // char **
    node_t *actuals;   // struct actuals
    uint16_t points;   // keeps track of the amount of pointers that are using this frame
} frame_t;

#endif