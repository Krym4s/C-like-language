//
// Created by egolg on 20.10.2020.
//

#include "IsE_Processor.h"
#include <math.h>

const double EPSILON = 10e-7;

FILE* logs = fopen ("logs_stack.txt","w");



int processorInit (struct Processor* thou)
{
    assert(thou);

    thou->stackBuff = newStack();
    thou->stackFunc = newStack();

    stackInit(thou->stackBuff);
    stackInit(thou->stackFunc);

    thou->nMembers = 0;
    thou->code = {};
    thou->rip = 0;

    FILE* processorLogs = fopen ("processor_logs.txt","w");

    stackConstructor (thou->stackBuff, 100, processorLogs);
    stackConstructor (thou->stackFunc, 100, processorLogs);

    for (int k = 0; k < 4; k++)
        thou->reg[k];

    return NO_ERRORS;

}

int processorConstruct (struct Processor* thou, char* byteCode, int nOfMembers, const FILE* processorLogs)
{
    assert(thou);
    assert(byteCode);
    assert (processorLogs);

    thou->stackBuff = newStack ();
    thou->stackFunc = newStack();

    thou->rip = 0;
    thou->nMembers = nOfMembers;

    for (int k = 0; k < 4; k++)
        thou->reg[k];

    stackConstructor (thou->stackBuff, 100, processorLogs);
    stackConstructor (thou->stackFunc, 100, processorLogs);
    thou->code = byteCode;

    return NO_ERRORS;
}

int readIsE_ByteCode (const char* filename, char** storage, int* nMembers)
{
    assert (filename);
    assert (storage);
    assert (nMembers);

    readToStorage (filename, storage);

    IsE_Header* header = (IsE_Header*) storage;
    int errorCode;
    if (errorCode == isExecutable (header))
        return errorCode;


   *nMembers = *(int*)(*storage + 8);
   //*nMembers = header->nMembers;
   *storage = (*storage + sizeof(IsE_Header));

   return errorCode;
}

int executeProcessorCommand (struct Processor* processor)
{
    assert (processor); //printf ("%d", processor->nMembers);

   /* txCreateWindow (200,200);
    txTextCursor (false);*/
    int p = 0;
    for (processor->rip = 0; processor->rip < processor->nMembers; p++)
    {
        char command = processor->code[processor->rip++]; //printf("%d\n",command);
        switch (command)
        {
            #define DEF_CPU(name,value,nParams,instruction,readAsmArg,writeDisAsmArg) case value: instruction break;

#include "processor_commit.IsCPU"

#undef DEF_REG
            default:
                printf ("There is no such command %x\n", processor->rip);
                processorDump (processor, ARGNAME(processor->errorCode), __LINE__);
        }

           /* for (int a = 0; a < 200; a++)
                for (int b = 0; b < 200; b++) {

                    txSetPixel (a, b, RGB(256 - processor->RAM[200 * a + b ], (256 + processor->RAM[200 * a + b ])/2,
                                          processor->RAM[200 * a + b ]));
                }*/
        //printf ("%d\n", p);
    }
    //txSleep(10000);

    return NO_ERRORS;
}

int processorDump (struct Processor* processor, char* reason, int line)
{
    assert (processor);
    assert (reason);

    if (!processor)
        return NULL_POINTER_IN_ARGUMENT;


    fprintf (logs, "executed on line %d\n",   line);
    fprintf (logs, "Processor(%s) [%p] \n { \n",  reason, processor);
    fprintf (logs, "\t" "nMembers  = %zux\n", processor->nMembers);
    fprintf (logs, "\t" "rip       = %zu\n",  processor->rip);
    fprintf (logs, "\t" "errorCode = %d\n",   processor->errorCode);
    for (int k = 0; k < 4; k++)
        fprintf (logs, "\t" "rax       = %lg\n",  processor->reg[k]);

    for (int i = 0; i < processor->nMembers; ++i)
        fprintf (logs, "\t\t" "[%d] %lg\n", i, *(processor->code + i));

    stackDump (processor->stackBuff, ARGNAME(processor->stack->errcode), line);
    stackDump (processor->stackFunc, ARGNAME(processor->stack->errcode), line);

}

int findReg (char** answerLine, int regN)
{
    #define DEF_REG(name,value) case value: *answerLine = #name; break;
    switch (regN)
    {
        #include "registers.IsCPU"

        default: strcat (*answerLine, "There is no such register\n");
    }
    #undef DEF_REG
}

int isExecutable (struct IsE_Header* header)
{
    if (header->signature != reference.signature)
        return WRONG_SIGNATURE;

    if (header->version != reference.version)
        return WRONG_VERSION;

    return NO_ERRORS;
}



