#include "IsE_Interpret.hpp"
CompilationErrors SplitInputToTokens (int nArgs, char* args[], char* buffer)
{
    char* codeBuf = nullptr;

    if (nArgs > 1)      // there are files
    {
        if(readToStorage (args[1], &codeBuf))
            return LAN_NO_INPUT_FILE;
    }
    else
         if(readToStorage (STANDART_INPUT, &codeBuf))
            return LAN_NO_INPUT_FILE;
    

    

    return LAN_NO_COMPILATION_ERRORS;
}