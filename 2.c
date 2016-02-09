#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#define SIZE  100
#define FALSE 0
#define TRUE 1
#define a 16
bool MATRIX(int L, int l[3], char* fileForP, char* fileForQ, char* fileForMatrix) {
	float x[3], temp1, temp2, temp3;
	float x1,x2,x3;
	FILE *fp, *fq, *fMatrix;
	int N=0, M=0,i,j;
	float P[100][3],Q[100][3];
	// Initialise P and Q to 0
	for (i=0; i < 100; i++) {
		for ( j=0;j<3; j++) {
			P[i][j] = Q[i][j] = 0;
		}
	}
	fp = fopen(fileForP,"r");
	if (fp != NULL) {
		char str[SIZE];
		// Checking validity of input P and pushing valid data into P matrix
		while (fgets(str,SIZE,fp) != NULL) {
			sscanf(str, "%f%f%f", &x[0], &x[1], &x[2]);
			int flag = 0;
			for ( i=0; i<3; i++) {
			// check for 0<= x[i] <= a
				if (x[i]>a || x[i]<0) {
					printf("initial condition : Invalid Input for P");
					return FALSE;
				}
				if (!( (((a*l[i])/L == x[i]) ||(x[i] == (a*(l[i]+1)/L)))   &&   ( ((a*l[(i+1)%3])/L <= x[(i+1)%3]) && (x[(i+1)%3]<= (a*(l[(i+1)%3]+1)/L)) )   &&   ( ((a*l[(i+2)%3])/L <= x[(i+2)%3]) &&  (x[(i+2)%3]<= (a*(l[(i+2)%3]+1))/L) ))) {
					flag++;
				}
			}
			/*
				If the condition fails for all three co-ordinates,
				the point is not a boundary point
			*/
			if (flag==3) {
				printf("Invalid Input for P\n");
				return FALSE;
			}
			P[N][0] = x[0];
			P[N][1] = x[1];
			P[N][2] = x[2];
 			/* This will count no. of inputs in file for P*/
			N++;
		}
	} else {
		printf("Could not open the input File  \n");
		return 1;
	}
	fclose(fp);
	fq = fopen(fileForQ,"r");
	// Checking validity of input Q and pushing valid data into Q Matrix
	if (fq != NULL) {
		char str2[SIZE];
		while (fgets(str2,SIZE,fq) != NULL) {
			sscanf(str2, "%f%f%f", &x[0], &x[1], &x[2]);
			// Calculate the radius of the sphere. 
			float sum= powf(2*a/(float)L,2);
			// Check if the point lies on the sphere.
			for(i=0; i<3 ; i++)
				sum-= powf(x[i]-((2*l[i]+1)*a)/(2*L), 2);

			// Decide the validity accordingly
			if(sum) {
					printf("Input data for Q is Invalid");
					return false; 
				}
			Q[M][0] = x[0];
			Q[M][1] = x[1];
			Q[M][2] = x[2];
 			/* This will count no. of inputs in file for Q*/
			M++;
		}
 } else {
		printf("Could not open the input File  \n");
		return 1;
	}
	fclose(fq);
	// Construction of matrix A 
	float A[M][N]; 
	for ( i =0; i < M; i++) {
		for ( j =0; j < N; j++) {
			float temp;
			// sqrt( x^2 + y^2 + z^2 ) 
			temp = sqrt( powf((Q[i][0] - P[j][0]), 2) + powf((Q[i][1] - P[j][1]), 2) + powf((Q[i][2] - P[j][2]), 2));
			A[i][j]= temp ? (sinf(temp))/temp : 1;
		}
	}
	// Finding the Product Matrix of A and Transpose of A 
	float result[N][N];
		for ( i =0; i < N; i++) {
			for ( j =0; j <= i; j++) {
				result[i][j] = 0;
				for ( int k=0; k<N; k++) {
					result[i][j]+= A[k][i] * A[k][j];
				}
				result[j][i] = result[i][j];
			}
		}
	fMatrix = fopen(fileForMatrix, "w");
		for ( i =0; i < M; i++) {
			for ( j =0; j < M; j++) {
					fprintf(fMatrix, "%d %d %f\n", i, j, result[i][j]);
				}
		}
	fclose(fMatrix);
}
				
int main( int argc, char* argv[]) {
// check that whether file names are provided or not at the command line.
	if(argc!=4) { 
		printf("File Names are not provided at command line");
		return 1;
	}
	int L;
	do { 
		printf("Enter a Positive Integer: ");
		scanf("%d",&L);
	}
	while(L<=0);
	int l[3];
	do { 
		printf("\nEnter 3 integers less than %d: ",L);
		scanf("%d%d%d",&l[0],&l[1],&l[2]);
	}
	while(!(l[0]<L && l[1]<L && l[2]<L && l[0]>=0 && l[1]>=0 && l[2]>=0));
	bool result=MATRIX(L,l,argv[1],argv[2],argv[3]);
	switch(result) {
		case true: printf("\nAlgorithm is implemented Successfully\n");
								break;
		case false: printf("\nAlgorithm is implemented Successfully\n");
								break;
	}
	return 0;
}


