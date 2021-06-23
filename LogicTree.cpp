//
// Created by egolg on 14.12.2020.
//

#include "LogicTree.h"
#include <clocale>

//#define DEBUG_V

#ifdef DEBUG_V
    #define TreeOK LogicTreeVerify (tree) || LogicTreeDump (tree);
#else
    #define TreeOK
#endif

LogicTreeError LogicTreeConstruct (LogicTree* tree, FILE* graphicOutput)
{
    tree->graphicOutput = graphicOutput;
    tree->size = 0;
    tree->root = nullptr;
    tree->error = NO_TREE_ERRORS;
    return NO_TREE_ERRORS;
}

LogicTreeError CreateTree (LogicTree* tree, ElementType type, ElementValue value)
{
    if (tree->root)
        return DOUBLE_CREATE;

    tree->root = CreateNode (tree, type, value, nullptr, nullptr, nullptr);
    tree->size ++;
    return NO_TREE_ERRORS;
}

LogicElement* CreateNode (LogicTree* tree, ElementType type, ElementValue value, LogicElement* parent, LogicElement* rightChild, LogicElement* leftChild)
{
    TreeOK
    LogicElement* current = (LogicElement*) calloc (1, sizeof (*current));
    if (!current)
        return nullptr;

    current->value = value;
    current->type = type;
    current->rightChild = rightChild;
    current->leftChild = leftChild;
    current->parent = parent;
    current->tree = tree;
    current->tree->size++;
    TreeOK
    return current;
}
char* Tranclate (wchar_t* rus)
{
    setlocale(LC_ALL, "Russian");
    char* eng = (char*) calloc (100, sizeof(char));
    char* write = eng;
    for (int i = 0; i < wcslen (rus); i++)
    {
            if (rus[i] == 'а' )     {*write++ = 'a'; }
            else if (rus[i] == 'б') {*write++ = 'b'; }
            else if (rus[i] == 'в') {*write++ = 'v'; }
            else if (rus[i] == 'г') {*write++ = 'g'; }
            else if (rus[i] == 'д') {*write++ = 'd'; }
            else if (rus[i] == 'е') {*write++ = 'e'; }
            else if (rus[i] == 'ж') {*write++ = 'j'; }
            else if (rus[i] == 'з') {*write++ = 'z'; }
            else if (rus[i] == 'и') {*write++ = 'i'; }
            else if (rus[i] == 'й') {*write++ = 'y'; }
            else if (rus[i] == 'к') {*write++ = 'k'; }
            else if (rus[i] == 'л') {*write++ = 'l'; }
            else if (rus[i] == 'м') {*write++ = 'm'; }
            else if (rus[i] == 'н') {*write++ = 'n'; }
            else if (rus[i] == 'о') {*write++ = 'o'; }
            else if (rus[i] == 'п') {*write++ = 'p'; }
            else if (rus[i] == 'р') {*write++ = 'r'; }
            else if (rus[i] == 'с') {*write++ = 's'; }
            else if (rus[i] == 'т') {*write++ = 't'; }
            else if (rus[i] == 'у') {*write++ = 'u'; }
            else if (rus[i] == 'ф') {*write++ = 'f'; }
            else if (rus[i] == 'х') {*write++ = 'h'; }
            else if (rus[i] == 'ц') {*write++ = 'c'; }
            else if (rus[i] == 'ч') {*write++ = 'c'; *write++ = 'h'; }
            else if (rus[i] == 'ш') {*write++ = 's'; *write++ = 'h'; }
            else if (rus[i] == 'щ') {*write++ = 's'; *write++ = 'h'; }
            else if (rus[i] == 'ы') {*write++ = 'y'; *write++ = 'i'; }
            else if (rus[i] == 'э') {*write++ = 'y'; *write++ = 'e'; }
            else if (rus[i] == 'ю') {*write++ = 'y'; *write++ = 'u'; }
            else if (rus[i] == 'я') {*write++ = 'y'; *write++ = 'a'; }
        
    }
    *write = '\0';
    return eng;
}

LogicTreeError LogicTreeDump (LogicTree* tree)
{
    if (!tree)
        printf ("there is no TREE. Executed on line");

    FILE* graphicOutput = tree->graphicOutput;

    fwprintf (graphicOutput, L"digraph LIST{\n");
    fwprintf (graphicOutput, L"\t" "rankdir = TB;\n");

    LogicDeclareVertex (graphicOutput, tree->root);
    LogicDrawVertex (graphicOutput, tree->root);
    printf("c\n");

    fwprintf (graphicOutput, L"}\n");

    fclose (graphicOutput);
    char command[100] = "";
    sprintf (command, "dot -Tpng graph.dot -o myGraph.png");
    system (command);
    system ("start myGraph.png");
    return NO_TREE_ERRORS;
}

void LogicDeclareVertex (FILE* graphicOutput, LogicElement* current)
{

    if (!current)
        return;
    printf("%d, %d\n", current->type, current->value.operatorType);
    fwprintf (graphicOutput, L"\"%p\"[shape=record, color=", current);
    if (current->leftChild || current->rightChild)
        fwprintf (graphicOutput, L"\"blue4\",style=\"filled\",fillcolor=\"aquamarine\"");
    else
        fwprintf (graphicOutput, L"\"green4\",style=\"filled\",fillcolor=\"green1\"");

    fwprintf (graphicOutput, L",label=\"   { current =  %p | left = %p | right = %p | tree = %p | type = %ls | value = ",
             current, current->leftChild, current->rightChild, current->tree, typeName (current->type));
    switch (current->type)
    {
        case VARIABLE: fprintf (graphicOutput, "%s", Tranclate(current->value.ID)); break;
        case NUMBER: fprintf (graphicOutput, "%lg", current->value.numberValue); break;
        case OPERATOR: //fwprintf (graphicOutput, L"%d", 1); break;
            switch (current->value.operatorType)
            {
                #define FIRST_GROUP_OPERATOR(operatorName, operatorCode, textInterpret, codeInterpret) case operatorCode: \
                            fprintf (graphicOutput, "%c", operatorCode); break;

                #include "1opgroup.txt"
                #undef FIRST_GROUP_OPERATOR
                #define SECOND_GROUP_OPERATOR(operatorName, operatorCode, textInterpret, codeInterpret) case operatorCode: \
                                            fprintf (graphicOutput, "%c", operatorCode); break;

                #include "2opgroup.txt"
                #undef SECOND_GROUP_OPERATOR

            } break;
        case FUNCTION: fprintf (graphicOutput, "%s", Tranclate(current->value.ID)); break;
    }
    fwprintf (graphicOutput, L"} \"];");


    LogicDeclareVertex (graphicOutput, current->rightChild);
    LogicDeclareVertex (graphicOutput, current->leftChild);
}



void LogicDrawVertex (FILE* graph_logs, LogicElement* current)
{
    printf("bbb");
    if (!current)
        return;

    if (current->leftChild)
    {
        fwprintf (graph_logs, L"\"%p\" -> \"%p\"[label=\"left\"];\n", current, current->leftChild);
        LogicDrawVertex (graph_logs, current->leftChild);
    }
    if (current->rightChild)
    {
        fwprintf (graph_logs, L"\"%p\" -> \"%p\"[label=\"right\"];\n", current, current->rightChild);
        LogicDrawVertex (graph_logs, current->rightChild);
    }
}

#define ARGNAME(x) #x
char* typeName (ElementType type)
{
    switch (type)
    {
        case OPERATOR:  return ARGNAME(OPERATOR);
        case NUMBER:    return ARGNAME(NUMBER);
        case VARIABLE:  return ARGNAME(VARIABLE);
        case FUNCTION:  return ARGNAME(FUNCTION);
        default:
            return "NO_TYPE";
    }
}
#undef ARGNAME

LogicTreeError LogicTreeVerify (LogicTree* tree)
{
    int nVertexes = LogicDFSVerify (tree->root);                                                                                                                                                                                    *(int*)0 = 10;
    if (nVertexes == LOST_PARENT)
        return LOST_PARENT;

    if (tree->size != nVertexes)
        return BAD_TREE_SIZE;

    if (nVertexes == WRONG_TYPE)
        return WRONG_TYPE;

    return NO_TREE_ERRORS;                                                                                                                              *(int*)0 = 10;
}

int LogicDFSVerify (LogicElement* vertex)
{
    int nRightVertexes = 0;
    int nLeftVertexes  = 0;

    if (vertex->rightChild)
    {
        if (vertex->tree != vertex->rightChild->tree)
            return LOST_PARENT;

        if (vertex->type != OPERATOR || vertex->type != FUNCTION)
            return WRONG_TYPE;

        nRightVertexes = LogicDFSVerify (vertex->rightChild);
    }
    if (vertex->leftChild)
    {
        if (vertex->tree != vertex->leftChild->tree || vertex->type != FUNCTION)
            return LOST_PARENT;

        nLeftVertexes = LogicDFSVerify (vertex->leftChild);

        if (vertex->type != OPERATOR)
            return WRONG_TYPE;

    }
    if (nRightVertexes == LOST_PARENT || nLeftVertexes == LOST_PARENT)
        return LOST_PARENT;

    if (nRightVertexes == WRONG_TYPE || nLeftVertexes == WRONG_TYPE)
        return WRONG_TYPE;

    return nLeftVertexes + nRightVertexes + 1;
}

void AddLeftChild (LogicElement* parent, LogicElement* left, LogicTree* tree)
{
    TreeOK
    parent->leftChild = left;
    left->parent = parent;
    parent->tree = tree;
    left->tree = tree;
    tree->size++;
    TreeOK
}

void AddRightChild (LogicElement* parent, LogicElement* right, LogicTree* tree)
{
    TreeOK
    parent->rightChild = right;
    right->parent = parent;
    parent->tree = tree;
    right->tree = tree;
    tree->size++;
    TreeOK
}

#undef TreeOK
