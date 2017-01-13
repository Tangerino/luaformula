# luaformula
Securely execute a Lua formula

The main purpose of Lua is to enable execute chunks of Lua code from a host C program.
When you trust the Lua code to be executed there is no major concerns about security, what I call execute a managed code. 
But if the source code to be executed comes from an external source (file, API call, user input, etc) some precaution need to be done in order to avoid any security concerns such as:
* The script never returns
* The script tries to access your file system
* Exhaust memory
* Run arbitrary programs, etc

This code tries to tries to eliminates these concerns as it removes (if necessary) some system libraries from the Lua context and impose a execution time limitation to the user code.
Once the time out is reach, the user code will break.

Enjoy
