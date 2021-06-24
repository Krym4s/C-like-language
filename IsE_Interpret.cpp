#include "IsE_Interpret.hpp"

const char* STANDART_INPUT      = "data/russian_code.txt";
const int   MISTAKE             = -1;

const int   START_TOKEN_NUMBER  = 1000;

CompilationErrors SplitInputToTokens (int nArgs, char* args[], int* nTokens, LogicElement** tokens)
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

    *nTokens = GetTokens (codeBuf, tokens);

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
        if (TokensNumberFit (&splitter))
            return LAN_NOT_ENOUGH_MEMORY;

        SkipSpaces (codeBuf, &splitter.symbIdx);

        if (!HandleNumber (codeBuf, &splitter))
            HandleID (codeBuf, &splitter);
    }

    return splitter.numberOfElements;

}

CompilationErrors TokensNumberFit (TokenSplitter* splitter)
{
    if (splitter->currentArraySize <= splitter->numberOfElements + 2) // as we can add two tokens in one cycle
    {
        splitter->currentArraySize *= 2;
        LogicElement* temp  = (LogicElement*) realloc (splitter->tokens, sizeof(splitter->tokens) * splitter->currentArraySize);

        if (!temp)
        {
            free (splitter->tokens);
            return LAN_NOT_ENOUGH_MEMORY;
        }

        splitter->tokens = temp;
    }
    return LAN_NO_COMPILATION_ERRORS;
}

int HandleNumber (const char* codeBuf, TokenSplitter* splitter)
{
    double value    = 0;
    int    sign     = 1;
    int    change   = 0;
    double exp      = 1;

    if (*(codeBuf + splitter->symbIdx) == '\"')
    {
        change++;
        splitter->symbIdx++;

        SkipSpaces (codeBuf, &splitter->symbIdx);

        if (*(codeBuf + splitter->symbIdx) == IY0 || *(codeBuf + splitter->symbIdx) == H)
        {
            sign = -1;
            while (!IsSpace(*(codeBuf + splitter->symbIdx)))
                if (IsTerminal(*(codeBuf + splitter->symbIdx)))
                    (splitter->symbIdx)++;
                else
                    SyntaxError (codeBuf, splitter);

            SkipSpaces (codeBuf, &splitter->symbIdx);
        }
        while (*(codeBuf + splitter->symbIdx) != L'\"' && !(*(codeBuf + splitter->symbIdx) == K || *(codeBuf + splitter->symbIdx) == C))
        {
            SkipSpaces (codeBuf, &splitter->symbIdx);
            while (IsAlpha (*(codeBuf + splitter->symbIdx)))
            {
                if (*(codeBuf + splitter->symbIdx) == K || *(codeBuf + splitter->symbIdx) == CH0)
                    SyntaxError (codeBuf, splitter);

                value = value * 10 + (*(codeBuf + splitter->symbIdx) - A) % 12;
                SkipAlphas (codeBuf, &splitter->symbIdx);
            }
            SkipSpaces (codeBuf, &splitter->symbIdx);
        }
        if (*(codeBuf + splitter->symbIdx) == K || *(codeBuf + splitter->symbIdx) == C)
        {
            SkipAlphas (codeBuf, &splitter->symbIdx);
            while (*(codeBuf + splitter->symbIdx) != L'\"')
            {
                SkipSpaces (codeBuf, &splitter->symbIdx);
                while (IsAlpha (*(codeBuf + splitter->symbIdx)))
                {
                    if (*(codeBuf + splitter->symbIdx) == K || *(codeBuf + splitter->symbIdx) == C || *(codeBuf + splitter->symbIdx) == IY0 || *(codeBuf + splitter->symbIdx) == H)
                        SyntaxError (codeBuf, splitter);

                    exp /= 10;
                    value = value + ((*(codeBuf + splitter->symbIdx) - A) % 12 ) * exp;
                    SkipAlphas (codeBuf, &splitter->symbIdx);
                }
                SkipSpaces (codeBuf, &splitter->symbIdx);
            }
        }
        if (*(codeBuf + splitter->symbIdx) != L'\"')
            SyntaxError (codeBuf, splitter);

        value *= sign;

        (splitter->tokens + splitter->numberOfElements)->type = NUMBER;
        (splitter->tokens + splitter->numberOfElements)->value.numberValue = value;
        (splitter->numberOfElements)++;
        (splitter->symbIdx)++;
    }
    return change;
}

CompilationErrors HandleID (const char* buffer, TokenSplitter* splitter)
{
    if (*(buffer + splitter->symbIdx) == '\0')
        return LAN_NO_COMPILATION_ERRORS;

    SkipSpaces (buffer, &splitter->symbIdx);
    const char* beginOfIdentificator = buffer + splitter->symbIdx;
    while (IsTerminal(*(buffer + splitter->symbIdx)))
        splitter->symbIdx++;

    if (!IsSpace (*(buffer + splitter->symbIdx)) && *(buffer + splitter->symbIdx) != '\0')
        SyntaxError (buffer, splitter);
    
    unsigned int length = buffer - beginOfIdentificator + splitter->symbIdx;
    if (!HandleOperatorType2 (beginOfIdentificator, splitter, length))
        if(!HandleOperatorType1 (beginOfIdentificator, splitter, length))
            HandleFunction (beginOfIdentificator, splitter, length);

    return LAN_NO_COMPILATION_ERRORS;
}

int HandleOperatorType1 (const char* buffer, TokenSplitter* splitter, unsigned int length)
{
#define FIRST_GROUP_OPERATOR(operatorName, operatorCode, textInterpret, codeInterpret) \
if (strncmp(codeInterpret, buffer + length - strlen (codeInterpret), strlen (codeInterpret)) == 0){ \
    (splitter->tokens + splitter->numberOfElements)->type = OPERATOR;                               \
    (splitter->tokens + splitter->numberOfElements)->value.operatorType = operatorName;             \
    splitter->numberOfElements++;                                                                   \
    return 1;                                                                                       \
    }
#include "data/1opgroup.txt"
#undef FIRST_GROUP_OPERATOR

    return 0;
}

int HandleOperatorType2 (const char* buffer, TokenSplitter* splitter, unsigned int length)
{
#define SECOND_GROUP_OPERATOR(operatorName, operatorCode, textInterpret, codeInterpret)     \
if (strncmp(codeInterpret, buffer, strlen (codeInterpret)) == 0){                           \
    (splitter->tokens + splitter->numberOfElements)->type = OPERATOR;                       \
    (splitter->tokens + splitter->numberOfElements)->value.operatorType = operatorName;     \
    splitter->numberOfElements++; printf ("%d %d\n", *codeInterpret, *buffer);                                                          \
    return 1;                                                                               \
    }
#include "data/2opgroup.txt"
#undef SECOND_GROUP_OPERATOR
    return 0;
}

int HandleFunction (const char* buffer, TokenSplitter* splitter, unsigned int length)
{
    char* temp = (char*) calloc (length, sizeof (*temp));

    (splitter->tokens + splitter->numberOfElements)->type = VARIABLE;
    strncpy (temp, buffer, length);
    printf ("[%s]\n", temp);
    (splitter->tokens + splitter->numberOfElements)->value.ID = temp;
    splitter->numberOfElements++;
    return 1;
}

int SyntaxError (const char* codeBuf, TokenSplitter* splitter)
{
    exit(1);
}

int IsSpace (const char symb)
{
    switch (symb)
    {
        case '\n': case '\t': case ' ': case '\r': case '.': case ',': case '!': case '?': case '\'':return 1;
        default: return 0;
    }
}

int IsTerminal (const char symb)
{
    if ( A <= symb && YA0 >= symb || symb == '\"' || symb == ':' || symb == '-' )
        return 1;
    else
        return 0;
}

int IsAlpha (const char symb)
{
    if (A <= symb && YA0 >= symb)
        return 1;
    else
        return 0;
}

unsigned int SkipSpaces (const char* buffer, unsigned int* index)
{
    while (IsSpace(*(buffer + *index)) && *(buffer + *index) != '\0')
        (*index)++;

    return *index;//
}

unsigned int SkipAlphas (const char* buffer, unsigned int* index)
{
    while (IsAlpha(*(buffer + *index)) && *(buffer + *index) != '\0')
        (*index)++;

    return *index;
}