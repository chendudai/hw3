#ifndef TREE_H
#define TREE_H

#include "defs.h"

/* complete the definition of the pointer to Tree */
typedef void* pNode;
typedef struct _tree Tree;
typedef struct _tree* pTree;

typedef int (*GetKeyFunction)(pNode e);
typedef pNode (*CloneFunction)(pNode e);
typedef void (*PrintFunction)(pNode e);
typedef void (*DelFunction)(pNode e);

/* complete the definition of the interface functions */
pTree TreeCreate(GetKeyFunction pGetKeyFunc, CloneFunction pCloneFunc, PrintFunction pPrintFunc, DelFunction pDelteFunc, int k);
void TreeDestroy(pTree my_tree);
int TreeNodesCount(pTree my_tree);
void TreePrint(pTree my_tree);
Result TreeAddLeaf(pTree my_tree, int key, pNode new_node);
Result TreeNodeIsActive(pTree my_tree, int key, Bool* active);
Result TreeNodeIsLeaf(pTree my_tree, int key, Bool* active);
Result TreeDelLeaf(pTree my_tree, int key);
pNode TreeGetRoot(pTree my_tree);
pNode TreeGetNode(pTree mytree, int key);
pNode* TreeGetChildren(pTree my_tree, int key);

#endif
