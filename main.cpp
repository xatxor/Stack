#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TXLib.h"

#include "header.h"

int main(void)
{
    Stack stk = {};
    STACK_CTOR(&stk);

    STACK_DUMP(&stk);

    push(&stk, 1);
    push(&stk, 2);
    push(&stk, 3);
    printf("1 stk.right_canary = %d\n", (int)stk.right_canary);
    STACK_DUMP(&stk);
    printf("2 stk.right_canary = %d\n", (int)stk.right_canary);
    elem_t val = 0;
    pop(&stk, &val);
    printf("3 stk.right_canary = %d\n", (int)stk.right_canary);

    STACK_DUMP(&stk);

    pop(&stk, &val);

    STACK_DUMP(&stk);

    pop(&stk, &val);

    stack_dtor(&stk);

    return 0;
}
