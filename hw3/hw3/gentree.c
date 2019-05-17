#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"


/* definition of the element in the tree */
typedef struct _ELEMENT* PELEMENT;
typedef struct _ELEMENT{
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
} Tree;

PELEMENT find_key(pTree my_tree, int key);

//******************************************************************************
//* function name : TreeCreate
//* Description   : Creating the tree.
//* Parameters    : pGetKeyFunc - A pointer to a function for getting a key from a node.
//*					pCloneFunc - A pointer to a function that returns a pointer to a new copy of the node.
//*					pPrintFunc - A pointer to a function that prints the node.
//*					pDelteFunc - A pointer to a function that deletes the node.
//*					k- The number of maximum sons of a node.
//* Return value  : pTree - A pointer to the tree that was created.
//******************************************************************************

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


//******************************************************************************
//* function name : TreeDestroy
//* Description   : Deletes the tree.
//* Parameters    : pTree - A pointer to the tree.
//* Return value  : None
//******************************************************************************

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




Result TreeAddLeaf(pTree my_tree,int key,pNode new_node)
{
	if (my_tree->head == NULL)
	{
		my_tree->head = (PELEMENT)malloc(sizeof(ELEMENT));
		if (my_tree->head == NULL)
		{
			return FAILURE;
		}
		my_tree->head->parent = NULL;
		my_tree->head->childrenCount = 0;
		for (int i = 0; i < my_tree->k; i++)
		{
			my_tree->head->children[i] = NULL;
		}
		my_tree->head->obj = my_tree->clone(new_node);
		my_tree->nodes_num++;
		return SUCCESS;
	}
	else
	{
		PELEMENT element_with_key = find_key(my_tree, key);
		if (element_with_key == NULL)
			return FAILURE;
		if (element_with_key->childrenCount == my_tree->k) // Checking if the element with the key has k children.
			return FAILURE;
		for (int i = 0; i < my_tree->k - 1; i++)
		{
			if (my_tree->element_with_key->children[i] == NULL) // There is an empty space for a child.
			{
				my_tree->element_with_key->children[i] = (PELEMENT)malloc(sizeof(ELEMENT));
				if (my_tree->element_with_key->children[i] == NULL)
				{
					return FAILURE;
				}
				my_tree->head->children[i]->parent = my_tree->head;
				my_tree->head->children[i]->childrenCount = 0;
				my_tree->head->childrenCount++;
				for (int j = 0; j< my_tree->k; j++)
				{
					my_tree->head->children[i]->children[j] = NULL;
				}
				my_tree->element_with_key->children[i]->obj = my_tree->clone(new_node);
				my_tree->nodes_num++;
				return SUCCESS;
			}
		}
	}
}


PELEMENT find_key(pTree my_tree, int key)
{
	pTree tmp = my_tree;
	if (my_tree == NULL || my_tree->head==NULL)
		return NULL;
	if (tmp->get_key(tmp->head->obj) == key)
	{
		return tmp->head;
	}
	for (int i = 0; i < my_tree->k; i++)
	{
		tmp->head = tmp->head->children[i];
		if (find_key(tmp, key) != NULL)
			return tmp->head;
	}
	return NULL;
}




Result TreeNodeIsActive(pTree my_tree, int key, Bool* is_active)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return FAILURE;
	PELEMENT element_with_key = find_key(my_tree, key);
	if (element_with_key == NULL)
		return FAILURE;

	if (element_with_key->childrenCount < my_tree->k)
		*is_active = TRUE;
	else
		*is_active = FALSE;
	return SUCCESS;
}



Result TreeNodeIsLeaf(pTree my_tree, int key, Bool* is_leaf)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return FAILURE;
	PELEMENT element_with_key = find_key(my_tree, key);
	if (element_with_key == NULL)
		return FAILURE;

	if (element_with_key->childrenCount == 0)
		*is_leaf = TRUE;
	else
		*is_leaf = FALSE;
	return SUCCESS;

}


Result TreeDelLeaf(pTree my_tree, int key)
{
	Bool is_leaf=FALSE;
	if (TreeNodeIsLeaf(my_tree, key, &is_leaf) == SUCCESS)
	{
		if (is_leaf == TRUE)
		{
			PELEMENT element_with_key = find_key(my_tree, key);
			if (element_with_key == NULL)
				return FAILURE;
			my_tree->delete_node(element_with_key->obj);
			free(element_with_key);
			return SUCCESS;
		}
	}
	return FAILURE;
}

pNode TreeGetRoot(pTree my_tree)
{
	if (my_tree == NULL)
		return NULL;
	return my_tree->clone(my_tree->head->obj);
}

pNode TreeGetNode(pTree my_tree, int key)
{
	if (my_tree == NULL)
		return NULL;
	PELEMENT element_with_key = find_key(my_tree, key);
	if (element_with_key == NULL)
		return NULL;
	return my_tree->clone(element_with_key->obj);
}

pNode* TreeGetChildren(pTree my_tree, int key)
{
	if (my_tree == NULL)
		return NULL;

	PELEMENT element_with_key = find_key(my_tree, key);
	if (element_with_key == NULL)
		return NULL;


	pNode* arr_children_obj = (pNode*)(sizeof(pNode)*my_tree->k);
	if (arr_children_obj == NULL)
		return NULL;
	for (int i = 0; i < my_tree->k; i++)
	{
		arr_children_obj[i] = NULL;
	}
	
	for (int i = 0; i < element_with_key->childrenCount; i++)
	{
		arr_children_obj[i] = my_tree->clone(element_with_key->children[i]->obj);
	}
	return arr_children_obj;
}