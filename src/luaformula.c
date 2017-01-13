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

#include <string.h>
#include <time.h>
#include "luaformula.h"

// this will be available in the globals, so the user can tamper its value, so what now ???
#define _MAGIC_LUA_TO_VAR_ "___the_magic_lua_variable____"

/**
 * Load Lua state and protect it from using some libraries and functions
 * that can harm the system
 * @param managedCode True if we can trust in the code
 * @return Pointer to Lua state or NULL if error
 */
lua_State * luaStateAlloc(int managedCode) {
    lua_State *L = NULL;
    L = luaL_newstate();
    if (L) {
        luaL_openlibs(L);
        if (!managedCode) {
            const char *preCode =
                    "io=nil; " \
                    "os.remove=nil; " \
                    "os.getenv=nil; " \
                    "os.rename=nil; " \
                    "os.tmpname=nil; " \
                    "os.setlocale=nil; " \
                    "os.execute=nil; " \
                    "os.exit=nil; " \
                    "debug=nil; " \
                    "package=nil; " \
                    "dofile=nil; " \
                    "load=nil; " \
                    "pcall=nil; " \
                    "loadfile=nil; " \
                    "loadstring=nil; " \
                    "setfenv=nil; " \
                    "require=nil; " \
                    "module = nil;" \
                    "xpcall = nil;" \
                    "setmetatable = nil;";
            if ((luaL_loadstring(L, preCode) || lua_pcall(L, 0, 0, 0))) {
                lua_close(L);
                L = NULL;
            }
        }
    }
    return L;
}

/**
 * Close lua state
 * @param L Lua state pointer
 */
void luaStateFree(lua_State *L) {
    lua_close(L);
}

/**
 * Internal script timer hook
 * @param L
 * @param ar
 */
static void _lua_hook_timeout__(lua_State* L, lua_Debug *ar) {
    const char *v = _MAGIC_LUA_TO_VAR_;
    lua_getglobal(L, v);
    if (lua_isnumber(L, -1)) {
        time_t timeLimit = lua_tonumber(L, -1);
        time_t now = time(NULL);
        if (timeLimit >= now) {
            return;
        }
    }
    lua_sethook(L, _lua_hook_timeout__, LUA_MASKLINE, 0);
    luaL_error(L, "{\"error\": \"timeout\"}");
}

/**
 * Execute a formula
 * The user may wish to wrap this function in a new thread and join it with a timer
 * @param L TheLua state
 * @param code The code to be executed
 * @param result The formula result
 * @param timeout The maximum execution time in seconds
 * @return NULL if OK or a error message. Must be freed by the caller
 */
char *luaExecuteFormula(lua_State *L, const char *code, double *result, int timeout) {
    char *returnMessage = NULL;
    if (timeout > 0) {
        time_t startTime = time(NULL) + timeout;
        lua_pushnumber(L, startTime);
        lua_setglobal(L, _MAGIC_LUA_TO_VAR_);
        lua_sethook(L, _lua_hook_timeout__, LUA_MASKLINE, 100);
    }
    if ((luaL_loadstring(L, code) || lua_pcall(L, 0, 1, 0))) {
        returnMessage = strdup((char *) lua_tostring(L, -1));
    } else {
        if (lua_isboolean(L, -1)) {
            *result = (double) lua_toboolean(L, -1);
            if (*result) *result = 1;
        } else if (lua_isnumber(L, -1)) {
            *result = (double) lua_tonumber(L, -1);
        } else {
            returnMessage = strdup("Formula must return a number as in: return a + b");
        }
        lua_pop(L, 1);
    }
    return returnMessage;
}
