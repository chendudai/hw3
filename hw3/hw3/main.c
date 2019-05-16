#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "partition.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LINE_SIZE 255

int main()
{
  char szLine[MAX_LINE_SIZE];
  char* delimiters = " \t\n";
  char* command;
  char* x_str, *y_str;
  double x, y;
  InitPartition();
  fgets(szLine,MAX_LINE_SIZE,stdin);
  while (!feof(stdin)) {
	command = strtok(szLine, delimiters);
	if (!strncmp(command, "ADD", 3)) {
		x_str = strtok(NULL, delimiters);
		y_str = strtok(NULL, delimiters);
		x = atof(x_str);
		y = atof(y_str);
		RefineCell(x, y);
	}
	else if (!strncmp(command, "PRINT_PARTITION", 15)) {
		printf("Current partition:\n");
		PrintPartition();
	}
	else if (!strncmp(command, "INIT_PARTITION", 14)) {
		InitPartition();
	}
	fgets(szLine,MAX_LINE_SIZE,stdin);
  }
  
  DeletePartition();
  return 0;
}
