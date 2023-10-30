#include <omp.h> 
#include <stdio.h>

int main()
{
#pragma omp target
#pragma omp parallel
	puts("Hey! OpenMP even work in DPU...");

	return 0;
}
