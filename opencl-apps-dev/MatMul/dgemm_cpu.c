#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char*argv[])
{
	unsigned int m = 1024;
	unsigned int n = 1024;

	size_t bytes = m*n*sizeof(double);

	double* a = (double*)malloc(bytes);
	double* b = (double*)malloc(bytes);
	double* c = (double*)malloc(bytes);

	int i, j, k;
	int base;
	#pragma omp parallel for private(i,j)
	for(i = 0; i < m; i++)
	{
	    base = n*i;
	    for(j = 0; j < n; j++)
	    {
		a[base+j] = cosf(j)*sinf(i);
		b[base+j] = cosf(i)*sinf(j);
	    }
	}

	// Kernel analogue.
	#pragma omp parallel for private(i,j)
	for(i = 0; i < m; i++)
	{
	    base = n*i;
	    for(j = 0; j < n; j++)
	    {
		for(k = 0; k < m; k++)
		{
		    c[base+j] += a[base+k]*b[k*n+j];
	    	}
	    }
	}

	// Output
	FILE* outfile = fopen("out.cpu.txt", "w");

	for(i = 0; i < m; i++)
	{
	    base = n*i;
	    for(j = 0; j < n; j++)
	    {
		fprintf(outfile, "%f ", c[base+j]);
	    }
	    fprintf(outfile, "\n");
	}

	fclose(outfile);
	printf("Done.\n");

	free(a);
	free(b);
	free(c);

	return 0;
}
