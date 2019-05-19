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
struct _tree{
  PELEMENT head;
  int nodes_num;
  int k;
  GetKeyFunction get_key;
  CloneFunction clone;
  PrintFunction print_node;
  DelFunction delete_node;
};

PELEMENT find_key(pTree my_tree, PELEMENT p_element, int key);
void TreePrint_rec(pTree my_tree, PELEMENT p_element);
void TreeDestroy_rec(pTree my_tree, PELEMENT p_element);

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
	if (k <= 0 || !pGetKeyFunc || !pCloneFunc || !pPrintFunc || !pDelteFunc)
		return NULL;
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
//* Parameters    : my_tree - A pointer to the tree.
//* Return value  : None
//******************************************************************************

void TreeDestroy(pTree my_tree)
{
	if (my_tree == NULL)
		return;
	if (my_tree->head == NULL)
	{
		free(my_tree);
		return;
	}

	TreeDestroy_rec(my_tree, my_tree->head);
	free(my_tree);
}

void TreeDestroy_rec(pTree my_tree, PELEMENT p_element)
{
	if (p_element == NULL)
		return;
	for (int i = 0; i < my_tree->k; i++)
	{
		TreeDestroy_rec(my_tree, p_element->children[i]);
	}
	my_tree->delete_node(p_element->obj);
	 if(p_element->children != NULL)
			free(p_element->children);
	free(p_element);
}



//******************************************************************************
//* function name : TreeNodesCount
//* Description   : Returns the number of nodes in the tree.
//* Parameters    : my_tree - A pointer to the tree.
//* Return value  : int - the number of nodes in the tree.
//******************************************************************************

int TreeNodesCount(pTree my_tree)
{
	return my_tree->nodes_num;
}

//******************************************************************************
//* function name : TreePrint
//* Description   : Prints the nodes in the tree (pre-order)
//* Parameters    : my_tree - A pointer to the tree.
//* Return value  : None
//******************************************************************************

void TreePrint(pTree my_tree)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return;
	pTree tmp = my_tree;
	TreePrint_rec(my_tree, my_tree->head);

}

void TreePrint_rec(pTree my_tree, PELEMENT p_element)
{
	if (p_element == NULL)
		return;
	my_tree->print_node(p_element->obj);
	if (p_element->childrenCount > 0) //This is not a leaf
	{
		for (int i = 0; i < my_tree->k; i++)
		{
			if (p_element->children[i] != NULL)
			{
				TreePrint_rec(my_tree, p_element->children[i]);
			}
		}
	}
}



//******************************************************************************
//* function name : TreeAddLeaf
//* Description   : Adds leaf to the tree .The leaf is new_node. The parent of the leaf is the node that has the key in the input.
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the parent of the leaf.
//*			        new_node - A pointer to the leaf.
//* Return value  : Result - Returns FAILURE if the adding of the leaf didn't succeed. Otherwise it returns SUCCESS.
//******************************************************************************


Result TreeAddLeaf(pTree my_tree,int key,pNode new_node)
{
	if (my_tree == NULL || new_node == NULL) //checking if the pointer of the input are NULL
		return FAILURE;
	if (my_tree->head == NULL) // If the tree is empty.
	{
		my_tree->head = (PELEMENT)malloc(sizeof(ELEMENT));
		if (my_tree->head == NULL) //checking allocation
		{
			return FAILURE;
		}
		my_tree->head->parent = NULL;
		my_tree->head->childrenCount = 0;
		my_tree->head->children = (PELEMENT*)malloc(sizeof(PELEMENT)*my_tree->k);
		if (my_tree->head->children == NULL)
		{
			free(my_tree->head);
			return FAILURE;
		}
		for (int i = 0; i < my_tree->k; i++)
		{
			my_tree->head->children[i] = NULL;
		}
		my_tree->head->obj = my_tree->clone(new_node);
		my_tree->nodes_num++;

		return SUCCESS;
	}
	else //If the tree is not empty.
	{
		PELEMENT element_with_key = find_key(my_tree, my_tree->head, key);
		if (element_with_key == NULL)
			return FAILURE;
		if (element_with_key->childrenCount == my_tree->k) // Checking if the element with the key has k children.
			return FAILURE;

		for (int i = 0; i < my_tree->k; i++)
		{
			if (element_with_key->children[i] == NULL)
			{
				element_with_key->children[i] = (PELEMENT)malloc(sizeof(ELEMENT));
				if (element_with_key->children[i] == NULL) //checking allocation
				{
					return FAILURE;
				}
				element_with_key->children[i]->children = (PELEMENT*)malloc(sizeof(PELEMENT)*my_tree->k);
				if (element_with_key->children[i]->children == NULL)
				{
					return FAILURE;
				}
				for (int j = 0; j < my_tree->k; j++)
				{
					element_with_key->children[i]->children[j] = NULL;
				}
				element_with_key->children[i]->parent = element_with_key;
				element_with_key->children[i]->childrenCount = 0;
				element_with_key->childrenCount++;
				element_with_key->children[i]->obj = my_tree->clone(new_node);
				my_tree->nodes_num++;
				return SUCCESS;

			}

		}
		return FAILURE;
	}
	return FAILURE;


}



//******************************************************************************
//* function name : find_key
//* Description   : Finds a node in the my_tree that has the same key value.
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the node.
//* Return value  : Returns a pointer to the element of the node.
//******************************************************************************

PELEMENT find_key(pTree my_tree,PELEMENT p_element, int key)
{
	if (my_tree == NULL || p_element ==NULL || p_element->obj == NULL)
		return NULL;
	if (my_tree->get_key(p_element->obj) == key)
	{
		return p_element;
	}
	for (int i = 0; i < my_tree->k; i++)
	{
		PELEMENT element_with_key = find_key(my_tree, p_element->children[i], key);
		if (element_with_key != NULL)
			return element_with_key;
	}
	return NULL;
}


//******************************************************************************
//* function name : TreeNodeIsActive
//* Description   : Checks if the node that has the key value is active.
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the node.
//*			        is_active - A pointer that is TRUE if the node is active. Otherwise it is FALSE.
//* Return value  : Result - Returns FAILURE if the adding of the leaf didn't succeed. Otherwise it returns SUCCESS.
//******************************************************************************

Result TreeNodeIsActive(pTree my_tree, int key, Bool* is_active)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return FAILURE;
	PELEMENT element_with_key = find_key(my_tree, my_tree->head, key);
	if (element_with_key == NULL)
		return FAILURE;

	if (element_with_key->childrenCount < my_tree->k)
		*is_active = TRUE;
	else
		*is_active = FALSE;
	return SUCCESS;
}


//******************************************************************************
//* function name : TreeNodeIsLeaf
//* Description   : Checks if the node that has the key value is a leaf.
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the node.
//*			        is_leaf - A pointer that is TRUE if the node is a leaf. Otherwise it is FALSE.
//* Return value  : Result - Returns FAILURE if the adding of the leaf didn't succeed. Otherwise it returns SUCCESS.
//******************************************************************************


Result TreeNodeIsLeaf(pTree my_tree, int key, Bool* is_leaf)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return FAILURE;
	PELEMENT element_with_key = find_key(my_tree, my_tree->head, key);
	if (element_with_key == NULL)
		return FAILURE;

	if (element_with_key->childrenCount == 0)
		*is_leaf = TRUE;
	else
		*is_leaf = FALSE;
	return SUCCESS;

}


//******************************************************************************
//* function name : TreeDelLeaf
//* Description   : Delets the node that has the key value (only if the node is a leaf).
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the node.
//* Return value  : Result - Returns FAILURE if the adding of the leaf didn't succeed. Otherwise it returns SUCCESS.
//******************************************************************************

Result TreeDelLeaf(pTree my_tree, int key)
{
	Bool is_leaf=FALSE;
	if (TreeNodeIsLeaf(my_tree, key, &is_leaf) == SUCCESS)
	{
		if (is_leaf == TRUE)
		{
			PELEMENT element_with_key = find_key(my_tree, my_tree->head, key);
			if (element_with_key == NULL)
				return FAILURE;

			if (element_with_key->parent != NULL)
			{
				for (int i = 0; i < my_tree->k; i++) //Putting NULL in the children array of the parent of the the deleted leaf  
				{
					if (element_with_key->parent->children[i] != NULL && my_tree->get_key(element_with_key->parent->children[i]->obj) == key) {
						element_with_key->parent->children[i] = NULL;
					}
				}
			}

			else //element_with_key->parent == NULL. We are deleting the first node of the tree.
			{
				my_tree->head = NULL;
			}

			my_tree->delete_node(element_with_key->obj);
			if (element_with_key->children != NULL)
				free(element_with_key->children);
			if (element_with_key->parent != NULL)
			{
				element_with_key->parent->childrenCount--;
			}
			my_tree->nodes_num--;
			free(element_with_key);
			return SUCCESS;
		}
	}
	return FAILURE;
}

//******************************************************************************
//* function name : TreeGetRoot
//* Description   : Returns a pointer to the root of the tree.
//* Parameters    : my_tree - A pointer to the tree.
//* Return value  : Returns a pointer to the root of the tree.
//******************************************************************************

pNode TreeGetRoot(pTree my_tree)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return NULL;
	return my_tree->clone(my_tree->head->obj);
}

//******************************************************************************
//* function name : TreeGetNode
//* Description   : Returns a pointer to a node with a key value.
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the node.
//* Return value  : Returns a pointer to a node with a key value.
//******************************************************************************

pNode TreeGetNode(pTree my_tree, int key)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return NULL;
	PELEMENT element_with_key = find_key(my_tree, my_tree->head, key);
	if (element_with_key == NULL)
		return NULL;
	return my_tree->clone(element_with_key->obj);
}


//******************************************************************************
//* function name : TreeGetChildren
//* Description   : Returns an array of pointers to the childern nodes.
//* Parameters    : my_tree - A pointer to the tree.
//*			        key - The key of the node that the function gets his children.
//* Return value  :  Returns an array of pointers to the childern nodes.
//******************************************************************************

pNode* TreeGetChildren(pTree my_tree, int key)
{
	if (my_tree == NULL || my_tree->head == NULL)
		return NULL;

	PELEMENT element_with_key = find_key(my_tree, my_tree->head, key);
	if (element_with_key == NULL)
		return NULL;

	pNode* arr_children_obj = (pNode*)malloc(sizeof(pNode) * (my_tree->k));
	if (arr_children_obj == NULL)
		return NULL;

	
	for (int i = 0; i < my_tree->k; i++)
	{
		if (element_with_key->children[i] == NULL)
		{
			arr_children_obj[i] = NULL;
		}
		else
		{
			arr_children_obj[i] = my_tree->clone(element_with_key->children[i]->obj);
		}
	}
	return arr_children_obj;
}