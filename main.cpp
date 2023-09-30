#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TXLib.h"

#define STACK_CTOR(stk) stack_ctor(stk, #stk, __LINE__, __FILE__, __func__);
#define STACK_DUMP(stk) stack_dump(stk, #stk, __LINE__, __FILE__, __func__);
#define STACK_VERIF(stk) stack_verif(stk, #stk, __LINE__, __FILE__, __func__);
#define CHECK_ERR(stk, err) if (stk->status & err) printf(#err "\n");

#define ELEM_F "%d"
typedef int elem_t;

enum Stack_Errors{
    OK = 0,
    STACK_NULLPTR = 1 << 1,
    DATA_NULLPTR = 1 << 2,
    NEGATIVE_CAP = 1 << 3,
    NEGATIVE_SIZE = 1 << 4,
    SIZE_GREATER_CAP = 1 << 5,
    NULL_FILE = 1 << 6,
    NULL_LINE = 1 << 7,
    NULL_FUNC = 1 << 8,
    NULL_NAME = 1 << 9,
    IMPOSSIBLE_ACTION = 1 << 10
};

struct Stack{
    const char* name;
    elem_t* data;
    int cur_size;
    int capacity;
    int line;
    const char* file;
    const char* func;
    int status;
};

Stack_Errors stack_ctor(Stack* stk, const char* name, int line, const char* file, const char* func);
Stack_Errors stack_dtor(Stack* stk);
Stack_Errors stack_realloc_increase(Stack* stk);
Stack_Errors stack_realloc_decrease(Stack* stk);
Stack_Errors stack_realloc_nullify(Stack* stk);
Stack_Errors push(Stack* stk, elem_t value);
Stack_Errors pop(Stack* stk, elem_t* rtrn_value);
Stack_Errors stack_verif(Stack* stk, const char* name, int line, const char* file, const char* func);
void stack_check_status(Stack* stk, const char* name, int line, const char* file, const char* func);
Stack_Errors stack_dump(Stack* stk, const char* name, int line, const char* file, const char* func);
Stack_Errors stack_print(Stack* stk);
int max_int(int a, int b);

int main(void)
{
    Stack stk = {};
    STACK_CTOR(&stk);

    STACK_DUMP(&stk);

    push(&stk, 1);
    push(&stk, 2);
    push(&stk, 3);

    STACK_DUMP(&stk);

    elem_t val = 0;
    pop(&stk, &val);

    STACK_DUMP(&stk);

    pop(&stk, &val);

    STACK_DUMP(&stk);

    pop(&stk, &val);
    pop(&stk, &val);

    stack_dtor(&stk);

    return 0;
}

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

Stack_Errors stack_realloc_increase(Stack* stk)
{
    STACK_VERIF(stk);

    int old_cap = stk->capacity;
    int new_cap = max_int(old_cap*2, 1);

    stk->data = (elem_t*)realloc(stk->data, new_cap*sizeof(elem_t));
    stk->capacity = new_cap;
    stack_realloc_nullify(stk);

    STACK_VERIF(stk);

    printf("stack_realloc_increase happened from %d to %d!\n", old_cap, new_cap);

    return OK;
}

Stack_Errors stack_realloc_decrease(Stack* stk)
{
    STACK_VERIF(stk);

    int old_cap = stk->capacity;
    int new_cap = max_int(old_cap/2, 1);

    stk->data = (elem_t*)realloc(stk->data, new_cap*sizeof(elem_t));
    stk->capacity = new_cap;

    STACK_VERIF(stk);

    printf("stack_realloc_decrease happened from %d to %d!\n", old_cap, new_cap);

    return OK;
}

Stack_Errors stack_realloc_nullify(Stack* stk)
{
    STACK_VERIF(stk);

    int start = stk->cur_size;
    int finish = stk->capacity;
    for (int i = start; i < finish; i++)
        stk->data[i] = 0;

    STACK_VERIF(stk);
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

Stack_Errors stack_verif(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    if (!stk)                                   stk->status |= STACK_NULLPTR;
    if (!stk->data)                             stk->status |= DATA_NULLPTR;
    if (stk->capacity <= 0)                     stk->status |= NEGATIVE_CAP;
    if (stk->cur_size < 0)                      stk->status |= NEGATIVE_SIZE;
    if (stk->cur_size > stk->capacity)          stk->status |= SIZE_GREATER_CAP;
    if (!stk->file)                             stk->status |= NULL_FILE;
    if (!stk->line)                             stk->status |= NULL_LINE;
    if (!stk->func)                             stk->status |= NULL_FUNC;
    if (!stk->name)                             stk->status |= NULL_NAME;

    if (stk->status != 0)
        stack_check_status(stk, name, line, file, func);

    return OK;
}

void stack_check_status(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    if (stk->status == 0)
        return;
    printf("\nERRORS were found:\n");

    CHECK_ERR(stk, STACK_NULLPTR);
    CHECK_ERR(stk, DATA_NULLPTR);
    CHECK_ERR(stk, NEGATIVE_CAP);
    CHECK_ERR(stk, NEGATIVE_SIZE);
    CHECK_ERR(stk, SIZE_GREATER_CAP);
    CHECK_ERR(stk, NULL_FILE);
    CHECK_ERR(stk, NULL_LINE);
    CHECK_ERR(stk, NULL_FUNC);
    CHECK_ERR(stk, NULL_NAME);
    CHECK_ERR(stk, IMPOSSIBLE_ACTION);

    stack_dump(stk, name, line, file, func);
    stack_dtor(stk);

    printf("Bye!\n");
    abort();
}

Stack_Errors stack_dump(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    printf("\nSTACK_DUMP happened!\n");
    printf("stack[%p] ", stk);

    printf("created as \"%s\" from file %s (line %d)\n", stk->name, stk->file, stk->line);
    printf("    from func \"%s\"\n", stk->func);

    printf("        called as \"%s\" from file %s (line %d)\n", name, file, line);
    printf("            from func \"%s\"\n", func);

    printf("                cur_size = %d\n", stk->cur_size);
    printf("                capacity = %d\n", stk->capacity);
    printf("                data[%p] = \n", stk->data);
    stack_print(stk);

    return OK;
}

Stack_Errors stack_print(Stack* stk)
{
    printf("                ");
    printf("{\n");

    int i = 0;

    while (i < stk->cur_size)
    {
        printf("                    ");
        printf("*");
        printf("[%d] = %p = " ELEM_F "\n", i , stk->data+i, stk->data[i]);
        i++;
    }
    while (i < stk->capacity)
    {
        printf("                    ");
        printf(" ");
        printf("[%d] = %p = " ELEM_F "\n", i, stk->data+i, stk->data[i]);
        i++;
    }
    printf("                ");
    printf("}\n");

    return OK;
}

int max_int(int a, int b)
{
    if (a > b)
        return a;
    return b;
}
