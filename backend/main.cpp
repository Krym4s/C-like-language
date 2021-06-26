#include "TreeToAsm.h"
#include "IsE_LinesF.h"

int main()
{
    char* buffer = nullptr;
    readToStorage("../frontend/output.txt", &buffer);
    printf ("%s", buffer);
    LogicTree tree = {};
    FILE* grDump = fopen("graph.txt", "w");
    LogicTreeConstruct (&tree, grDump);
    CreateLogicTree(buffer, &tree);
    //LogicTreeDump(&tree);
    CreateAsmCode (&tree, "myasm.txt");

    return 0;
}
