#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	FILE *ptr = fopen(argv[1],"r");
	long offset = atol(argv[2]);
	int readByte = atoi(argv[3]);
	char c;
	
	fseek(ptr,offset,0);
	for(int i=0;i<readByte;i++){
		c = fgetc(ptr);
		if(c==EOF) break;
		printf("%c",c);
	}

	fclose(ptr);
	return 0;
}
