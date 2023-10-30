#include <omp.h> 
#include "log.h"
#include <stdio.h>

int main()
{
#pragma omp target
	printify();

	return 0;
}
