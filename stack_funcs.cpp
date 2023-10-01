#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include "TXLib.h"

#include "header.h"

Stack_Errors stack_ctor(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    assert(stk);

    stk->cur_size = 0;
    stk->capacity = 1;

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    stk->status = 0;

    stk->data = (elem_t*)calloc(stk->capacity, sizeof(elem_t));

    STACK_VERIF(stk);

    printf("stack_ctor happened!\n");

    return OK;
}

Stack_Errors stack_dtor(Stack* stk)
{
    assert(stk);

    free(stk->data);
    stk->cur_size = -1;
    stk->capacity = -1;
    stk->name = NULL;
    stk->file = NULL;
    stk->func = NULL;

    printf("stack_dtor happened!\n");

    return OK;
}

Stack_Errors push(Stack* stk, elem_t value)
{
    STACK_VERIF(stk);

    if ((stk->cur_size + 1) > stk->capacity)
        stack_realloc_increase(stk);

    stk->data[stk->cur_size] = value;
    (stk->cur_size)++;

    printf("stack_push happened with " ELEM_F "!\n", value);

    return OK;
}

Stack_Errors pop(Stack* stk, elem_t* rtrn_value)
{
    if (stk->cur_size == 0)
        stk->status |= IMPOSSIBLE_ACTION;

    STACK_VERIF(stk);

    *rtrn_value = stk->data[stk->cur_size-1];
    stk->data[stk->cur_size-1] = 0;
    (stk->cur_size)--;

    if (stk->cur_size <= stk->capacity/4)
        stack_realloc_decrease(stk);

    printf("stack_pop happened with " ELEM_F "!\n", *rtrn_value);

    return OK;
}
