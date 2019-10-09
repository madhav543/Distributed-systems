#include<stdio.h>
#include<stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
	if(argc!=4) printf("%s","invalid arguments");
	else
	{
		int m,n,o;
		m = atoi(argv[1]);
		n = atoi(argv[2]);
		o = atoi(argv[3]);

		int **a = (int **)malloc(m * sizeof(int *)); 
    	for (int i=0; i<m; i++)
    	{
    		a[i] = (int *)malloc(n * sizeof(int));
    	}
 		int **b = (int **)malloc(n * sizeof(int *)); 
    	for (int i=0; i<n; i++)
    	{
    		b[i] = (int *)malloc(o * sizeof(int));
    	}
		int **c = (int **)malloc(m * sizeof(int *)); 
    	for (int i=0; i<m; i++)
    	{
    		c[i] = (int *)malloc(o * sizeof(int));
    	} 
		//int a[m][n],b[n][o],c[m][o];
		for(int i=0;i<m;i++)
		{
			for(int j=0;j<n;j++)
			{
				a[i][j] = rand()%100;
			}
		}
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<o;j++)
			{
				b[i][j] = rand()%100;
			}
		}
		time_t start1, end1;
		time(&start1); 
		for(int i=0;i<m;i++)
		{
			for(int j=0;j<o;j++)
			{
				c[i][j] = 0;
				for(int k=0;k<n;k++)
				{
					c[i][j] += a[i][k]*b[k][j];
				}
			}
		}
		time(&end1);
		double time_taken1 = (double)(end1 - start1);
		printf("%f without parallel\n",time_taken1);
		/*for(int i=0;i<m;i++)
		{
			for(int j=0;j<o;j++)
			{
				printf("%d\t",c[i][j]);
			}
			printf("\n");
		}*/
		time_t start2, end2;
		time(&start2); 
		#pragma omp parallel for
		for(int i=0;i<m;i++)
		{
			for(int j=0;j<o;j++)
			{
				c[i][j] = 0;
				for(int k=0;k<n;k++)
				{
					c[i][j] += a[i][k]*b[k][j];
				}
			}
		}

		time(&end2);
		double time_taken2 = (double)(end2 - start2);
		printf("%f with parallel\n",time_taken2);
		/*for(int i=0;i<m;i++)
		{
			for(int j=0;j<o;j++)
			{
				printf("%d\t",c[i][j]);
			}
			printf("\n");
		}*/

    	for (int i=0; i<m; i++)
    	{
    		free(a[i]);
    	}
    	free(a); 
    	for (int i=0; i<n; i++)
    	{
    		free(b[i]);
    	}
    	free(b); 
    	for (int i=0; i<m; i++)
    	{
    		free(c[i]);
    	}
    	free(c); 

	}
	return 0;
}
