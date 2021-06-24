#include <stdio.h>
#include <stdlib.h>
#include "IsE_LinesF.h"
#include "IsE_Interpret.hpp"

int main(int argc, char* argv[])
{
    int nTokens = 0;//
    LogicElement* tokens = nullptr;
    SplitInputToTokens (argc, argv, &nTokens, &tokens);
    return 0;
}