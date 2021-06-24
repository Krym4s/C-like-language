#include "IsE_Interpret.hpp"

CompilationErrors SplitInputToTokens (int nArgs, char* args[], int* nTokens, char* buffer)
{
    char* codeBuf = nullptr;

    if (nArgs > 1)      // there are files
        if(readToStorage (args[1], &codeBuf))
            return LAN_NO_INPUT_FILE;
    else
         if(readToStorage (STANDART_INPUT, &codeBuf))
            return LAN_NO_INPUT_FILE;

    LogicElement* tokens = nullptr;

    *nTokens = GetTokens (codeBuf, &tokens);

    if (*nTokens == -1)
        return LAN_BAD_NUMBER_TOKEN;

    return LAN_NO_COMPILATION_ERRORS;
}

int GetTokens (const char* codeBuf, LogicElement** tokens)
{
    assert (codeBuf);

    TokenSplitter splitter = {(LogicElement*) calloc (START_TOKEN_NUMBER, sizeof (**tokens)), 
                              0,
                              START_TOKEN_NUMBER, 
                              0}; 

    if (!splitter.tokens)
        return LAN_NOT_ENOUGH_MEMORY;

    while (*(codeBuf + splitter.symbIdx) != '\0' )
    {
        SkipSpaces (codeBuf, &splitter.symbIdx);
       //if(!HandleNumber (codeBuf, tokens, &index, &currentArraySize, &numberOfElements))
        {
            SkipSpaces (codeBuf, &splitter.symbIdx);
            //HandleIdentificator (codeBuf, tokens, &index, &currentArraySize, &numberOfElements);
            SkipSpaces (codeBuf, &splitter.symbIdx);
        }
        SkipSpaces (codeBuf, &splitter.symbIdx);
    }

}

int IsSpace (const char symb)
{
    switch (symb)
    {
        case '\n': case '\t': case ' ': case '\r': case '.': case ',': case '!': case '?': case '\'':return 1;
        default: return 0;
    }
}

unsigned int SkipSpaces (const char* buffer, unsigned int* index)
{
    while (IsSpace(*(buffer + *index)) && *(buffer + *index) != '\0')
        (*index)++;

    return *index;
}