#define _GNU_SOURCE
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <mpi.h>

//Name: Dheeraj Reddy Maddi
//A Number: A04170468
//Approach: Sorting the input file based on the disks, distributing it using a structure and calculating the SSTF and sending it to process 0 to display.
//Execution: mpicc MPI.c -o mpicode
//Execution: mpirun -np number ./mpicode TextFile

//#define _GNU_SOURCE
//#include <stdio.h>
//#include <stdlib.h>

//----------------------------------------------------------
// Merge Sort Implementation for SSTF from line 20 to 88 which has complexity of n*logn
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 
//---------------------------------------------------------
//Shortest Seek Time First from lines 91 to 168
int sstf(int arr[],int cnt){
	int i=0;
	int j=i-1;

	int k=i+1;
	int sum=arr[0];
	int p=0;

	int q=0;
	int y=0;
	// Iterating the array from head by finding the closest integer.
	// If the element of the array is visited, then it is replaced with '-1' value as an indication. 

	while(i>=0 && i<cnt){
		// Handling edge cases from line 142-174
		// If we reach any of the edge, it means that there is no other element after that, so we traverse to previous value until there is no '-1'. From this point we just 			   calculate the difference between numbers until we reach 			   the other edge, which is implemented from 136-161
		
		if(i==0){
			
			p=i+1;
			while(arr[p]==-1){
				++p;
			}
			sum=sum+abs(arr[p]-arr[i]);
			arr[i]=-1;
			while(p<cnt-1){
				
				sum=sum+abs(arr[p]-arr[p+1]);
				++p;
			}
			break;
		}

		if(i==cnt-1){
			q=i-1;
			while(arr[q]==-1){
				--q;
			}
			//printf("%d \n",q);
			sum=sum+abs(arr[q]-arr[i]);
			arr[i]=-1;
			while(q>0){
				//printf(":");
				sum=sum+abs(arr[q]-arr[q-1]);
				//printf("%d\n",sum);
				--q;
			}
		
			break;
		}

		// If the current value is not the edge value, then we check for adjacent elements which are closest to current until we dont get '-1' which means already visited element. 			   implemented from 180-195 lines.

		j=i-1;
		k=i+1;
		while(arr[j]==-1&&j>0){
			j=j-1;
		}
		while(arr[k]==-1&&k<cnt-1){
			k=k+1;
		}
		if(abs(arr[i]-arr[j])<abs(arr[i]-arr[k])){
			sum=sum+abs(arr[i]-arr[j]);
			arr[i]=-1;
			i=j;
		}
		else{
			sum=sum+abs(arr[i]-arr[k]);
			arr[i]=-1;
			i=k;
		}
	}
	
	//Printing final sum value
	//printf("Total Sum: %d ",sum);
	return sum;
}

// -----------------------MAIN FUNCTION---------------------------
int main(int argc, char *argv[]) 
{
	int rank,i,comm_sz;
	int array[10000];
	int array1[10000];
	int stor[10000];
	int count;
	int cnt=0;
	int sndans;
	struct {	
		int x;
		int y;		
		int ar[10000];
	}a1;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	int blocklengths[3]={1,1,10000};
	MPI_Datatype types[3]={MPI_INT, MPI_INT,MPI_INT};
	MPI_Aint displacements[3];
	MPI_Datatype restype;
	MPI_Aint intex;
	
	
	MPI_Type_extent(MPI_INT, &intex);
	MPI_Type_extent(MPI_INT, &intex);
	
	displacements[0] = (MPI_Aint) 0; 
	displacements[1] = intex;
	displacements[2]= intex+intex;
	MPI_Type_struct(3, blocklengths, displacements, types, &restype);
	MPI_Type_commit(&restype);




	MPI_Request request;
	MPI_Status status;
	
	//Coordinator from lines 212 to 308
	if(rank==0){

		FILE * fp;
		char * line = NULL;
		char * line1= NULL;
		size_t len = 0;
		ssize_t read;
		
		int p=0;
		char *a=NULL;
		char *b=NULL;
		int p1;
		int q1;

		fp = fopen(argv[1], "r");	
		if (fp == NULL)
		exit(EXIT_FAILURE);
		
		
		while ((read = getline(&line, &len, fp)) != -1) {
			
			line1=strtok(line,",");
			bool f=0;
			while (line1 != NULL) {
				if(f==0) 
				    a=line1;
				else
				    b=line1;	 
				line1 = strtok(NULL, ","); 
				f=1;
	    		} 

			p1=atoi(a);
			if(p!=0){		
				q1=atoi(b);
			}
			array[p]=p1;
			array1[p]=q1;
			p=p+1;        

	    	}
		int i=0,j=0;
		//Sorting based on disks
		for(i=1;i<p;++i){
			for(j=i+1;j<p;++j){
				if(array[j]<array[i]){
					int temp=array[j];
					array[j]=array[i];
					array[i]=temp;
					int temp1=array1[j];
					array1[j]=array1[i];
					array1[i]=temp1;
				}
			}
		}

		//for(i=1;i<p;++i)
		//	printf("%d ",array[i]);
		//printf("\n");	
		//for(i=1;i<p;++i)
		//	printf("%d ",array1[i]);	
		
		
		//After sorting, storing the range(size) of the each disk to send it to the receiver to retrieve that particular information from lines 277 to 304
		stor[cnt++]=array[1];
		stor[cnt++]=1;	
		int c=1;
		for(i=1;i<p-1;++i){
			c=c+1;
			if(array[i]!=array[i+1]){
								
				stor[cnt++]=c;
				stor[cnt++]=array[i+1];	
				stor[cnt++]=i+1;		
				//c=0;			
			}		
		}
		stor[cnt++]=c+1;
		
		int k=0;
		for( k=0;k<cnt-2;k=k+3){
			a1.x=stor[k+1];			
			a1.y=stor[k+2];
			int l1=0;
			for(i=a1.x;i<a1.y;++i)
				a1.ar[l1++]=array1[i];
			a1.y=l1;
			
			MPI_Send(&a1,1, restype, stor[k], 100, MPI_COMM_WORLD);
			
		
		}

		 fclose(fp);
}
	else{
		int i=0;
			//Receiving ans sending back to coordinator
			MPI_Recv(&a1,1,restype,0,100,MPI_COMM_WORLD,&status);
			
			//MergeSort for SSTF
			mergeSort(a1.ar, 0, a1.y-1);  
		
			int ans=sstf(a1.ar,a1.y);
			
			sndans=ans;
			MPI_Send(&sndans,1, MPI_INT, 0, 100, MPI_COMM_WORLD);
			
		}	
		
	
	//Coordinator
	if(rank==0){
		int p=0;
		for(p=1;p<comm_sz;++p){
			MPI_Recv(&sndans,1,MPI_INT,p,100,MPI_COMM_WORLD,&status);
			printf("Disk %d: travelled distance is %d\n",p,sndans);		
		}	
	}
	
	  
	MPI_Finalize();	


	return 0;
}
