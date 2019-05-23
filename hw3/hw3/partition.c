#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"

pTree my_partition=NULL;
int numOfNodes = 1;


typedef struct _SQUARE {
	double x_i,x_f,y_i,y_f;//square vertexes
	int key;//the key of each node will be the x cord of the square center
} SQUARE, *pSQUARE;

//******************************************************************************
//* function name : getKeyFun
//* Description   : Returns a node's key.
//* Parameters    : poninter to the node.
//* Return value  : key(int)
//******************************************************************************
int getKeyFunc(pNode e)
{
	if (e == NULL)
	{
		return -1;
	}
	pSQUARE my_node = (pSQUARE)(e);
	return my_node->key;
}

//******************************************************************************
//* function name : cloneFunc
//* Description   : Returns a node clone.
//* Parameters    : poninter to node.
//* Return value  : poninter to the cloned node.
//******************************************************************************
pNode cloneFunc(pNode e)
{
	if (e == NULL)
	{
		return NULL;
	}
	pSQUARE orig_node = (pSQUARE)(e);
	pSQUARE cloned_node = (pSQUARE)malloc(sizeof(SQUARE));
	if (cloned_node == NULL)
	{
		return NULL;
	}
	cloned_node->x_i = orig_node->x_i;
	cloned_node->x_f = orig_node->x_f;
	cloned_node->y_i = orig_node->y_i;
	cloned_node->y_f = orig_node->y_f;
	cloned_node->key = orig_node->key;
	return cloned_node;

}

//******************************************************************************
//* function name : DelteFunc
//* Description   : Deletes a node.
//* Parameters    : poninter to node.
//* Return value  : None.
//******************************************************************************
void DelteFunc(pNode e)
{
	if (e == NULL)
	{
		return;
	}
	pSQUARE my_node = (pSQUARE)(e);
	free(my_node);
	return;
}

//******************************************************************************
//* function name : PrintFunc
//* Description   : Prints a node.
//* Parameters    : poninter to node.
//* Return value  : None.
//******************************************************************************
void PrintFunc(pNode e)
{
	if (e == NULL)
	{
		return;
	}
	pSQUARE my_node = (pSQUARE)(e);
	printf("[%f %f] [%f %f]", my_node->x_i, my_node->x_f, my_node->y_i, my_node->y_f);
	return;
}



GetKeyFunction	pGetKeyFunc = getKeyFunc;
CloneFunction	pCloneFunc = cloneFunc;
PrintFunction	pPrintFunc = PrintFunc;
DelFunction		pDelteFunc= DelteFunc;

//******************************************************************************
//* function name : InitPartition
//* Description   : Initiates the partition,with the first square [0 1] [0 1].
//* Parameters    : None.
//* Return value  : None.
//******************************************************************************
void InitPartition()
{
	numOfNodes=1;
	TreeDestroy(my_partition);
	
	my_partition = TreeCreate(pGetKeyFunc, pCloneFunc, pPrintFunc, pDelteFunc, 4);
	pSQUARE new_node = (pSQUARE)malloc(sizeof(SQUARE));
	new_node->key = 1;
	new_node->x_i = 0;
	new_node->x_f = 1;
	new_node->y_i = 0;
	new_node->y_f = 1;
	
	TreeAddLeaf(my_partition, 1,new_node);
	free(new_node);
	return;
}

//******************************************************************************
//* function name : set_part_key
//* Description   : seraches for a given x,y cordinates,each suqare is its father,and returns the father key.
//* Parameters    : x-horizontal cordinate of new point,y-vertical cordinate of new point(new point that refines the cell).square,pointer to possible father
//* Return value  : None
//******************************************************************************

int set_part_key(double x, double y, pSQUARE square)
{
	int key;
	if (((x >= square->x_i) && (x < square->x_f) && (y >= square->y_i) && (y < square->y_f)) || ((x==1) &&(square->x_f==1) && (y >= square->y_i) && (y < square->y_f)) || ((y == 1) && (square->y_f == 1) && (x >= square->x_i) && (y < square->x_f)) || (((x == 1) && (square->x_f == 1) && (y == 1) && (square->y_f == 1))))
		
	{
		key = square->key;
		pSQUARE *children = (pSQUARE*)(TreeGetChildren(my_partition, square->key));
		for (int i = 0; i < 4; i++)
		{
			if (children[i] != NULL)
			{
				int tmp_key = set_part_key(x, y, children[i]);
				if (tmp_key != -1)
				{
					key = tmp_key;
				}
			}
		}
		
		for (int i = 0; i < 4; i++)
		{
			if (children[i] != NULL)
			{
				free(children[i]);
			}
			
		}
		free(children);
				
	}
	else key = -1;
	return key;
}

//******************************************************************************
//* function name : RefineCell
//* Description   : Refines the cell partition.
//* Parameters    : x-horizontal cordinate of new point,y-vertical cordinate of new point(new point that refines the cell).
//* Return value  : None
//******************************************************************************

void RefineCell(double x, double y)
{
	pSQUARE first_square = (pSQUARE)(TreeGetRoot(my_partition));
	int key = set_part_key(x, y, first_square);//find he key of the node who is the square father of x,y cordinates
	if (key == -1)
	{
		key = first_square->key;
	}
	pSQUARE father_square = TreeGetNode(my_partition, key);
	pSQUARE new_square = (pSQUARE)malloc(sizeof(SQUARE));
	if (new_square == NULL)
	{
		return;
	}
	new_square->key = numOfNodes + 1;
	numOfNodes++;
	double new_sqr_size = (father_square->x_f - father_square->x_i)/2;
	//now,setwhich quarter of father square is the new square
	if ((x < (father_square->x_i + new_sqr_size) )&& (y < (father_square->y_i + new_sqr_size)))
	{
		new_square->x_i = father_square->x_i;
		new_square->x_f = father_square->x_i +new_sqr_size;
		new_square->y_i = father_square->y_i;
		new_square->y_f = father_square->y_i + new_sqr_size;
	}
	else if ((x >= (father_square->x_i + new_sqr_size)) && (y < (father_square->y_i + new_sqr_size)))
	{
		new_square->x_i = father_square->x_i + new_sqr_size;
		new_square->x_f = father_square->x_f;
		new_square->y_i = father_square->y_i;
		new_square->y_f = father_square->y_i + new_sqr_size;
	}
	else if ((x < (father_square->x_i + new_sqr_size)) && (y >= (father_square->y_i + new_sqr_size)))
	{
		new_square->x_i = father_square->x_i;
		new_square->x_f = father_square->x_i + new_sqr_size;
		new_square->y_i = father_square->y_i + new_sqr_size;
		new_square->y_f = father_square->y_f ;
	}
	else
	{
		new_square->x_i = father_square->x_i + new_sqr_size;
		new_square->x_f = father_square->x_f ;
		new_square->y_i = father_square->y_i + new_sqr_size;
		new_square->y_f = father_square->y_f;
	
	}

	TreeAddLeaf(my_partition, key, new_square);
	free(father_square);
	free(new_square);
	free(first_square);
	return;

}

//******************************************************************************
//* function name : printRec
//* Description   : Prints square minus all its children,and then the same for each child,recursivly.
//* Parameters    : poninter to square.
//* Return value  : None
//******************************************************************************

void printRec(pSQUARE square)
{
	pSQUARE *children = (pSQUARE*)(TreeGetChildren(my_partition, square->key));
	PrintFunc(square);
	for (int i = 0; i < 4; i++)//printing the suqare minus its subsquares.
	{
		if (children[i] != NULL)
		{
			printf("\\");
			PrintFunc(children[i]);
		}
		if (i == 3)
		{
			printf("\n");
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		if (children[i] != NULL)
			printRec(children[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		if (children[i] != NULL)
		{
			free(children[i]);
		}

	}
	free(children);
	
}

//******************************************************************************
//* function name : PrintPartition
//* Description   : Prints the partition.
//* Parameters    : None.
//* Return value  : None
//******************************************************************************
void PrintPartition()
{
	if (my_partition == NULL)
	{
		InitPartition();
	}
	pSQUARE first_square = (pSQUARE)(TreeGetRoot(my_partition));
	printRec(first_square);
	free(first_square);
	return;
}

//******************************************************************************
//* function name : DeletePartition
//* Description   : Deletes the partition.
//* Parameters    : None.
//* Return value  : None
//******************************************************************************
void DeletePartition()
{
	numOfNodes = 1;
	TreeDestroy(my_partition);
}