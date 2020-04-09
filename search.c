// author  : Ahmed Mohamed Hanafy - ID : 20170357 - 09/04/2020

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[]){
	int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/
	int source;		/* rank of sender	*/
	int dest;		/* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	MPI_Status status;	/* return status for 	*/

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	printf("%d\n",my_rank);

	if( my_rank == 0){ /// master thread
		int l = 1 , u = 50 ;
		MPI_Send(&l, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
		MPI_Send(&u, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
	}
	else{ /// slave
	//			MPI_Send( mat2[j], x, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD); ///sending matrix 2
	//		MPI_Recv(row2, x, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD, &status);			
		/// each slave recieve a lower and upper bound for the files numbers
		/// and will iterate over them and find the matches
		int lower_bound = 0 , upper_bound = -1;
		MPI_Recv(&lower_bound, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&upper_bound, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		for(int i = lower_bound ; i <= upper_bound ; ++i){
			char filename [80]; 
			sprintf(filename,"Aristo-Mini-Corpus P-%d",i);
			printf("%s\n",filename);

		}
		
	}

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}