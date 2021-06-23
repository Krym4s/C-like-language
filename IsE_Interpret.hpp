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

const char* STANDART_INPUT = "data/russian_code.txt";

//!-------------------------------------------------
//! handles input and remembers code
//! @param [in] nArgs  - number of arguments in main
//! @param [in] args   - args of main
//!
//! @param [out] codeBuf - buffer with code
//!-------------------------------------------------

CompilationErrors SplitInputToTokens (int nArgs, char* args[], char* buffer); 

//!-------------------------------------------------
//!
//!
//!
//!
//!-------------------------------------------------

