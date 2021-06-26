//
// Created by egolg on 20.12.2020.
//

#include "TreeToAsm.h"
unsigned int MAX_LIT_SIZE = 100;
unsigned int RECOMMENDED_VOCABULARY_SIZE = 10;
unsigned int ifn = 0;


//--------------------vocabulary-section---------------

int VocabularyConstruct (Vocabulary* vocabulary)
{
    vocabulary->size = RECOMMENDED_VOCABULARY_SIZE;
    vocabulary->vocabulary = (char**) calloc (RECOMMENDED_VOCABULARY_SIZE, sizeof(char*));
    vocabulary->index = 0;
    return 0;
}

int IsDeclared (Vocabulary* vocabulary, char* name)
{
    for (int i = 0; i < vocabulary->index; i++)
        if (strcmp (name, vocabulary->vocabulary[i]) == 0)
            return 1;
    return 0;
}

int EnlargeVocabularyIfNecessary (Vocabulary* vocabulary)
{
    wchar_t** temp = nullptr;
    if (vocabulary->index == vocabulary->size - 1)
    {
        temp = (wchar_t**) realloc (vocabulary->vocabulary, vocabulary->size * 2);
        if (!temp)
            return 1;
        vocabulary->size *= 2;
    }
    return 0;
}

int InsertInto (Vocabulary* vocabulary, char* name)
{

    if (!IsDeclared (vocabulary, name))
    {
        EnlargeVocabularyIfNecessary (vocabulary);
        vocabulary->vocabulary[vocabulary->index++] = name;
        return 1;
    }
    return 0;
}

int GetNameIdx (Vocabulary* vocabulary, char* name)
{
    for (int i = 0; i < vocabulary->index; i++)
    {
        printf ("%s\n", vocabulary->vocabulary[i]);
        if (strcmp (name, vocabulary->vocabulary[i]) == 0)
            return i;
    }

    return -1;
}

//-------------read-section---------------

void CreateLogicTree (char* buffer, LogicTree* tree)
{
    unsigned int index = 0;
    tree->root = ReadNewNode (buffer, tree, nullptr, &index);
}

LogicElement* ReadNewNode (char* buffer, LogicTree* tree, LogicElement* parent, unsigned int* index)
{

    int operatorType = 0;
    double value = 0;
    char* literal = (char*) calloc (MAX_LIT_SIZE, sizeof(*literal));
    LogicElement* current = nullptr;

        while (*(buffer + *index) != '{')
            (*index)++;

        (*index)++;
        while (isspace(*(buffer + *index)))
            (*index)++;

    ElementValue* val = (ElementValue*) calloc (1, sizeof(*val));

        switch (*(buffer + *index))
        {
            case '\'':
                sscanf ((buffer + *index + 1), "%c", &operatorType);

                val->operatorType = operatorType;
                current = CreateNode (tree, OPERATOR, val, parent, nullptr, nullptr);
                break;
            case '$' :
                sscanf ((buffer + *index + 1), "%s", literal);
                val->ID = literal;
                current = CreateNode (tree, FUNCTION, val, parent, nullptr, nullptr);
                break;
            case '#' :
                sscanf ((buffer + *index + 1), "%s", literal);
                val->ID = literal;
                current = CreateNode (tree, VARIABLE, val, parent, nullptr, nullptr);
                break;
            case '?' :
                sscanf ((buffer + *index + 1), "%lg", &value);
                val->numberValue = value;
                current = CreateNode (tree, NUMBER, val, parent, nullptr, nullptr);
                break;
            case 'n' :
                while (*(buffer + *index) != '}')
                    (*index)++;
                return nullptr;
        }
        while (*(buffer + *index) != '{')
            (*index)++;

        current->leftChild = ReadNewNode (buffer, tree, current, index);

        while (*(buffer + *index) != '{')
            (*index)++;
        current->rightChild  = ReadNewNode (buffer, tree, current, index);
        while (*(buffer + *index) != '}')
            (*index)++;

        return current;

}

//------------asm-creating-section----------------

int CreateAsmCode (LogicTree* tree, char* fileName)
{
    FILE* output = fopen (fileName, "w");
    fprintf (output, "PUSH 0; set begin\n"
                     "POP rax\n"
                     "PUSH 1\n"
                     "POP rcx\n\n");
    printf ("creating\n");
    FunctionDeclareHandler (tree->root, output);
    LogicElement* current = tree->root->rightChild;
    while (current)
    {
        FunctionDeclareHandler (current, output);
        current = current->rightChild;
    }
    return 0;
}

int FunctionDeclareHandler (LogicElement* current, FILE* output)
{
    printf ("function declare\n");

    LogicElement* function = current->leftChild;
    printf ("uf\n");
    fprintf (output, "%s:", function->value.ID);
    Vocabulary variables = {};
    VocabularyConstruct (&variables);
    if (function->leftChild)
        HandleArguments (function->leftChild, output, &variables);

    HandleBody (function->rightChild, output, &variables);
    if (current->tree->root == current)
        fprintf (output, "HLT; halt\n");
}

int HandleArguments (LogicElement* current, FILE* output, Vocabulary* variables)
{
    printf ("handle arguments\n");
    if (current->type == OPERATOR && current->value.operatorType == VAR)
    {
        LogicElement* var = current->rightChild;
        if (InsertInto (variables, var->value.ID))
        {
            if (current->leftChild)
                return HandleArguments (current->leftChild, output, variables);
            return 1;
        }
    }
    return 0;
}

int HandleBody (LogicElement* current, FILE* output, Vocabulary* variables)
{
    printf ("handle body\n");
    fprintf (output, "\n");
    HandleElement (current, output, variables);
    fprintf (output, "\n");
    return 0;
}

int HandleElement (LogicElement* current, FILE* output, Vocabulary* variables)
{
    printf ("handle element: %d ", current->type);
    switch (current->type)
    {
        case OPERATOR: OperatorHandler (current, output, variables); break;
        case VARIABLE: VarHandler (current, output, variables); break;
        case FUNCTION: FunctionHandler (current, output, variables); break;
        case NUMBER: NumberHandler (current, output, variables); break;
    }
    return 0;
}

int OperatorHandler (LogicElement* current, FILE* output, Vocabulary* vocabulary)
{
    printf ("handle operator\n");
    int cInd = 0;
    switch (current->value.operatorType)
    {
        case ADDITION:
            HandleElement (current->leftChild, output, vocabulary);
            HandleElement (current->rightChild, output, vocabulary);
            fprintf (output, "ADD\n"); break;
        case SUBTRACTION:
            HandleElement (current->leftChild, output, vocabulary);
            HandleElement (current->rightChild, output, vocabulary);
            fprintf (output, "SUB\n"); break;
        case MULTIPLICATION:
            HandleElement (current->leftChild, output, vocabulary);
            HandleElement (current->rightChild, output, vocabulary);
            fprintf (output, "MUL\n"); break;
        case DIVISION:
            HandleElement (current->leftChild, output, vocabulary);
            HandleElement (current->rightChild, output, vocabulary);
            fprintf (output, "DIV\n"); break;
        case POWER:
            HandleElement (current->leftChild, output, vocabulary);
            HandleElement (current->rightChild, output, vocabulary);
            fprintf (output, "POW\n"); break;
        case SIN:
            HandleElement (current->leftChild, output, vocabulary);
            fprintf (output, "SIN\n"); break;
        case COS:
            HandleElement (current->leftChild, output, vocabulary);
            fprintf (output, "COS\n"); break;
        case PRINT:
            HandleElement (current->leftChild, output, vocabulary);
            fprintf (output, "OUT\n"); break;
        case SCAN:
            cInd = GetNameIdx (vocabulary, current->leftChild->value.ID);
            fprintf (output, "IN\n"
                             "POP [ rax + %d ]; accorded to IN\n", cInd + 1); break;
        case ASSIGNMENT:
            HandleElement (current->rightChild, output, vocabulary);
            printf("%s", current->leftChild->value.ID);
            cInd = GetNameIdx (vocabulary, current->leftChild->value.ID);
            printf ("%d\n", cInd);
            fprintf (output, "POP [ rax + %d ]; accorded to asignment\n", cInd + 1); break;
        case RETURN:
            if (current->leftChild)
                HandleElement (current->leftChild, output, vocabulary);

            fprintf (output, "PUSH rax ; ret prepare\n"
                             "POP rcx\n"
                             "PUSH [ rax ]\n"
                             "POP rax\n");
            fprintf (output, "RET\n\n"); break;
        case VAR:
            InsertInto (vocabulary, current->rightChild->value.ID);
            IterFree (output);
            break;
        case IF:
            HandleElement (current->leftChild->leftChild, output, vocabulary);
            HandleElement (current->leftChild->rightChild, output, vocabulary);
            HandleLogicOperator(current->leftChild, output, vocabulary);
            HandleBody (current->rightChild, output, vocabulary);
            fprintf (output, "%d:\n", ifn);
            ifn++;
            break;
        case WHILE:
            fprintf (output, "%d:\n", ifn + 1);
            HandleElement (current->leftChild->leftChild, output, vocabulary);
            HandleElement (current->rightChild->rightChild, output, vocabulary);
            HandleLogicOperator(current->leftChild, output, vocabulary);
            ifn++;
            HandleBody (current->rightChild, output, vocabulary);
            fprintf (output, "JMP : %d\n", ifn);
            fprintf (output, "%d:\n", ifn - 1);
            break;
        case LR:
            if (current->leftChild)
                HandleElement (current->leftChild, output, vocabulary);

            if (current->rightChild)
                HandleElement (current->rightChild, output, vocabulary);
            break;
        default: return 0;
    }
    return 0;
}

int HandleLogicOperator (LogicElement* current, FILE* output, Vocabulary* vocabulary)
{
    printf ("handle logic operator\n");
    switch (current->value.operatorType)
    {
        case EQUALITY: fprintf (output, "JNE : %d\n", ifn); break;
        case INEQUALITY: fprintf (output, "JE : %d\n", ifn); break;
        case MORE: fprintf (output, "JBE : %d\n", ifn); break;
        case LESS: fprintf (output, "JAE : %d\n", ifn); break;
        case MORE_OR_EQUAL: fprintf (output, "JB : %d\n", ifn); break;
        case LESS_OR_EQUAL: fprintf (output, "JA : %d\n", ifn); break;
        default: return 1;
    }
    return 0;
}

int FunctionHandler (LogicElement* current, FILE* output, Vocabulary* vocabulary)
{
    printf ("handle function\n");
    Vocabulary variables = {};
    VocabularyConstruct (&variables);
    fprintf (output, "PUSH rcx; remember rcx\n"
                     "POP rbx\n\n");
    AddArgs (current->leftChild, output, vocabulary);
    fprintf (output,"PUSH rax; move stack idx\n"
             "POP [ rbx ]\n"
             "PUSH rbx\n"
             "POP rax\n\n");
    //IterFree (output);
    fprintf (output, "CALL : %s\n", current->value.ID);
}

int AddArgs (LogicElement* current, FILE* output, Vocabulary* vocabulary)
{
    printf ("adding args\n");
    int nOfArgs = 0;
    while (current)
    {
        printf ("as\n");
        HandleElement (current->leftChild, output, vocabulary);
        fprintf (output, "POP [ rbx + %d ]; arg of called function\n", nOfArgs + 1);
        IterFree (output);
        current = current->rightChild;
        nOfArgs++;
    }
    return nOfArgs;
}

void IterFree (FILE* output)
{
    fprintf (output, "\nPUSH rcx;iteration \n"
                     "PUSH 1\n"
                     "ADD\n"
                     "POP rcx\n\n");
}

int VarHandler (LogicElement* current, FILE* output, Vocabulary* vocabulary)
{
    printf ("handle var\n");
    int idx = GetNameIdx (vocabulary, current->value.ID);
    fprintf (output, "PUSH [ rax + %d ]; variable get\n", idx + 1);
    return 0;
}

int NumberHandler (LogicElement* current, FILE* output, Vocabulary* vocabulary)
{
    printf ("handle number\n");
    fprintf (output, "PUSH %lg; number get\n", current->value.numberValue);
    return 0;
}