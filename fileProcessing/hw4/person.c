#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
// 필요한 경우 헤더 파일과 함수를 추가할 수 있음

// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 데이터를 읽고 쓸 때도
// 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 pagebuf에 저장한다
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓰거나 삭제 레코드를 수정할 때나
// 위의 readPage() 함수를 호출하여 pagebuf에 저장한 후, 여기에 필요에 따라서 새로운 레코드를 저장하거나
// 삭제 레코드 관리를 위한 메타데이터를 저장합니다. 그리고 난 후 writePage() 함수를 호출하여 수정된 pagebuf를
// 레코드 파일에 저장합니다. 반드시 페이지 단위로 읽거나 써야 합니다.
//
// 주의: 데이터 페이지로부터 레코드(삭제 레코드 포함)를 읽거나 쓸 때 페이지 단위로 I/O를 처리해야 하지만,
// 헤더 레코드의 메타데이터를 저장하거나 수정하는 경우 페이지 단위로 처리하지 않고 직접 레코드 파일을 접근해서 처리한다.

//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
int allPage;
int allRecord;
int deletedPageNum;
int deletedRecordNum;
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
	rewind(fp);
	fseek(fp,16,SEEK_SET);
	for(int i=0;i<pagenum;i++) fseek(fp,PAGE_SIZE,SEEK_CUR);
	fread(pagebuf,PAGE_SIZE,1,fp);
	/*int size=0; char s[4]; for(int i=0;i<4;i++) s[size++] = pagebuf[i]; size=0;
	printf("%d\n",*(int *)s);
	for(int i=4;i<8;i++) s[size++] = pagebuf[i]; size=0;
	printf("%d\n",*(int *)s);
	for(int i=8;i<12;i++) s[size++] = pagebuf[i];
	printf("%d\n", *(int *)s);*/
}
//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 레코드 파일의 위치에 저장한다. 
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	rewind(fp);
	fseek(fp,16,SEEK_SET);
	for(int i=0;i<pagenum;i++) fseek(fp,PAGE_SIZE,SEEK_CUR);
	fwrite(pagebuf,PAGE_SIZE,1,fp);
	/*int size = 0; char s[4]; for(int i=0;i<4;i++) s[size++] = pagebuf[i]; size=0;
	printf("%d\n",*(int *)s);
	for(int i=4;i<8;i++) s[size++] = pagebuf[i]; size=0;
	printf("%d\n",*(int *)s);
	for(int i=8;i<12;i++) s[size++] = pagebuf[i];
	printf("%d\n",*(int *)s);*/
}

//
// 새로운 레코드를 저장할 때 터미널로부터 입력받은 정보를 Person 구조체에 먼저 저장하고, pack() 함수를 사용하여
// 레코드 파일에 저장할 레코드 형태를 recordbuf에 만든다. 
// 
void pack(char *recordbuf, const Person *p)
{
	int i=0;
	int len = 0;
	for(len=0;len<strlen(p->id);len++) {recordbuf[i++] = p->id[len];}
	recordbuf[i++] = '#';
	for(len=0;len<strlen(p->name);len++) {recordbuf[i++] = p->name[len];}
	recordbuf[i++] = '#';
	for(len=0;len<strlen(p->age);len++) {recordbuf[i++] = p->age[len];}
	recordbuf[i++] = '#';
	for(len=0;len<strlen(p->addr);len++) {recordbuf[i++] = p->addr[len];}
	recordbuf[i++] = '#';
	for(len=0;len<strlen(p->phone);len++) {recordbuf[i++] = p->phone[len];}
	recordbuf[i++] = '#';
	for(len=0;len<strlen(p->email);len++) {recordbuf[i++] = p->email[len];}
	recordbuf[i++] = '#';//recordbuf[i]='\0';
}

// 
// 아래의 unpack() 함수는 recordbuf에 저장되어 있는 레코드를 구조체로 변환할 때 사용한다.
//
void unpack(const char *recordbuf, Person *p)
{
	char str[15]; 
	int i;
	for(i=0;i<strlen(recordbuf);i++){
		if(recordbuf[i] == '#'){
			break;
		}
		str[i] = recordbuf[i];
	}
	str[i] = '\0';
	strcpy(p->id,str);
}

//
// 새로운 레코드를 저장하는 기능을 수행하며, 터미널로부터 입력받은 필드값들을 구조체에 저장한 후 아래 함수를 호출한다.
//
void add(FILE *fp, const Person *p)
{
	int recordSize = strlen(p->id)+strlen(p->name)+strlen(p->age)+strlen(p->addr)+
		   strlen(p->phone)+strlen(p->email)+6;
	char recordBuf[recordSize];
	pack(recordBuf,p);
	char pageBuf[PAGE_SIZE];
	int index=0;
	char s[4];
	int i,j;
	int su;
	if(allPage > 0){ // 기존 페이지를 가지고 온다
		int first = 0;
		int sw = 0; // 만약 해당 값이 0이면 right 만족 못하거나 삭제 레코드 없는 것
		if(deletedPageNum!=-1 && deletedRecordNum!=-1){ // 삭제 레코드 있다.
			char deletePage[PAGE_SIZE];
			int page = deletedPageNum; int record = deletedRecordNum;
			int prevPage,prevRecord;
			readPage(fp,deletePage,page);
			
			while(page!=-1 && record!=-1){
				su = 8*(record+1);
				for(i=0;i<4;i++) s[i] = deletePage[su++];
				su = *(int *)s;
				if(recordSize<=su){
					sw = 1;
					su = 4 + (record*8);

					for(i=0;i<4;i++) s[i] = deletePage[su++];
					int depth = *(int *)s;
					depth = depth+HEADER_AREA_SIZE;
					index = depth;
					depth++;
					for(i=0;i<4;i++) s[i] = deletePage[depth++];
					deletedPageNum = *(int *)s;
					for(i=0;i<4;i++) s[i] = deletePage[depth++];
					deletedRecordNum = *(int *)s;

					for(i=0;i<recordSize;i++){
						deletePage[index++] = recordBuf[i];
					}
					writePage(fp,deletePage,page);
					
					if(first == 0){
						fseek(fp,8,SEEK_SET);
						memcpy(s,(char *)&deletedPageNum,sizeof(int));
						fwrite((void *)s,sizeof(s),1,fp);
						memcpy(s,(char *)&deletedRecordNum,sizeof(int));
						fwrite((void *)s,sizeof(s),1,fp);
					}else{
						char updatePage[PAGE_SIZE];
						readPage(fp,updatePage,prevPage);
						int d = 4 + (prevRecord*8);
						for(i=0;i<4;i++) s[i] = updatePage[d++];
						d = *(int *)s+HEADER_AREA_SIZE+1;
						memcpy(s,(char *)&deletedPageNum,sizeof(int));
						for(i=0;i<4;i++) updatePage[d++] = s[i];
						memcpy(s,(char *)&deletedRecordNum,sizeof(int));
						for(i=0;i<4;i++) updatePage[d++] = s[i];
						writePage(fp,updatePage,prevPage);
					}
					break;
				}else{ // 만족 못하므로
					prevPage = page;
					prevRecord = record;
					su = 4 + (record*8);
					for(i=0;i<4;i++) s[i] = deletePage[su++];
					int depth = *(int *)s;
					depth = depth + HEADER_AREA_SIZE;
					depth++; // 별 위치는 건너뜀
					for(i=0;i<4;i++) s[i] = deletePage[depth++];
					page = *(int *)s;
					for(i=0;i<4;i++) s[i] = deletePage[depth++];
					record = *(int *)s;
					if(page==-1 && record==-1) break; 
					readPage(fp,deletePage,page);
					first++; // 헤드 매칭이 아니다
				}
			}
		}
		if(sw==0){ // 삭제 레코드가 없다 혹은 라이트 만족 못함
			readPage(fp,pageBuf,allPage-1);
			for(i=0;i<4;i++) s[index++] = pageBuf[i];
			int value = *(int *)s;
			int limit = 4 + (8 * (value+1));
			int sum = 0;
			for(i=0;i<value;i++){
				index = 8 + (8*i);
				//index = index-4;
				for(j=0;j<4;j++) s[j] = pageBuf[index++];
				sum += *(int *)s;
			}
			sum += recordSize;
			rewind(fp);
			index = 0;
			if( (limit>HEADER_AREA_SIZE) || (sum>DATA_AREA_SIZE) ){ // 새페이지
				allPage++; allRecord++;	
				char pageBuf2[PAGE_SIZE];
				memcpy(s,(char *)&allPage,sizeof(int));
				fwrite((void *)s,sizeof(s),1,fp);
				memcpy(s,(char *)&allRecord,sizeof(int));
				fwrite((void *)s,sizeof(s),1,fp);

				i=1; memcpy(s,(char *)&i,sizeof(int));
				for(i=0;i<4;i++) pageBuf2[index++] = s[i];
				i=0; memcpy(s,(char *)&i,sizeof(int));
				for(;i<4;i++) pageBuf2[index++] = s[i];
				memcpy(s,(char *)&recordSize,sizeof(int));
				for(i=0;i<4;i++) pageBuf2[index++] = s[i];
				index = HEADER_AREA_SIZE;
				for(i=0;i<recordSize;i++) pageBuf2[index++] = recordBuf[i];
				writePage(fp,pageBuf2,allPage-1);
			}else{ // 기존페이지에 삽입
				allRecord++;
				fseek(fp,4,0);
				memcpy(s,(char *)&allRecord,sizeof(int));
				fwrite((void *)s,sizeof(s),1,fp);
				int offset = sum - recordSize;	
				value++;
				memcpy(s,(char *)&value,sizeof(int));
				for(i=0;i<4;i++) pageBuf[index++] = s[i];

				index = 4 + ((value-1) * 8);
				memcpy(s,(char *)&offset,sizeof(int));
				for(i=0;i<4;i++) pageBuf[index++] = s[i];
				memcpy(s,(char *)&recordSize,sizeof(int));
				for(i=0;i<4;i++) pageBuf[index++] = s[i];		
				index = offset+HEADER_AREA_SIZE;
				for(i=0;i<recordSize;i++) pageBuf[index++] = recordBuf[i];
				writePage(fp,pageBuf,allPage-1);
			}
			
		}
	}else if(allPage == 0){ // 새 페이지를 만든다
		allPage++; allRecord++;

		rewind(fp);
		memcpy(s,(char *)&allPage,sizeof(int));
		fwrite((void *)s,sizeof(s),1,fp);
		memcpy(s,(char *)&allRecord,sizeof(int));
		fwrite((void *)s,sizeof(s),1,fp);

		for(i=0;i<4;i++) pageBuf[index++] = s[i];
		//printf("%d\n",*(int *)s);
		i = 0; memcpy(s,(char *)&i,sizeof(int));
		for(;i<4;i++) pageBuf[index++] = s[i];
		memcpy(s,(char *)&recordSize,sizeof(int));
		for(i=0;i<4;i++) pageBuf[index++] = s[i];
		index = HEADER_AREA_SIZE;
		for(i=0;i<recordSize;i++) pageBuf[index++] = recordBuf[i];
		writePage(fp,pageBuf,0);
	}
	
}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete(FILE *fp, const char *id)
{
	int page = allPage-1;
	char pageBuf[PAGE_SIZE];
	char s[4];
	int i,j;
	int value;
	int offset;
	int val,val2;

	Person person;
	
	while(page>=0){
		readPage(fp,pageBuf,page);
		for(i=0;i<4;i++) s[i] = pageBuf[i];
		value = *(int *)s;
		for(i=0;i<value;i++){
			offset = 4+8*i;
			for(j=0;j<4;j++) s[j] = pageBuf[offset++];
			val = *(int *)s; // 인덱스 저장을 위함
			for(j=0;j<4;j++) s[j] = pageBuf[offset++];
			val2 = *(int *)s; // 길이 저장을 위함
			offset = val + HEADER_AREA_SIZE; // 데이터 있는 위치 시작점
			char recordBuf[val2];
			for(j=0;j<val2;j++) recordBuf[j] = pageBuf[offset++];
			unpack(recordBuf,&person);
			if(strcmp(person.id,id)==0){ // 일치 문자열 찾음
				deletedPageNum = page;
				deletedRecordNum = i;
				offset = val + HEADER_AREA_SIZE;
				char spare[8];

				fseek(fp,8,SEEK_SET);
				fread((void *)spare,sizeof(spare),1,fp);
				pageBuf[offset++] = '*';
				for(j=0;j<8;j++) pageBuf[offset++] = spare[j];
				writePage(fp,pageBuf,page);
				
				memcpy(s,(char *)&deletedPageNum,sizeof(int));
				for(j=0;j<4;j++) spare[j] = s[j];
				memcpy(s,(char *)&deletedRecordNum,sizeof(int));
				for(j=0;j<4;j++) spare[j+4] = s[j];
				fseek(fp,8,SEEK_SET);
				fwrite((void *)spare,sizeof(spare),1,fp);
			}
		}
		page--;
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;  // 레코드 파일의 파일 포인터
	fp = fopen(argv[2],"r+");
	char spare[16];
	char s[4];
	int i,j;
	int size=0;
	Person person;

	if(fp == NULL){ // 레코드 파일이 없는 경우
		fp = fopen(argv[2],"w");
		memcpy(s,(char *)&size,sizeof(int));
		for(i=0;i<2;i++) for(j=0;j<4;j++) spare[size++] = s[j];
		memset(s,0xFF,sizeof(s));
		for(i=0;i<2;i++) for(j=0;j<4;j++) spare[size++] = s[j];
		fwrite((void *)spare,sizeof(spare),1,fp);
		allPage = 0; allRecord = 0; deletedPageNum = -1; deletedRecordNum = -1;
	}else{
		fread((void *)spare,sizeof(spare),1,fp);
		for(i=0;i<4;i++) s[size++] = spare[i]; 
		allPage = *(int *)s; size=0;
		for(i=4;i<8;i++) s[size++] = spare[i];
		allRecord = *(int *)s; size=0;
		for(i=8;i<12;i++) s[size++] = spare[i];
		deletedPageNum = *(int *)s; size=0;
		for(i=12;i<16;i++) s[size++] = spare[i];
		deletedRecordNum = *(int *)s; size=0;
	}	
	//printf("%d %d %d %d\n",allPage,allRecord,deletedPageNum,deletedRecordNum);
	//return ;
	if(strcmp(argv[1],"a")==0){ // 레코드 추가
		strcpy(person.id,argv[3]);
		strcpy(person.name,argv[4]);
		strcpy(person.age,argv[5]);
		strcpy(person.addr,argv[6]);
		strcpy(person.phone,argv[7]);
		strcpy(person.email,argv[8]);
		add(fp,&person);
	}else if(strcmp(argv[1],"d")==0){
		delete(fp,argv[3]);
	}
	fclose(fp);
	return 1;
}
