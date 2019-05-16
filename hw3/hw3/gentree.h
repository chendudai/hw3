#ifndef TREE_H
#define TREE_H

#include "defs.h"

/* complete the definition of the pointer to Tree */

typedef void* pNode;

typedef int (*GetKeyFunction)(pNode e);
typedef pNode (*CloneFunction)(pNode e);
typedef void (*PrintFunction)(pNode e);
typedef void (*DelFunction)(pNode e);

/* complete the definition of the interface functions */

#endif
