#include<stdio.h>
#include<mpi.h>

void barrier(int rank,int size)
{
	MPI_Status status;
	int signal=-1;
	if((rank+1)*2-1>size)
	{
		MPI_Send(&signal,1,MPI_INT,(rank+1)/2-1,0,MPI_COMM_WORLD);
		MPI_Recv(&signal,1,MPI_INT,(rank+1)/2-1,0,MPI_COMM_WORLD,&status);
	}
	else if(rank!=0 && (rank+1)*2>=size)
	{
		MPI_Recv(&signal,1,MPI_INT,(rank+1)*2-1,0,MPI_COMM_WORLD,&status);
		MPI_Send(&signal,1,MPI_INT,(rank+1)/2-1,0,MPI_COMM_WORLD);
		MPI_Recv(&signal,1,MPI_INT,(rank+1)/2-1,0,MPI_COMM_WORLD,&status);
		MPI_Send(&signal,1,MPI_INT,(rank+1)*2-1,0,MPI_COMM_WORLD);
	}
	else if(rank!=0 && (rank+1)*2<size)
	{
		MPI_Recv(&signal,1,MPI_INT,(rank+1)*2-1,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&signal,1,MPI_INT,(rank+1)*2,0,MPI_COMM_WORLD,&status);
		MPI_Send(&signal,1,MPI_INT,(rank+1)/2-1,0,MPI_COMM_WORLD);
		MPI_Recv(&signal,1,MPI_INT,(rank+1)/2-1,0,MPI_COMM_WORLD,&status);
		MPI_Send(&signal,1,MPI_INT,(rank+1)*2-1,0,MPI_COMM_WORLD);
		MPI_Send(&signal,1,MPI_INT,(rank+1)*2,0,MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&signal,1,MPI_INT,(rank+1)*2-1,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&signal,1,MPI_INT,(rank+1)*2,0,MPI_COMM_WORLD,&status);
		MPI_Send(&signal,1,MPI_INT,(rank+1)*2-1,0,MPI_COMM_WORLD);
		MPI_Send(&signal,1,MPI_INT,(rank+1)*2,0,MPI_COMM_WORLD);
	}
}

int main(int argc, char** argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank==0)
    {
    	barrier(rank,size);
    	printf("rank - %d\n",rank);
    }

    printf("rank - %d\n",rank);
    MPI_Finalize();

}
