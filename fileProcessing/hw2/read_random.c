#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define SUFFLE_NUM	10000 
void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);

int main(int argc, char **argv) {
	struct timeval start,end;
	int time;
	FILE *ptr = fopen(argv[1],"r");
	int *read_order_list;
	int num_of_records;
	char buffer[251];

	gettimeofday(&start,NULL);
	fread(&num_of_records,sizeof(int),1,ptr);
	int array[num_of_records];
	GenRecordSequence(array, num_of_records);
	read_order_list = array;
	for(int i=0;i<num_of_records;i++){
		fseek(ptr,(read_order_list[i]*250)+4,SEEK_SET);
		fread(buffer,sizeof(char),250,ptr);
		buffer[250]='\0';
		//printf("%s",buffer);
		memset(buffer,0,251*sizeof(char));
		//printf("\n");
	}
	gettimeofday(&end,NULL);
	time = 1000000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec);
	printf("#records: %d elapsed_time: %d us\n",num_of_records,time);

	fclose(ptr);
	return 0;
}

void GenRecordSequence(int *list, int n) {
	int i, j, k;
	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}
}

void swap(int *a, int *b) {
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
