#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
void bubbleSort(int arr[],int n){
int i,j,temp;
	for(i=0;i<n-1;i++){
		for(j=0;j<n-i-1;j++){
			if(arr[j]>arr[j+1]){
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
}
int  main(){
	int i;
	int j;
	int ch,n;
	printf("\n ENTER THE NUMBER TO SORT :- ");
	scanf("%d",&n);
	int arr[n];
	for(i=0;i<n;i++){
		scanf("%d",&arr[i]);
	}
	printf("\n ENTER 1 FOR FORK,WAIT AND BUBBLE SORT");
	printf("\n ENTER 2 FOR ORPHAN PROCESS");
	printf("\n ENTER 3 FOR ZOMBIE PROCESS");
	printf("\nENTER YOUR CHOICE HERE...");
	scanf("%d",&ch);
		switch(ch){
			case 1:
		
			pid_t pid = fork();
			if(pid<0)
				printf("\n FORK FAILED");
				
			else if(pid==0){
				printf("\nCHILD PROCESS EXECUTNG");
				bubbleSort(arr,n);
				for(i=0;i<n;i++){
					printf("%d",arr[i]);
				}
				printf("\n");
				
			}
			else{
			printf("\nPARENT PROCESS EXECUTNG");
				bubbleSort(arr,n);
				for(i=0;i<n;i++){
					printf("%d",arr[i]);
				}
				printf("\n");
				wait(NULL);
				printf("\nCHILD PEOCESS COMPLETED....");
			}
			break;
			
		}
	
}
