#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char*argv[])
{
	unsigned int n = 1000000;

	size_t bytes = n*sizeof(double);

	double* a = (double*)malloc(bytes);
	double* b = (double*)malloc(bytes);
	double* c = (double*)malloc(bytes);

	int i;
	for(i = 0; i < n; i++)
	{
		a[i] = sinf(i)*sinf(i);
		b[i] = cosf(i)*cosf(i);
	}

	// Kernel analogue.
	for(i = 0; i < n; i++)
	{
		c[i] = a[i]+b[i];
	}


	double sum = 0;
	for(i=0; i<n; i++)
	{
		sum += c[i];
	}
	printf("final result: %f\n", sum/n);

	free(a);
	free(b);
	free(c);

	return 0;
}
