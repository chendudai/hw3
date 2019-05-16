#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"


/* definition of the element in the tree */
typedef struct _ELEMENT* PELEMENT;
typedef struct _ELEMENT
{
  pNode obj;
  PELEMENT* children;
  PELEMENT parent;
  int childrenCount;
} ELEMENT;

/* definition of the tree structure */    
typedef struct _tree{
  PELEMENT head;

  /* *** complete the definition of the tree structure *** */

} Tree;

/* *** complete the interface functions implementation *** */

