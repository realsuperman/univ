#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	FILE *ptr = fopen(argv[1],"r+");
	long offset = atol(argv[2]);
	//fseek(ptr,-ftell(ptr),SEEK_CUR);
	fseek(ptr,offset,0);
	fwrite(argv[3],sizeof(char),strlen(argv[3]),ptr);
	fclose(ptr);
	return 0;
}
