#ifndef TREE_H
#define TREE_H

#include "defs.h"

/* complete the definition of the pointer to Tree */
typedef Tree* pTree;
typedef void* pNode;

typedef int (*GetKeyFunction)(pNode e);
typedef pNode (*CloneFunction)(pNode e);
typedef void (*PrintFunction)(pNode e);
typedef void (*DelFunction)(pNode e);

/* complete the definition of the interface functions */
pTree TreeCreate(GetKeyFunction pGetKeyFunc, CloneFunction pCloneFunc, PrintFunction pPrintFunc, DelFunction pDelteFunc, int k);
void TreeDestroy(pTree my_tree);
int TreeNodesCount(pTree my_tree);

#endif
