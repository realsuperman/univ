#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	FILE *ptr = fopen(argv[1],"r");
	long offset = atol(argv[2]);
	int deleteBytes = atoi(argv[3]);
	char c;
	
	fseek(ptr,0,SEEK_END);
	int end = ftell(ptr);
	char str[end];
	int index=0;
	rewind(ptr);

	for(int i=0;i<offset;i++){ // copy
		c = fgetc(ptr);
		if(c==EOF) break;
		str[index++]=c;
	}
	if(c!=EOF){
		for(int i=0;i<deleteBytes;i++){ // skip
			c = fgetc(ptr);
			if(c==EOF) break;
		}
		if(c!=EOF){
			for(int i=ftell(ptr);i<end;i++){ // copy
				c = fgetc(ptr);
				str[index++]=c;
			}
		}
	}
	str[index]='\0';
	fclose(ptr);
	ptr = fopen(argv[1],"w");
	fwrite(str,sizeof(char),strlen(str),ptr);
	fclose(ptr);
	return 0;
}
