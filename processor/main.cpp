#include <stdlib.h>
#include <stdio.h>
#include "IsE_Processor.h"

int main(int argc, char *argv[])
{
    struct Processor processor;
    
    int nMembers = 0;
    char* storage;
    char* filename = "../asm/cpu.ISBC";

    readIsE_ByteCode (filename, &storage, &nMembers);

    //for ( int i = 0; i < nMembers; i++)
   //     printf ("%d\n", storage[i]);


    FILE* processorLogs = fopen ("processor_logs.txt", "w");

    processorConstruct (&processor, storage, nMembers, processorLogs);

    executeProcessorCommand (&processor);

    return 0;
}
