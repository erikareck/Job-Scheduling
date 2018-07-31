#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SWAP(x, y, t) ((t)=(x), (x)=(y), (y)=(t))

int** readfile(char*, int*);//Ū��r�� (e.g. 10000data_1.txt �榡)
void printArray(int**, int);//�G���}�C�L�Xdebug��
void max_heapify(int**, int, int, int); //�Ϥ��`�I > �l�`�I
void max_heap(int **, int, int);//�غcmax-heap tree
void heapSort(int**, int, int);//heapsort
int deleteHeap(int**, int*, int, int);//�R��max node, �^�ǳQ�R��node����
int feasible(int**, int);//�P�_�ثeJob Sequence���O�_feasible
void SPT(char*);
void EDD(char*);

int** readfile(char* filename, int* n){

	/*
		Ū��r��
		�ʺA���t 2���}�C int** SJ (sequence jobs)
		�s��覡
		SJ[0]->{p1, d1}
		SJ[1]->{p2, d2}
		SJ[2]->{p3, d3}
		......
	*/
	FILE *fPtr;
	int i, pt, dd, **SJ;

	fPtr = fopen( filename,"r");
	if (fPtr == NULL)	{
		printf("File could not be opened\n");
	}
	else{
		//printf("file %s has been open.\n", filename);
		fscanf (fPtr, "%d", n);
		SJ = (int**)malloc(*n * sizeof(int *));

		if (SJ == NULL){
			printf("Out of memory malloc(int*)\n");
			printf("dynamically Allocating failed!\n");
		}else{
			for (i=0; i<*n; i++){
				SJ[i] = (int*)malloc(2 * sizeof(int));
				if (SJ[i] == NULL){
					printf("Out of memory: malloc(int)\n");
				}else{
					fscanf(fPtr, "%d", &SJ[i][0]);
					fscanf(fPtr, "%d", &SJ[i][1]);
					//printf ("SJ[%d][0]=%d\t, ", i, SJ[i][0]);
					//printf ("SJ[%d][1]=%d\n", i, SJ[i][1]);
				}
			}
			//printf("Jobs number= %d\n", *n);
		}


	}
	fclose(fPtr);
	return SJ;
}

void printArray(int** a, int n){

	//	�G���}�C�L�Xdebug��
	int i;

	for (i=0; i<n; i++){
		printf ("a[%d][0]=%d\t, ", i, a[i][0]);
		printf ("a[%d][1]=%d\n", i, a[i][1]);
	}
	printf("Array Size=%d\n", n);

	return;
}

void max_heapify(int** a, int root, int n, int index){

	//�Ϥ��`�I > �l�`�I
	int max, *tmp;
	int left = root*2+1;
	int right= root*2+2;

	if (left<n && (a[left][index]>a[root][index])){
		max=left;
	}else{
		max=root;
	}
	if (right<n && (a[right][index]>a[max][index])){
		max=right;
	}

	if (max!=root){
		SWAP(a[max], a[root], tmp);
		max_heapify(a, max, n, index);
	}
	return;
}

void heapSort(int** a, int n, int index){

	//heapsort
	//���غc max-heap tree�A�Ƨ�
	//index ��ܬO�̷� pt(0) �� dd(1) ���ȱƧ�
	int i, *tmp;

	for (i=n/2-1; i>=0; i--){ //build max-heap tree
		max_heapify(a, i, n, index);
	}
	for (i=n-1; i>0; i--){//sorting
		SWAP(a[0], a[i], tmp);
		max_heapify(a, 0, i, index);
	}
	return;
}

void max_heap(int **a, int n, int index){

	//build max-heap tree
	int i;

	for (i=n/2-1; i>=0; i--){
		max_heapify(a, i, n, index);
	}
	return;
}


int deleteHeap(int **a, int* n, int index, int ii){

	//�R��max node, �^�ǳQ�R��node����
	int x, i, j, *tmp;

	if (*n==0) {
		printf("ii=%d, empty max-heap tree.\n", ii);
	}
	else{
		x = a[0][index];//�R���̤j�I
		*n=(*n)-1;
		SWAP(a[0], a[*n], tmp);


		for (i=*n/2-1; i>=0; i--){ //�վ� Max-Heap tree
			max_heapify(a, i, *n, index);
		}

	}
	//printf("array size = %d\t", *n);
	//printf(",delete node's value = %d\n", x);
	//printArray(a, *n);
	return x;
}

int feasible(int**a, int n){

	//�P�_�ثeJob Sequence���O�_feasible
	int i, dd=0;

	heapSort(a, n, 1); //�̷ӨC�Ӥu�@�� due date�ƧǡA�Y�O������@�Ӥu�@�O���h�{�w��unfeasible�æ^�� 0
	for (i=0; i<n; i++){
		dd=dd+a[i][0];
		if (dd>a[i][1]){
			return 0;
		}
	}
	return 1;
}

void EDD(char* filename) {

	//	EDD

	int **SJ, n, *tmp, **J, nJ=0, nT=0, dd=0, i;
	clock_t starting_time, finishing_time;

	SJ = readfile(filename, &n);
	J = (int**)malloc(n * sizeof(int *));
	if (SJ == NULL){
		printf("Out of memory malloc(int*) for Job Sequence (J). \n");
	}

	starting_time=clock();
	heapSort(SJ, n, 1); //�� dd �Ƨ�

	int longest_pt;

	J[0]=SJ[0];//�N�Ĥ@�� job �ƶi Job Sequence
	dd=dd+SJ[0][0];
	nJ++;

	for (i=1; i<n; i++){
		if (dd+SJ[i][0] <= SJ[i][1]){//Total processing time ���j��̫�@�Ӥu�@�� due date �h�ƶi Job Sequence
			J[nJ]=SJ[i];
			nJ++;
			dd=dd+SJ[i][0];
		}else{//�_�h�R���ثe�b Job Sequence ���̤j�� pt ���u�@
			nT++;
			max_heap(J, nJ, 0); //�غc max-heap tree
			if (J[0][0] >= SJ[i][0]){
				longest_pt=deleteHeap(J, &nJ, 0, i);//�R���`�I
				J[nJ]=SJ[i];
				dd=dd-longest_pt+J[nJ][0];
				nJ++;
			} else{
				//printf("decard SJ[%d]\n", i);
			}
		}
	}

	heapSort(J, nJ, 1);
	finishing_time=clock();

	printf("Total processing time: %d\n", dd);
	printf("number of jobs in Job Sequence: %d\n", nJ);
	printf("number of tardy jobs: %d\n", nT);
	printf("running time: %lf secs", (finishing_time-starting_time)/(double)(CLOCKS_PER_SEC));

	free(SJ);
	free(J);
	return;
}

void SPT(char* filename) {

	// SPT

	int **SJ, n, *tmp, **J, nJ=0, nT=0, dd=0, i;
	clock_t starting_time, finishing_time;

	SJ = readfile(filename, &n);
	J = (int**)malloc(n * sizeof(int *));

	if (SJ == NULL){
		printf("Out of memory malloc(int*) for Job Sequence (J). \n");
	}

	starting_time=clock();
	heapSort(SJ, n, 0); //�� pt �Ƨ�

	int longest_pt;

	J[0]=SJ[0]; //�N�Ĥ@�� job �ƶi Job Sequence
	dd=dd+SJ[0][0];
	nJ++;

	for (i=1;i<n;i++){
		J[nJ]=SJ[i];

		nJ++;
		if (feasible(J, nJ)==1){ //�P�_ feasible �M�w�O�_�˱�ثe���u�@
			dd=dd+SJ[i][0];
		}else{
			nT++;
			SJ[i][0]=1000000000;
			max_heap(J, nJ, 0);
			longest_pt=deleteHeap(J, &nJ, 0, i);
		}

	}

	heapSort(J, nJ, 1);
	finishing_time=clock();

	printf("Total processing time: %d\n", dd);
	printf("number of jobs in Job Sequence: %d\n", nJ);
	printf("number of tardy jobs: %d\n", nT);
	printf("running time: %lf secs", (finishing_time-starting_time)/(double)(CLOCKS_PER_SEC));

	free(SJ);
	free(J);
	return;
}

