#include <stdio.h>

int main(int argc,char *argv[]){
	FILE *orginal = fopen(argv[1],"r");
	FILE *copyTarget = fopen(argv[2],"w");

	while(!feof(orginal)){
		char buffer[11] ={0,};
		int c = fread(buffer,sizeof(char),10,orginal);
		fwrite(buffer,sizeof(char),c,copyTarget);
	}

	fclose(orginal);
	fclose(copyTarget);
	return 0;
}
