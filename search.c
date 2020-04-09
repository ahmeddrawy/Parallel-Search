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
		int cnt = 0 ;
		for(int i = 1 ; i < p ; ++i){
			int tmpcnt = 0 ;
			MPI_Recv(&tmpcnt, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
			cnt+=tmpcnt;
		}
		printf("%d\n", cnt);
	}
	else{ /// slave
	//			MPI_Send( mat2[j], x, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD); ///sending matrix 2
	//		MPI_Recv(row2, x, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD, &status);			
		/// each slave recieve a lower and upper bound for the files numbers
		/// and will iterate over them and find the matches
		int lower_bound = 0 , upper_bound = -1;
		MPI_Recv(&lower_bound, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&upper_bound, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		char query[1024] =  "Energy";
		char delim[] = " ";
		int cnt =  0 ;
		for(int i = lower_bound ; i <= upper_bound ; ++i){
			char filename [1024]; 
			sprintf(filename,"Aristo-Mini-Corpus/Aristo-Mini-Corpus P-%d.txt",i);
			// printf("%s\n",filename);
			FILE * fp;
		    char * line = NULL;
		    size_t len = 0;
		    ssize_t read;
		    // strcat()
		    fp = fopen(filename, "r");
		    if (fp == NULL){
		    	printf("failed");
		        exit(EXIT_FAILURE);
		    }

		    while ((read = getline(&line, &len, fp)) != -1) {
		    	/// now we have each line in the file we want to count on it 
		    	char tmpquery[1024] ;
		    	
		    	strcpy(tmpquery , query);
		    	char *ptr = strtok(tmpquery, delim); /// we divide the query to words 
		    	int found = 1 ; 
		    	while(ptr != NULL ){
		    		char tmpline[2048];
		    		//printf("%s\n" , ptr);
		    		strcpy(tmpline , line);
		    		printf("%s\n",tmpline);
		    		if(strstr(tmpline, ptr) == NULL){
		    			found = -1;
		    		}
		    		/// now we have a word from query we need to search all words from line to check if exist
		    		ptr = strtok(NULL,delim);
		    	}
		    	if(found == 1){ /// we found all query words in this line
		    		cnt++;
		    		printf("%s\n",line);
		    	}
		    	
		    }

		    fclose(fp);
		    if (line)
		        free(line);
		}
		/// todo send cnt to root
		MPI_Send( &cnt, 1, MPI_INT, 0, tag, MPI_COMM_WORLD); ///sending row of one
		//printf("%d",cnt);	
		
	}

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}