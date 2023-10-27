#include <omp.h> 
#include <stdio.h>

int main()
{
#pragma omp target
	puts("Hi Folks!");

	return 0;
}
