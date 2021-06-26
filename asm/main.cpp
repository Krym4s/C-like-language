#include <stdio.h>
#include <stdlib.h>
#include "IsE_Assembler.h"

const unsigned VERSION = 1;

int main(int argc, char *argv[])
{
    char* filename = "";
    findInput (argc, argv, &filename);
    //printf ("%s", filename);
    struct String* assemblerCode;
    char* storage = "";
    int nLines = 0;

    readAssemblerCodeLines (filename,&assemblerCode, &storage, &nLines);

    char* IsE_ByteCode;
    unsigned int nMember;

    FILE* asmListing = fopen ("asmListing.txt", "w");

    translateAssemblerCode (assemblerCode, &IsE_ByteCode, nLines, &nMember, asmListing);
    createByteCode ("cpu.ISBC", IsE_ByteCode, VERSION, nMember);

    fclose (asmListing);
    return 0;
}
