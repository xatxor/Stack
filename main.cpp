#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TXLib.h"

#define STACK_CTOR(stk) stack_ctor(stk, #stk, __LINE__, __FILE__, __func__);
#define STACK_DUMP(stk) stack_dump(stk, #stk, __LINE__, __FILE__, __func__);

#define ELEM_F "%d"
typedef int elem_t;

enum Errors{
    OK = 0,
    ERR = 1,
};

struct Stack{
    const char* name;
    elem_t* data;
    int cur_size;
    int capacity;
    int line;
    const char* file;
    const char* func;
};

Errors stack_ctor(Stack* stk, const char* name, int line, const char* file, const char* func);
Errors stack_dtor(Stack* stk);
Errors stack_realloc_increase(Stack* stk);
Errors stack_realloc_decrease(Stack* stk);
Errors stack_realloc_nullify(elem_t* start, elem_t* finish);
Errors push(Stack* stk, elem_t value);
Errors pop(Stack* stk, elem_t* rtrn_value);
Errors stack_verif(Stack* stk);
Errors stack_dump(Stack* stk, const char* name, int line, const char* file, const char* func);
Errors stack_print(Stack* stk);
Errors error_manager(Errors err);
int max_int(int a, int b);
void my_exit(Errors err);

int main(void)
{
    Stack stk = {};
    STACK_CTOR(&stk);

    STACK_DUMP(&stk);

    push(&stk, 1);

    STACK_DUMP(&stk);

    push(&stk, 2);

    STACK_DUMP(&stk);

    push(&stk, 3);

    STACK_DUMP(&stk);

    stack_dtor(&stk);

    return 0;
}

Errors stack_ctor(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    assert(stk);

    stk->cur_size = 0;
    stk->capacity = 1;

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    stk->data = (elem_t*)calloc(stk->capacity, sizeof(elem_t));

    printf("stack_ctor happened!\n");

    return OK;
}

Errors stack_dtor(Stack* stk)
{
    assert(stk);

    printf("stack_dtor started...\n");

    free(stk->data);
    stk->cur_size = -1;
    stk->capacity = -1;
    stk->name = NULL;
    stk->file = NULL;
    stk->func = NULL;

    printf("stack_dtor happened!\n");

    return OK;
}

Errors stack_realloc_increase(Stack* stk)
{
    stack_verif(stk);
    printf("stack_realloc_increase happened!\n");

    int new_cap = max_int(stk->capacity*2, 1);

    stk->data = (elem_t*)realloc(stk->data, new_cap*sizeof(elem_t));
    assert(stk->data);
    //stack_realloc_nullify(stk->data + (stk->capacity+1), stk->data + (stk->capacity*2));
    stk->capacity = new_cap;

    return OK;
}


Errors stack_realloc_decrease(Stack* stk)
{
    stack_verif(stk);
    printf("stack_realloc_decrease happened!\n");

    int new_cap = max_int(stk->capacity/2, 1);

    stk->data = (elem_t*)realloc(stk->data, new_cap*sizeof(elem_t));
    assert(stk->data);
    stk->capacity = new_cap;

    return OK;
}

Errors stack_realloc_nullify(elem_t* start, elem_t* finish)
{
    while (start < finish)
    {
        *start = 0;
        start++;
    }
    return OK;
}

Errors push(Stack* stk, elem_t value)
{
    stack_verif(stk);

    if ((stk->cur_size + 1) > stk->capacity)
    {
        if (Errors err = stack_realloc_increase(stk))
            return error_manager(err);
    }

    stk->data[stk->cur_size] = value;
    (stk->cur_size)++;

    printf("stack_push happened!\n");

    STACK_DUMP(stk);

    return OK;
}

Errors pop(Stack* stk, elem_t* rtrn_value)
{
    stack_verif(stk);

    if (stk->cur_size <= 0)
    {
        printf(!"pop is not done!: stk->cur_size <= 0!");
            return ERR;
    }
    *rtrn_value = stk->data[stk->cur_size-1];
    stk->data[stk->cur_size-1] = 0;
    (stk->cur_size)--;

    if (stk->cur_size <= stk->capacity/4)
    {
        if (Errors err = stack_realloc_decrease(stk))
            return error_manager(err);
    }

    printf("stack_pop happened!\n");

    return OK;
}

Errors stack_verif(Stack* stk)
{
    assert(stk);
    assert(stk->data);
    assert(stk->capacity > 0);
    assert(stk->cur_size >= 0);
    assert(stk->file);
    assert(stk->line);
    assert(stk->func);
    assert(stk->name);

    return OK;
}

Errors stack_dump(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    stack_verif(stk);
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
}

Errors stack_print(Stack* stk)
{
    stack_verif(stk);
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

Errors error_manager(Errors err)
{
    switch (err)
    {
        case ERR:
            printf("Error code: ERR");
            return ERR;
        default:
            return OK;
    }
}

int max_int(int a, int b)
{
    if (a > b)
        return a;
    return b;
}
