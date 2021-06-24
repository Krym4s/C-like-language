//
// Created by egolg on 14.12.2020.
//

#ifndef FRONTEND1_LOGICTREE_H
#define FRONTEND1_LOGICTREE_H

#include "IsE_LinesF.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

enum ElementType
{
    OPERATOR,
    VARIABLE,
    FUNCTION,
    NUMBER
};

enum Operations
{
#define FIRST_GROUP_OPERATOR(operatorName, operatorCode, textInterpret, codeInterpret) operatorName = operatorCode,
#include "data/1opgroup.txt"

#undef FIRST_GROUP_OPERATOR

#define SECOND_GROUP_OPERATOR(operatorName, operatorCode, textInterpret, codeInterpret) operatorName = operatorCode,
#include "data/2opgroup.txt"

#undef SECOND_GROUP_OPERATOR
};

enum LogicTreeError
{
    LOST_PARENT = -1,
    NO_TREE_ERRORS,
    DOUBLE_CREATE,
    NO_FREE_MEMORY,
    BAD_TREE_SIZE,
    WRONG_TYPE,
};

union ElementValue
{
    double numberValue;
    wchar_t* ID;
    char operatorType;
};

struct LogicTree;

struct LogicElement
{
    ElementType   type;
    ElementValue  value;
    LogicElement* parent;
    LogicElement* leftChild;
    LogicElement* rightChild;
    LogicTree* tree;
};

struct LogicTree
{
    unsigned int  size;
    LogicElement* root;
    FILE* graphicOutput;
    LogicTreeError error;

};

LogicTreeError LogicTreeConstruct (LogicTree* tree, FILE* graphicOutput);

LogicTreeError CreateTree (LogicTree* tree, ElementType type, ElementValue value);

LogicElement* CreateNode (LogicTree* tree, ElementType type, ElementValue value, LogicElement* parent, LogicElement* rightChild, LogicElement* leftChild);

LogicTreeError LogicTreeDump (LogicTree* tree);

void LogicDeclareVertex (FILE* graphicOutput, LogicElement* current);

char* typeName (ElementType type);

void LogicDrawVertex (FILE* graph_logs, LogicElement* current);

LogicTreeError LogicTreeVerify (LogicTree* tree);

int LogicDFSVerify (LogicElement* vertex);

void AddLeftChild (LogicElement* parent, LogicElement* left, LogicTree* tree);

void AddRightChild (LogicElement* parent, LogicElement* right, LogicTree* tree);

void currentElementOutput (LogicElement* root, FILE* output, int nTabs);

#endif //FRONTEND1_LOGICTREE_H
