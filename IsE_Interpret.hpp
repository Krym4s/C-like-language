#include "IsE_LinesF.h"
#include "LogicTree.h"

enum CompilationErrors
{
    LAN_NO_INPUT_FILE          = -4,
    LAN_BAD_NUMBER_TOKEN       = -3,
    LAN_UNDECLARED_ID          = -2,
    LAN_NOT_ENOUGH_MEMORY      = -1,
    LAN_NO_COMPILATION_ERRORS  =  0,
};

struct Vocabulary
{
    char** vocabulary;
    unsigned int size;
    unsigned int index;
};

struct TrVocabulary
{
    char** vocabulary;
    unsigned int size;
    unsigned int index;
};

struct TokenSplitter
{
    LogicElement*   tokens;
    unsigned int    symbIdx;
    unsigned int    currentArraySize;
    unsigned int    numberOfElements;
};


const char* STANDART_INPUT      = "data/russian_code.txt";
const int   MISTAKE             = -1;

const int   START_TOKEN_NUMBER  = 1000;

//!-------------------------------------------------
//! handles input and remembers code as vocabulary
//! @param [in] nArgs  - number of arguments in main
//! @param [in] args   - args of main
//!
//! @param [out] nTokens - number of Tokens
//! @param [out] codeBuf - buffer with code
//!-------------------------------------------------

CompilationErrors SplitInputToTokens (int nArgs, char* args[], int* nTokens, char* buffer); 

//!-------------------------------------------------
//! split code written in string ti tikens
//! @param [in] codeBuf - string with code
//! @param [out] tokens - array of tokens
//!
//! @return - number of tokens 
//! @note  number of tokens is -1 if there are mistakes
//!-------------------------------------------------

int GetTokens (const char* codeBuf, LogicElement** tokens);

//!------------------------------------------------
//! checks if symbol is space in IsE interpretatio
//! @param [in] symb - input symbol
//!
//! @return - 0 in case symbol is not space, 1 in other cases 
//!-------------------------------------------------

int IsSpace (const char symb);

//!--------------------------------------------------
//! Skips all spaces in string 
//! @param [in] buffer - string
//! @param [out] index  -  number of begining symbol in string
//!
//! @note - after skipping all spaces index will point to first not space symbol
//!--------------------------------------------------

unsigned int SkipSpaces (const char* buffer, unsigned int* index);
