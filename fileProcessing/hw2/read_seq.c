#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char **argv) {
	struct timeval start,end;
	int time;
	FILE *ptr = fopen(argv[1],"r");
	int length;
	char buffer[251];
	
	gettimeofday(&start,NULL);
	fread(&length,sizeof(int),1,ptr);
	for(int i=0;i<length;i++){
		fread(buffer,sizeof(char),250,ptr);
		buffer[250]='\0';//값을 찍기 위해 널문자 삽입
		//printf("%s",buffer);
		memset(buffer,0,251*sizeof(char));
		//printf("\n");
	}
	gettimeofday(&end,NULL);
	time = 1000000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec);
	printf("#records: %d elapsed_time: %d us\n",length,time);
	fclose(ptr);
	return 0;
}
