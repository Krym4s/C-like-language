//
// Created by egolg on 20.10.2020.
//

#ifndef DED_ASM_ISE_ASSEMBLER_H
#define DED_ASM_ISE_ASSEMBLER_H

#include "IsE_Stack.h"
#include "IsE_text.h"

enum Registers
{
#define DEF_REG(name,value) name = value,
#include "registers.IsCPU"
#undef DEF_REG
};

enum AssemblerCodeErrors
{
    NO_ASSEMBLER_ERRORS             = 0,
    NULL_POINTER_ARGUMENT_ASSEMBLER = -1,
    NO_FREE_MEMORY_ASSEMBLER        = -2,
    COMPILING_ERROR                 = -3,
    TREE_LIST_ERROR                 = -4,
    EQUAL_LABELS                    = -5
};

struct Label
{
     char* name;
     int address;
     struct Label* left;
     struct Label* right;
};

struct EgorHeader
{
    unsigned signature;
    unsigned version;
    unsigned nMembers;
};

const unsigned int MAX_COMMAND_LENGTH = 100;

//!--------------------------------------------------
//!         read assembler code and splits it into lines
//!
//! \param [in] fileName - name of input file
//! \param [out] assemblerCode - pointer to array of lines
//! \param [out] storage - pointer to text of code
//! \param [out] nLines - number of lines
//! \return error code
//!--------------------------------------------------

int readAssemblerCodeLines (char* fileName, struct String** assemblerCode, char** storage, int* nLines);

//!---------------------------------------------------
//!     compile assembler code to IsE_ByteCode
//!
//! \param [in] assemblerCode - assembler code splited into lines
//! \param [out] IsE_ByteCode - pointer to byte interpretation of code
//! \param [in] nLines - number of lines
//! \param [out] nMembers - number of bytes in bytecode
//! \param [in] asmListing - file listing
//! \return error code
//!---------------------------------------------------

int translateAssemblerCode (String* assemblerCode, char** IsE_ByteCode,int nLines, unsigned int* nMembers, FILE* asmListing);

//!-----------------------------------------------------
//!     makes listing
//!
//! \param [in] begN - number of first byte
//! \param [in] nMembers - number of bytes on output
//! \param [in] bytes - array of bytes
//! \param [in] command - line that contains command
//! \param [in] asmListing - file listing
//! \return error code
//!-----------------------------------------------------

int assemblerListing (int begN, int nMembers, char* bytes,const char* command, FILE* asmListing);

//!------------------------------------------------
//!     write IsE-Byte code to file
//!
//! \param [in] ByteFileName - output file name
//! \param [in] IsE_ByteCode - byte interpretation of code
//! \param [in] version - number of version of this assembler
//! \param [in] nMembers - number of bytes in byte interpretation
//! \return errorcode
//!-------------------------------------------------

int createByteCode (char* ByteFileName, char* IsE_ByteCode, const unsigned int version,const unsigned int nMembers);

//!------------------------------------
//! return number of register
//!
//! \param [in] registerName - name of register
//! \return number of register
//!------------------------------------

int findReg (char* registerName);

//!----------------------------------------
//! function created to work with cmd
//!
//! \param [in] argc - number of args
//! \param [in] argv - args
//! \param [out] input - input file
//! \return errorcode
//!------------------------------------------

int findInput (int argc, char *argv[], char** input);

int findLabel (struct Label* labelArr, const int codeByte, char* command);

int addLeaf (struct Label** previousLabel, const int delta, const char* labelName, const int codeByte);

int labelNameCompare (struct Label** currLabel, struct Label** previousLabel, int delta, int* direction, const int codeByte);

int findAddress (struct Label labelArr, char* nameOfLabel, int* address);

int interpretAsmCode (int nLines, char** IsE_ByteCode, String* assemblerCode, FILE* asmListing, struct Label* labelArr, int codeByte);

#endif //DED_ASM_ISE_ASSEMBLER_H
