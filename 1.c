#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#define a 16
#define SIZE 100 
#define TRUE 1
#define FALSE 0
// A linked list where each node represents a point r[]
struct node {
	float r[3];
	struct node* next;
};
void push(struct node** head, float r[3]) {
	struct node* newNode = (struct node*)(malloc(sizeof(struct node)));
	newNode->next = NULL;
	int i;
	// copy r[i] into newNode->r[i];
	for (i=0; i<3; i++) newNode->r[i] = r[i];
	if ( *head == NULL) {
		*head = newNode;
		return;
	}
	newNode->next = *head;
	*head = newNode;
}
bool partition(float r[3],int L, int lMatrix[L][L][L], struct node* pointList[L][L][L]) {
	//validity check for r[3]
	fflush(stdout);
	printf("Entering partition\n");
	int i;
	for(i=0;i<3;i++) { 
		if(!(r[i]>=0 && r[i] <= a)) {  
			printf(" Input Invalid");
					return FALSE;
		}
		printf("%f ", r[i]);
	}
	printf("\n");
	//validity check for L
	if(!(L>0))
		printf(" Input Invalid");

	float x[3];
	int l[3],binary[3], bString=0, temp;
	for (i=0; i<3; i++)	{
	printf("generating binary string: ");
	 x[i]=(r[i]*L)/a;
	 l[i] = (int)x[i];
	 // for x1, temp=1. binary = 0 | 1 = 1
	 // for x2, temp=0, binary =  1<<1 = 10 | 0 = 10
	 // for x3, temp=1, binary  = 10<<1  = 100 | 1 = 101
	 printf("%f %d", x[i], l[i]);
	 binary[i] =  (x[i]-l[i]) ? 0 : 1;
	 printf("binary[%d] : %d \n",i, binary[i]);
	 bString = bString<<1 | binary[i];
	} 
	printf("\n");
	// write 000 always. 

	// update the lMatrix as well. 
	lMatrix[l[0]][l[1]][l[2]]++;
	// push r into to corresponding l[] index of point list
	push(&pointList[l[0]][l[1]][l[2]],r);
	int b1,b2,b3;
	int done[8]={0,0,0,0,0,0,0,0},result;
	for ( i=1; i <= bString; i++)   {
		result = bString & i;
		if (result ) {
			b1=result&4 ? 1:0;
			b2=result&2 ? 1:0;
			b3=result&1 ? 1:0;
			// update the lMatrix
			if (!done[result]) {
				lMatrix[l[0]-b1][l[1]-b2][l[2]-b3]++;
				push(&pointList[l[0]-b1][l[1]-b2][l[2]-b3],r);
				done[result]=1;
			}
			printf("%d\n", done[result]);
		}
	}
	//fclose(fPartition);
	return TRUE;
}
bool partitionAll(int L, char* inputFileName, char* outputFileName) {
	printf("Entering partitionall\n");
	float r[3];
	float **R;
	// Declaring helper variables 
	int i,j,k;
	int a1,a2,a3;
	float x1,x2,x3, temp1, temp2, temp3;
	int lMatrix[L][L][L],K=0;
	struct node* pointList[L][L][L];
	// initialise every element of lMatrix and pointList to 0 and NULL respectively.
	for ( i=0; i<L; i++) {
		for ( j=0; j<L; j++) {
			for ( k=0; k<L; k++) {
				lMatrix[i][j][k]=0;
				pointList[i][j][k] = NULL;
			}
		}
	}

	FILE *fin, *fout;
	fin = fopen(inputFileName,"r");
	if (fin != NULL) {
		char str[SIZE];
		while (fgets(str,SIZE,fin) != NULL) {
			printf("getting data from input file\n");
			/*
			temp1 = x1;
			temp2 = x2;
			temp3 = x3;
			*/
			sscanf(str, "%f %f %f", &x1, &x2, &x3);
			printf("%f %f %f\n", x1,x2,x3);
			if (x1>a || x1<0 /*|| x1==temp1*/ ) { 
				printf("Input Failed\n");
				return FALSE;
			}
			if (x2>a || x2<0 /*|| x2==temp2*/) {
				printf("Input Failed\n");
				return FALSE;
			}
			if (x3>a || x3<0 /*|| x3==temp3*/) {
				printf("Input Failed\n");
				return FALSE;
			}
			r[0] = x1;
			r[1] = x2;
			r[2] = x3;
			partition(r,L,lMatrix,pointList);
			// so we have K points. 
			K++;
		}
	}
	else {
		printf("Could not open the input File  \n");
		return 1;
	}
	fclose(fin);
	fout = fopen(outputFileName, "w");
	FILE *fPartition ;
	char str[SIZE];
	for ( i=0; i<L; i++) {
		for ( j=0; j<L; j++) {
			for ( k=0; k<L; k++) {
				fprintf(fout,"%d %d %d\n", i, j, k);
				fprintf(fout,"%d\n",lMatrix[i][j][k]);
				// write the point list corresponding to l[]
				struct node* temp = pointList[i][j][k];
				while (temp!=NULL) {
					x1 = temp->r[0];
					x2 = temp->r[1];
					x3 = temp->r[2];
					fprintf(fout,"%f %f %f\n",x1,x2,x3);
					temp=temp->next;
				}

				// open the partition file
			//	fPartition = fopen("partitionFile", "r");
			//	while (fgets(str,SIZE,fPartition) != NULL) {
			//		sscanf(str,"%d %d %d %f %f %f",&a1,&a2,&a3,&x1,&x2,&x2);
			//		if (i==a1 && j==a2 && k==a3)
			//			fprintf(fout,"%f %f %f\n",x1,x2,x3);
			//	}
			//	// close the partition file.
			//	fclose(fPartition);
			}
		}
	}
	fclose(fout);
	return TRUE;
}
int main() {
	int L=0;
	printf(" Enter L: ");
	scanf("%d",&L);
	char *inputFileName = "./inputFile";
	char *outputFileName= "./outputFile";
	printf( partitionAll(L,inputFileName,outputFileName) ? "Successful": "Unsuccessful");

}
