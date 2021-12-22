#include <stdio.h>
void merge(FILE *from,FILE *to);
int main(int argc,char *argv[]){
	FILE *target = fopen(argv[1],"w");
	FILE *merge1 = fopen(argv[2],"r");
	FILE *merge2 = fopen(argv[3],"r");

	merge(merge1,target);
	merge(merge2,target);

	fclose(target);
	fclose(merge1);
	fclose(merge2);
	return 0;
}

void merge(FILE *from, FILE *to){
	while(!feof(from)){
		char buffer[11] = {0,};
		int c = fread(buffer,sizeof(char),10,from);
		fwrite(buffer,sizeof(char),c,to);
	}
}
