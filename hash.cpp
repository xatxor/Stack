#include <stdio.h>
#include <stdlib.h>
#include "header.h"

long long unsigned int get_hash(char* str, long long unsigned int len)
{
   long long unsigned int h = 5381;
   long long unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      h = ((h << 5) + h) + (*str);
   }
   return h;
}
