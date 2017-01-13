
/* 
 * File:   main.c
 * Author: Carlos Tangerino <carlos.tangerino@gmail.com>
 *
 * Created on January 13, 2017, 3:42 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "luaformula.h"

const char * prg1 = "for n in pairs(_G) do print(n) end";

/*
 * 
 */
int main(int argc, char** argv) {
    lua_State *L = luaStateAlloc(0);
    double result;
    char *m = luaExecuteFormula(L, prg1, &result, 1);
    if (m) {
        printf("Formula return an error (%s)\n", m);
        free(m);
    } else {
        printf("Result is %f\n", result);
    }
    return (EXIT_SUCCESS);
}

