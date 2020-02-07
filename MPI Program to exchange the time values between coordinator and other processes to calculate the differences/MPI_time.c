//Name: Dheeraj Reddy Maddi
//A#: A04170468
//Compilation: mpicc mpitime.c
//Execution: mpirun -np 4 a.out text.txt
//Note: Taking inputs in micro seconds(both in 500000) from the input file.
//Displaying the final output in milliseconds

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <time.h> 
#define _GNU_SOURCE

int main(int argc, char *argv[]) 
{
    	srand(time(0)); 

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int arr[100000];
    int cnt=0;
    int str=0;
    char s[100];	
    //int i=0;
    int sum=0;
    //Structure to collect the data 
    struct {	
		//int x;
		int y;
		char z[100];		
    }a1;

  int rank,i,j,comm_sz;
  //MPI_Status status;
  int a;
  
  
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  int blocklengths[2]={1,100};
  MPI_Datatype types[2]={MPI_INT,MPI_CHAR};
  MPI_Aint displacements[2];
  MPI_Datatype restype;
  MPI_Aint intex;

  MPI_Type_extent(MPI_INT, &intex);
  //MPI_Type_extent(MPI_INT, &intex);
  MPI_Type_extent(MPI_CHAR, &intex);

  displacements[0] = (MPI_Aint) 0; 
  displacements[1] = intex;
  //displacements[2] = intex+intex;
  MPI_Type_struct(2, blocklengths, displacements, types, &restype);
  MPI_Type_commit(&restype);
	
  MPI_Status status;
  MPI_Request request;
  if(rank == 0)
  {
     //printf("..............");	
     fp = fopen(argv[1], "r"); //File name 'file.txt'
    if (fp == NULL)
        exit(EXIT_FAILURE);

    //  Reading every line from file and storing in array(arr)		
    while ((read = getline(&line, &len, fp)) != -1) {
                
	//printf("%s", line);
	if(cnt==0){
		//s=line;
		strcpy(s,line);
		cnt=cnt+1;
		
	}
	else{
		str=atoi(line);
		arr[cnt-1]=str;
		cnt=cnt+1;
		
	}    
}
       //printf("%s %d\n",s,arr[1]); 

       strcpy(a1.z,s);
       
    //Sending coordinator time and threshold to each process  
	int k=0;
       for(i=1;i<comm_sz;++i){
       		for(j=0;j<30;++j){
			
			
			MPI_Recv(&a1,1,restype,MPI_ANY_SOURCE,100,MPI_COMM_WORLD,&status);
			//sleep in microseconds
			usleep(arr[0]);
			strcpy(a1.z,s);
 
       			a1.y=arr[1];
			MPI_Send(&a1,1, restype, status.MPI_SOURCE, 100, MPI_COMM_WORLD);
					
		}
       }			
  } 
  else{
		
		
		//srand(time(0)); 
		srand(time(NULL) + rank);		
		int i=0,j=0;
		double ar[30];
		double sum=0;
		double avg=0;
		double sum1=0;
		int fileth;
		char s[100];
		printf("Process with rank %d started sending time requests\n",rank);	
		int r=	(rand()%6)+3;
		//printf("%d...............\n",r);
		sleep(r);
		
		for(i=0;i<30;++i){
			double t0 = MPI_Wtime();
			
			MPI_Send(&a1,1, restype, 0, 100, MPI_COMM_WORLD);	
				
			MPI_Recv(&a1,1,restype,0,100,MPI_COMM_WORLD,&status);
			double t1=	 MPI_Wtime();	
					
			strcpy(s,a1.z);	
			
			fileth=a1.y;	
				
					
			
			ar[i]=t1-t0;
				
		}

		//printf("S::%d \n",fileth);	
		printf("Process with rank %d done sending time requests\n",rank);	
		for(i=0;i<30;++i){
			sum=sum+ar[i];		
		}
		//Average Calculation
		avg=sum/30;
		//Dividing the threshold by micro seconds
		double th=(double)fileth/1000000;
		
		//Rejecting few of the times which are above threshold
		for(i=0;i<30;++i){
			//printf("%lf %lf %lf.\n",ar[i] ,avg,fabs(ar[i]-avg));
			//printf("%lf.\n",ar[i]);
			if(fabs(ar[i]-avg)>th){
				ar[i]=-100;
						
			}		
		}
		int c=0;
		double ns=0;
		for(i=0;i<30;++i){
			if(ar[i]!=-100){
				ns=ns+ar[i];
				c=c+1;			
			}		
		}
		double navg=(double)ns/c;
		
		a1.z[strlen(a1.z)-1]='\0';
		printf("I am process with rank %d, My time is %s +-  %f ms\n",rank,a1.z,0.5*navg*1000);
}
	
	 
	 MPI_Finalize();
}
