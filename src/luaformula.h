/*
 MIT License

Copyright (c) 2013 Carlos Tangerino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 
 */

/* 
 * File:   luaformula.h
 * Author: Carlos Tangerino <carlos.tangerino@gmail.com>
 *
 * Created on January 13, 2017, 3:55 AM
 */

#ifndef LUAFORMULA_H
#define LUAFORMULA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

    /**
     * Load Lua state and protect it from using some libraries and functions
     * that can harm the system
     * @param managedCode True if we can trust in the code
     * @return Pointer to Lua state or NULL if error
     */
    lua_State * luaStateAlloc(int managedCode);

    /**
     * Close lua state
     * @param L Lua state pointer
     */
    void luaStateFree(lua_State *L);


    /**
     * Execute a formula
     * The user may wish to wrap this function in a new thread and join it with a timer
     * @param L TheLua state
     * @param code The code to be executed
     * @param result The formula result
     * @param timeout The maximum execution time in seconds
     * @return NULL if OK or a error message. Must be freed by the caller
     */
    char *luaExecuteFormula(lua_State *L, const char *code, double *result, int timeout);


#ifdef __cplusplus
}
#endif

#endif /* LUAFORMULA_H */

