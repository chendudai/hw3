#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"

pTree my_partition=NULL;


typedef struct _SQUARE {
	int x_i,x_f,y_i,y_f;//square vertexes
	int key;//the key of each node will be the x cord of the square center
} SQUARE, *pSQUARE;

GetKeyFunction getKeyFunc(pNode e)
{
	pSQUARE my_node = (pSQUARE)(e);
	return my_node->key;
}


CloneFunction cloneFunc(pNode e)
{
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

DelFunction DelteFunc(pNode e)
{
	pSQUARE my_node = (pSQUARE)(e);
	free(my_node);
	return;
}


PrintFunction PrintFunc(pNode e)
{
	//@@@@@
	return;
}



GetKeyFunction	(*pGetKeyFunc)(pNode e);
CloneFunction	(*pCloneFunc)(pNode e);
PrintFunction	(*pPrintFunc)(pNode e);
DelFunction		(*pDelteFunc)(pNode e);

void InitPartition()
{
	my_partition = TreeCreate(pGetKeyFunc, pCloneFunc, pPrintFunc, pDelteFunc, 4);
	pSQUARE new_node;
	new_node->x_i = 0;
	new_node->x_f = 1;
	new_node->y_i = 0;
	new_node->y_f = 1;

	TreeAddLeaf(my_partition, 1,new_node);
}

int set_part_key(int x, int y, pTree partition,)
{
	
}