#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	FILE *ptr = fopen(argv[1],"r");
	long offset = atol(argv[2]);
	fseek(ptr,0,SEEK_END);
	int end = ftell(ptr);
	int index = end+strlen(argv[3])+1;
	char str[index];
	char c;

	index = 0;
	rewind(ptr);
		
	for(int i=0;i<=offset;i++){
		c = fgetc(ptr);
		if(c==EOF) break;
		str[index++] = c;
	}
	for(int i=0;i<strlen(argv[3]);i++){
		str[index++] = argv[3][i]; 
	}
	for(int i=ftell(ptr);i<=end;i++){
		c = fgetc(ptr);
		if(c==EOF) break;
		str[index++] = c;
	}
	str[index] = '\0';
	
	fclose(ptr);
	ptr = fopen(argv[1],"w");
	fwrite(str,sizeof(char),strlen(str),ptr);
	fclose(ptr);
	return 0;
}
