#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "partition.h"
#include "gentree.h"

pTree my_partion;


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

void InitPartition()
{

}