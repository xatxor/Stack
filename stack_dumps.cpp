#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include "TXLib.h"

#include "header.h"

Stack_Errors stack_verif(Stack* stk, const char* name, int line, const char* file, const char* func)
{
    if (!stk)                                               stk->status |= STACK_NULLPTR;
    if (!stk->data)                                         stk->status |= DATA_NULLPTR;
    if (stk->capacity <= 0)                                 stk->status |= NEGATIVE_CAP;
    if (stk->cur_size < 0)                                  stk->status |= NEGATIVE_SIZE;
    if (stk->cur_size > stk->capacity)                      stk->status |= SIZE_GREATER_CAP;
    if (!stk->file)                                         stk->status |= NULL_FILE;
    if (!stk->line)                                         stk->status |= NULL_LINE;
    if (!stk->func)                                         stk->status |= NULL_FUNC;
    if (!stk->name)                                         stk->status |= NULL_NAME;
    if (*(stack_get_left_canary(stk)) != CANARY_VALUE)      stk->status |= LEFT_CANARY;
    if (*(stack_get_right_canary(stk)) != CANARY_VALUE)     stk->status |= RIGHT_CANARY;

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
    CHECK_ERR(stk, LEFT_CANARY);
    CHECK_ERR(stk, RIGHT_CANARY);

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
