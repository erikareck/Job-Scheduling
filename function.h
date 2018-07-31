#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SWAP(x, y, t) ((t)=(x), (x)=(y), (y)=(t))

int** readfile(char*, int*);//讀文字檔 (e.g. 10000data_1.txt 格式)
void printArray(int**, int);//二維陣列印出debug用
void max_heapify(int**, int, int, int); //使父節點 > 子節點
void max_heap(int **, int, int);//建構max-heap tree
void heapSort(int**, int, int);//heapsort
int deleteHeap(int**, int*, int, int);//刪除max node, 回傳被刪除node的值
int feasible(int**, int);//判斷目前Job Sequence中是否feasible
void SPT(char*);
void EDD(char*);

int** readfile(char* filename, int* n){

	/*
		讀文字檔
		動態分配 2維陣列 int** SJ (sequence jobs)
		存放方式
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

	//	二維陣列印出debug用
	int i;

	for (i=0; i<n; i++){
		printf ("a[%d][0]=%d\t, ", i, a[i][0]);
		printf ("a[%d][1]=%d\n", i, a[i][1]);
	}
	printf("Array Size=%d\n", n);

	return;
}

void max_heapify(int** a, int root, int n, int index){

	//使父節點 > 子節點
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
	//先建構 max-heap tree再排序
	//index 表示是依照 pt(0) 或 dd(1) 的值排序
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

	//刪除max node, 回傳被刪除node的值
	int x, i, j, *tmp;

	if (*n==0) {
		printf("ii=%d, empty max-heap tree.\n", ii);
	}
	else{
		x = a[0][index];//刪除最大點
		*n=(*n)-1;
		SWAP(a[0], a[*n], tmp);


		for (i=*n/2-1; i>=0; i--){ //調整 Max-Heap tree
			max_heapify(a, i, *n, index);
		}

	}
	//printf("array size = %d\t", *n);
	//printf(",delete node's value = %d\n", x);
	//printArray(a, *n);
	return x;
}

int feasible(int**a, int n){

	//判斷目前Job Sequence中是否feasible
	int i, dd=0;

	heapSort(a, n, 1); //依照每個工作的 due date排序，若是有任何一個工作逾期則認定為unfeasible並回傳 0
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
	heapSort(SJ, n, 1); //依 dd 排序

	int longest_pt;

	J[0]=SJ[0];//將第一個 job 排進 Job Sequence
	dd=dd+SJ[0][0];
	nJ++;

	for (i=1; i<n; i++){
		if (dd+SJ[i][0] <= SJ[i][1]){//Total processing time 不大於最後一個工作的 due date 則排進 Job Sequence
			J[nJ]=SJ[i];
			nJ++;
			dd=dd+SJ[i][0];
		}else{//否則刪除目前在 Job Sequence 中最大的 pt 的工作
			nT++;
			max_heap(J, nJ, 0); //建構 max-heap tree
			if (J[0][0] >= SJ[i][0]){
				longest_pt=deleteHeap(J, &nJ, 0, i);//刪除節點
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
	heapSort(SJ, n, 0); //依 pt 排序

	int longest_pt;

	J[0]=SJ[0]; //將第一個 job 排進 Job Sequence
	dd=dd+SJ[0][0];
	nJ++;

	for (i=1;i<n;i++){
		J[nJ]=SJ[i];

		nJ++;
		if (feasible(J, nJ)==1){ //判斷 feasible 決定是否捨棄目前的工作
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

