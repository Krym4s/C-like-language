#include "IsE_LinesF.h"
#include "LogicTree.h"
#include "CyrillicCode.hpp"
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

//!-------------------------------------------------
//! handles input and remembers code as vocabulary
//! @param [in] nArgs  - number of arguments in main
//! @param [in] args   - args of main
//!
//! @param [out] nTokens - number of Tokens
//! @param [out] tokens - lexical unit array
//!-------------------------------------------------

CompilationErrors SplitInputToTokens (int nArgs, char* args[], int* nTokens, LogicElement** tokens); 

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

 //!-------------------------------------------------
 //! @brief checks if symbol means something
 //! 
 //! @param [in] symb - symbol
 //! @return int - 0 if no 1 if yes
 //!-------------------------------------------------

int IsTerminal (const char symb);

 //!-------------------------------------------------
 //! @brief check if symbol is lettee
 //! 
 //! @param symb - symbol
 //! @return int - 0 if no, 1 if  yes
 //!-------------------------------------------------

int IsAlpha (const char symb);

//!--------------------------------------------------
//! Skips all spaces in string 
//! @param [in] buffer - string
//! @param [out] index  -  number of begining symbol in string
//!
//! @note - after skipping all spaces index will point to first not space symbol
//!--------------------------------------------------

unsigned int SkipSpaces (const char* buffer, unsigned int* index);

 //!-------------------------------------------------
 //! @brief skips all alphas in string
 //! 
 //! @param buffer - string
 //! @param index  - number of begining symbol in string
 //! @return unsigned int 
 //!-------------------------------------------------

unsigned int SkipAlphas (const char* buffer, unsigned int* index);

 //!-------------------------------------------------
 //! @brief Enlarge toens array if it is necessary
 //!
 //! @param [out] splitter - structure that handles token array  
 //! @return CompilationErrors - error code
 //!-------------------------------------------------
CompilationErrors TokensNumberFit (TokenSplitter* splitter);

 //!-------------------------------------------------
 //! @brief 
 //! 
 //! @param [in] codeBuf - buffer with IsE code
 //! @param [out] splitter - tokens handle structure
 //! @return CompilationErrors - error code
 //!-------------------------------------------------

int HandleNumber (const char* codeBuf, TokenSplitter* splitter);

 //!-------------------------------------------------
 //! @brief this function is called if there is syntax error in IsE code
 //! 
 //! @param codeBuf - buffer with code
 //! @param splitter - code handler
 //! @return int 
 //!-------------------------------------------------

int SyntaxError (const char* codeBuf, TokenSplitter* splitter);

 //!-------------------------------------------------
 //! @brief handles tokens that are not numbers 
 //! 
 //! @param buffer - buffer with code 
 //! @param splitter - code handle structure
 //! @return CompilationErrors - error code
 //!-------------------------------------------------

CompilationErrors HandleID (const char* buffer, TokenSplitter* splitter);

 //!-------------------------------------------------
 //! @brief handle operators of type 1 
 //! 
 //! @param buffer - buffer with IsE cide
 //! @param splitter - structure that handles code tokens 
 //! @param length - length of ID
 //! @return int 
 //!-------------------------------------------------


int HandleOperatorType1 (const char* buffer, TokenSplitter* splitter, unsigned int length);

 //!-------------------------------------------------
 //! @brief handle operators of type 2 
 //! 
 //! @param buffer - buffer with IsE cide
 //! @param splitter - structure that handles code tokens 
 //! @param length - length of ID
 //! @return int 
 //!-------------------------------------------------

int HandleOperatorType2 (const char* buffer, TokenSplitter* splitter, unsigned int length);


 //!-------------------------------------------------
 //! @brief handle functions and variable names 
 //! 
 //! @param buffer - buffer with IsE cide
 //! @param splitter - structure that handles code tokens 
 //! @param length - length of ID
 //! @return int 
 //!-------------------------------------------------

int HandleFunction (const char* buffer, TokenSplitter* splitter, unsigned int length);

