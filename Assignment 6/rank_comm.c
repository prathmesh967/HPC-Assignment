#include<mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
   // initializing mpi environment
   MPI_Init(&argc, &argv);  

   // total number of processes
   int world_size;
   MPI_Comm_size(MPI_COMM_WORLD, &world_size);  

   int world_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 

   printf("Hello world from processor rank %d out of %d processors\n", world_rank, world_size);

   MPI_Finalize();

   return 0;
}