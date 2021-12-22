#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
	int length = atoi(argv[1]);
	FILE *ptr = fopen(argv[2],"w");
	char str[250];

	fwrite(&length,sizeof(int),1,ptr);
	for(int i=0;i<250;i++){
		str[i]='a';
	}

	for(int i=0;i<length;i++){
		fwrite(str,1,sizeof(str),ptr);
	}

	fclose(ptr);
	return 0;
}
