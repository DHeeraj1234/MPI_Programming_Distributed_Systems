//Name:Dheeraj Reddy Maddi
//A number: A04170468
//Compiling: mpicc mpi3.c
//Execution: mpirun -np number a.out filename
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<stdbool.h>
#include<string.h>
int main(int argc, char *argv[]) 
{
  int rank, np, coord_time_sleep, reject_threshold, x, g;
  
  //MPI_Status status;

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;	
  int cnt=0;
  int str=0;
	//int nump=6;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);


  MPI_Status status;
  MPI_Request request;
  if(rank == 0)
  {
	//int nump=6;
	int p11=1;
	int p1;
	int q1;
	char * line1= NULL;
        char *a=NULL;
	char *b=NULL;
	int work[str];
	int available[str];
	


	fp = fopen(argv[1], "r"); //File name 'file.txt'
    if (fp == NULL){
	printf(";;");        
	exit(EXIT_FAILURE);
    }
    if((read = getline(&line, &len, fp)) != -1){
    	str=atoi(line);	
    }
	int max_need[str][np-1];
	int allocation[str][np-1];
	int need[str][np-1];
//	printf("%d\n",str);
    int array[str];
    int array1[str];		
    //  Reading every line from file and storing in array(arr) and array1		
    while ((read = getline(&line, &len, fp)) != -1) {
         
		
		line1=strtok(line," ");
		
			bool f=0;
			while (line1 != NULL) {
				if(f==0) 
				    a=line1;
				else
				    b=line1;	 
				line1 = strtok(NULL, " "); 
				f=1;
	    		} 

			p1=atoi(a);
			if(p11!=0){		
				q1=atoi(b);
			}
			array[p11-1]=p1;
			array1[p11-1]=q1;
			p11=p11+1;        
		cnt=cnt+1;
	//}  
}

	

	int i=0,j;
	int a1[str];
    	int a2[str];
	int a3[str];
	//Initially sending the total number of requests to other processes
	for(i=1;i<np;++i){
		MPI_Send(&str,1, MPI_INT, i, 100, MPI_COMM_WORLD);	
	}

	for(i=0;i<str;++i){
		work[i]=array1[i];
		available[i]=array1[i];	
	}
			
	for(i=1;i<np;++i){
		//Receving max_need and allocation from each process parallely
		MPI_Recv(&a1,str,MPI_INT,MPI_ANY_SOURCE,100,MPI_COMM_WORLD,&status);
		int l1=status.MPI_SOURCE;
		MPI_Recv(&a2,str,MPI_INT,l1,100,MPI_COMM_WORLD,&status);
		MPI_Recv(&a3,str,MPI_INT,l1,100,MPI_COMM_WORLD,&status);	
		
		
		printf("\n");
		for(j=0;j<str;++j){
			max_need[j][l1-1]=a2[j];
			allocation[j][l1-1]=a3[j];		
		}
	
	}
	int allocsum[10000];
	int sum=0;
	for(i=0;i<str;++i){
		sum=0;
		for(j=0;j<np-1;++j){
			sum=sum+allocation[i][j];		
		}	
		allocsum[i]=sum;
	}

	

	for(i=0;i<np-1;++i){
		available[i]=array1[i]-allocsum[i];
		
	}

	for(i=0;i<np-1;++i){
		work[i]=array1[i]-allocsum[i];
			
	}
	printf("The coordinator is checking if the state is safe\n");
	printf("Max_Need Matrix is:\n");
	for(i=0;i<str;++i){
		for(j=0;j<np-1;++j){
			printf("%d ",max_need[i][j]);	
		}
		printf("\n");
	}
	printf("\n");
		//printf("%d  . \n",str);
	printf("Allocation Matrix is:\n");	
	for(i=0;i<str;++i){
		for(j=0;j<np-1;++j){
			printf("%d ",allocation[i][j]);	
		}
		printf("\n");
	}	
	
	for(i=0;i<str;++i){
		for(j=0;j<np-1;++j){
				
			need[i][j]=max_need[i][j]-allocation[i][j];		
		}
		printf("\n");
	}
	printf("Need Matrix is:\n");
	for(i=0;i<str;++i){
		for(j=0;j<np-1;++j){
			printf("%d ",need[i][j]);	
			//need[i][j]=max_need[i][j]-allocation[i][j];		
		}
		printf("\n");
	}
	printf("Available vector is:\n");
	for(i=0;i<str;++i){
					
			printf("%d \n",available[i]);	
					
		}

	printf("Work vector is:\n");
	for(i=0;i<str;++i){
						
			printf("%d \n",work[i]);	
					
		}


//----------------------------------------------------------------
//Bankers Algorithm Calculation



		
	int k,c;	

		
		
	int finished[np-1];
	int p[np-1];
	for(i=0;i<np-1;++i){
		finished[i]=0;
		p[i]=0;	
	}

	int count=0;
	
	bool f1=0;
	for(i=0;i<np-1;++i){
		bool f=0;
		//printf("p:%d\n",p[i]);
		//printf("..");	
		//printf("p:%d %d ",p[i],i);
		if(p[i]==0){
		//printf("n:%d %d %d\n",need[0][i],need[1][i],need[2][i]);
		//printf("w:%d %d %d\n",work[0],work[1],work[2]);
		for(j=0;j<str;++j){
			
			if(need[j][i]>work[j]){  //j,i
				f=1;				
				break;			
			}
			
}	
		
		
		if(f==0){
			f1=1;
			for(k=0;k<str;++k){
					
				work[k]=work[k]+allocation[k][i]; //k,i	
						
			}
			p[i]=1;
			finished[count++]=i;	
			//printf("%d  %d\n",finished[count-1],i);			
		}
		 c=0;
		
		for(k=0;k<np-1;++k){
			//printf("%d..",p[k]);			
			if(p[k]!=1){
				c=c+1;
				
			}		
		}
		
		//printf("\n");
		
}
	if(i==np-2&&f1==0){
		printf("There is no safe sequence! Aborting\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);	
	}
	if(i==np-2&&c!=0){
			//printf(".......");
			i=-1;	
			f1=0;	
		}
	
}
	printf("Safe Sequence: ");
	for(i=0;i<np-1;++i){
		printf("%d ",finished[i]+1);

	}		
	printf("\n");
	

//-----------------------------------------------------------------


	
	printf("The current state is safe\n");
	for(i=0;i<np-1;++i){
		printf("The coordinator is allocating the resources "); //to process %d\n",finished[i]+1);
		for(j=0;j<str;++j){
			printf("%d ",need[j][finished[i]]);		
		}
		printf("to process %d\n",finished[i]+1);
	
		MPI_Send(&a1,str, MPI_INT, finished[i]+1, 100, MPI_COMM_WORLD);	
		MPI_Recv(&a1,str,MPI_INT,finished[i]+1,100,MPI_COMM_WORLD,&status);	

		
		for(j=0;j<str;++j){
		
		available[j]=available[j]+allocation[j][finished[i]];	
	}

		
	//for(i=0;i<str;++i){
		for(j=0;j<str;++j){
			max_need[j][finished[i]]=0;			
			//printf("%d ",max_need[i][j]);	
					
		}
	
	//printf("\n");	
	
	for(j=0;j<str;++j){
			allocation[j][finished[i]]=0;			
			
					
		}
	
	for(j=0;j<str;++j){
			need[j][finished[i]]=0;			
			
					
		}
	
	
	printf("Available vector is:\n");
	for(j=0;j<str;++j){
			//need[finished[i]+1][j]=0;			
			printf("%d \n",available[j]);	
					
		}
	k=0;
	printf("Max_Need Matrix is:\n");
	for(k=0;k<str;++k){
		for(j=0;j<np-1;++j){
			printf("%d ",max_need[k][j]);	
		}
		printf("\n");
	}
	printf("\n");
		//printf("%d  . \n",str);
	printf("Allocation Matrix is:\n");	
	for(k=0;k<str;++k){
		for(j=0;j<np-1;++j){
			printf("%d ",allocation[k][j]);	
		}
		printf("\n");
	}
	printf("Need Matrix is:\n");	
	for(k=0;k<str;++k){
		for(j=0;j<np-1;++j){
			printf("%d ",need[k][j]);	
		}
		printf("\n");
	}

		
	}

		
	fclose(fp);
}

		else{

		int str;
		MPI_Recv(&str,1,MPI_INT,0,100,MPI_COMM_WORLD,&status);
		srand(time(NULL) + rank);
		int r=	(rand()%6)+5;
		int p=1;
	int p1;
	int q1;
	int r1;
	char * line1= NULL;
        char *a=NULL;
	char *b=NULL;
	char *c=NULL;
	char name[20]="P_";
	char  k1[10];
	sprintf(k1, "%d", rank);
	strcat(name,k1);
	strcat(name,".txt");
	//printf("%s",name);	
	fp = fopen(name, "r"); //File name 'file.txt'
    if (fp == NULL){
	//printf(";;");        
	exit(EXIT_FAILURE);
    }
	int a1[100000];
    int a2[100000];
	int a3[100000];		
 	
   	
	printf("Process with rank %d reading the resource allocation\n",rank);

	while ((read = getline(&line, &len, fp)) != -1) {
          	
		
		line1=strtok(line," ");

		
			bool f=0;
			char* ara[3]={NULL,NULL,NULL};
			int ctn=0;
			while (line1 != NULL) {
				ara[ctn++]=line1;				 
				line1 = strtok(NULL, " "); 
				
	    		} 
			
			p1=atoi(ara[0]);
			if(p!=0){		
				q1=atoi(ara[1]);
				r1=atoi(ara[2]);
			}

			
			a1[p-1]=p1;
			a2[p-1]=q1;
			a3[p-1]=r1;
			
			p=p+1;        
		cnt=cnt+1;
	//}  
}
	int i=0;
		
	printf("Process with rank %d sending the resource allocation information to the coordinator\n",rank);
	MPI_Send(&a1,str, MPI_INT, 0, 100, MPI_COMM_WORLD);
	MPI_Send(&a2,str, MPI_INT, 0, 100, MPI_COMM_WORLD);
	MPI_Send(&a3,str, MPI_INT, 0, 100, MPI_COMM_WORLD);
	
	//Receiving the requests in the safe order
	MPI_Recv(&a1,str,MPI_INT,i,100,MPI_COMM_WORLD,&status);
	printf("Process %d has received the resource\n",rank);
	printf("Process %d is using the resource\n",rank);
	sleep(r);
	printf("Process %d is releasing the resource\n",rank);
	MPI_Send(&a1,str, MPI_INT, 0, 100, MPI_COMM_WORLD);
}
	MPI_Finalize();	



	return 0;
}
