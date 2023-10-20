//#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "header.h"

int main(void)
{
    Stack stk = {};
    STACK_CTOR(&stk);

    //STACK_DUMP(&stk);

    push(&stk, 1);
    push(&stk, 2);
    push(&stk, 3);

    /*
    *(int*)stk = 0;
    ((int*)stk)[1] = 0;
    ((char*)stk)[10] = -1;
    */

    //((char*)(&stk))[10] = 0;

    //STACK_DUMP(&stk);

    elem_t val = 0;
    val = pop(&stk);
    val = pop(&stk);

    val = pop(&stk);

    stack_dtor(&stk);

    return 0;
}
