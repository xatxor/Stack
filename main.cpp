#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "header.h"

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

    stack_dtor(&stk);

    return 0;
}
