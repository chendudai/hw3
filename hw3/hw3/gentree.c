#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"


/* definition of the element in the tree */
typedef struct _ELEMENT* PELEMENT;
typedef struct _ELEMENT
  pNode obj;
  PELEMENT* children;
  PELEMENT parent;
  int childrenCount;
} ELEMENT;

/* definition of the tree structure */    
typedef struct _tree{
  PELEMENT head;
  int nodes_num;
  int k;
  GetKeyFunction get_key;
  CloneFunction clone;
  PrintFunction print_node;
  DelFunction delete_node;
  /* *** complete the definition of the tree structure *** */
} Tree;

/* *** complete the interface functions implementation *** */
pTree TreeCreate(GetKeyFunction pGetKeyFunc, CloneFunction pCloneFunc, PrintFunction pPrintFunc, DelFunction pDelteFunc,int k)
{
	pTree my_tree = (pTree)malloc(sizeof(Tree));
	if (my_tree == NULL)
		return NULL;
	my_tree->head = NULL;
	my_tree->nodes_num = 0;
	my_tree->get_key = pGetKeyFunc;
	my_tree->clone = pCloneFunc;
	my_tree->print_node = pPrintFunc;
	my_tree->delete_node = pDelteFunc;
	my_tree->k = k;
	return my_tree;
}

void TreeDestroy(pTree my_tree)
{
	if (my_tree != NULL)
	//@@@@@@@@@@@@@@@@@@@@@ add free to Nodes
		free(my_tree);
}

int TreeNodesCount(pTree my_tree)
{
	return my_tree->nodes_num;
}

void TreePrint(pTree my_tree)
{
	pTree tmp = my_tree;
	if (tmp->head == NULL)
	{
		return;
	}
	tmp->print_node(my_tree->head->obj);
	for (int i = 0; i < my_tree->k; i++)
	{
		tmp->head = tmp->head->children[i];
		TreePrint(tmp);
	}
}

void TreeAddLeaf(pTree my_tree)
{


}
