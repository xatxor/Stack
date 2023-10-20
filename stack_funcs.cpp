#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>

//#include "TXLib.h"

#include "header.h"

Stack_Errors stack_ctor(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    assert(stk);
    printf("stack_ctor happening...\n");
    stk->cur_size = 0;
    stk->capacity = 2;

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    stk->status = 0;
    stk->data = (elem_t*)realloc(stk->data, 2*sizeof(canary_t) + stk->capacity*sizeof(elem_t));
    stk->data = (elem_t*)((char*)stk->data + sizeof(canary_t));

    stack_set_data_canaries(stk);

    stk->left_canary = CANARY_VALUE;
    stk->right_canary = CANARY_VALUE;

    stack_hash_update(stk);

    stack_realloc_nullify(stk);

    stack_hash_update(stk);

    STACK_VERIF(stk);

    printf("stack_ctor happened!\n");

    return OK;
}

Stack_Errors stack_dtor(Stack* stk)
{
    assert(stk);
    assert(stk->data);

    free(stack_get_left_canary(stk));
    stk->cur_size = -1;
    stk->capacity = -1;
    stk->name = NULL;
    stk->file = NULL;
    stk->func = NULL;
    stk->hashsum = 0;

    printf("stack_dtor happened!\n");

    return OK;
}

Stack_Errors push(Stack* stk, elem_t value)
{
    STACK_VERIF(stk);
    printf("stack_push happening...\n");

    if ((stk->cur_size + 1) > stk->capacity)
        stack_realloc_increase(stk);

    stk->data[stk->cur_size] = value;
    (stk->cur_size)++;

    stack_hash_update(stk);

    printf("stack_push happened with " ELEM_F " \n", value);

    return OK;
}

elem_t pop(Stack* stk)
{
    elem_t result = 0;
    if (stk->cur_size == 0)
        stk->status |= IMPOSSIBLE_ACTION;
    printf("stack_pop happening...\n");
    STACK_VERIF(stk);

    result = stk->data[stk->cur_size-1];
    stk->data[stk->cur_size-1] = EMPTY_ELEM;
    (stk->cur_size)--;

    stack_hash_update(stk);

    if (stk->cur_size <= stk->capacity/4)
        stack_realloc_decrease(stk);

    stack_hash_update(stk);

    printf("stack_pop happened with " ELEM_F " \n", result);

    return result;
}

Stack_Errors stack_set_data_canaries(Stack* stk)
{
    *(stack_get_left_canary(stk)) = CANARY_VALUE;
    *(stack_get_right_canary(stk)) = CANARY_VALUE;

    return OK;
}

canary_t* stack_get_left_canary(Stack* stk)
{
    assert(stk);
    assert(stk->data);

    return (canary_t*)((char*)stk->data - sizeof(canary_t));
}

canary_t* stack_get_right_canary(Stack* stk)
{
    assert(stk);
    assert(stk->data);

    return (canary_t*)((char*)stk->data + stk->capacity*sizeof(elem_t));
}

Stack_Errors stack_hash_update(Stack* stk)
{
    stk->hashsum = 0;
    stk->hashsum = stack_get_hash(stk);

    return OK;
}

long long unsigned int stack_get_hash(Stack* stk)
{
    size_t d_size = 2*sizeof(canary_t) + stk->capacity*sizeof(elem_t);
    return get_hash((char*)stk, sizeof(stk)) + get_hash((char*)stack_get_left_canary(stk), d_size);
}
