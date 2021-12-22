// 주의사항
// 1. blockmap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. blockmap.h에 정의되어 있지 않을 경우 본인이 이 파일에서 만들어서 사용하면 됨
// 3. 필요한 data structure가 필요하면 이 파일에서 정의해서 쓰기 바람(blockmap.h에 추가하면 안됨)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"
// 필요한 경우 헤더 파일을 추가하시오.

//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// file system에 의해 반드시 먼저 호출이 되어야 한다.
//
int dd_read(int ppn,char *pagebuf);
int dd_write(int ppn,char *pagebuf);
int dd_erase(int pbn);
int lbn[DATABLKS_PER_DEVICE];
int freeBlockNum;
//extern FILE *flashfp;


void ftl_open()
{
	//
	// address mapping table 초기화 또는 복구
	// free block's pbn 초기화
    	// address mapping table에서 lbn 수는 DATABLKS_PER_DEVICE 동일
	for(int i=0;i<DATABLKS_PER_DEVICE;i++) lbn[i] = -1;
	int value;
	char buf[PAGE_SIZE];
	char s[4];
	int temp = 0;
	dd_read(DATABLKS_PER_DEVICE*PAGES_PER_BLOCK,buf);
	for(int i = SECTOR_SIZE; i<SECTOR_SIZE+4; i++) s[temp++] = buf[i];
	temp = *(int *)s;
	if(temp<0){ // 등록된 값이 없다
		freeBlockNum = DATABLKS_PER_DEVICE;
	}else{ // 등록된 값이 있다
		lbn[temp] = DATABLKS_PER_DEVICE;
		freeBlockNum = (DATABLKS_PER_DEVICE-1)-temp;
		for(;;){
			temp = 0;
			memset(buf,0,sizeof(buf));
			dd_read(freeBlockNum*PAGES_PER_BLOCK,buf);
			for(int i=SECTOR_SIZE;i<SECTOR_SIZE+4;i++) s[temp++]=buf[i];
			temp = *(int *)s;
			if(temp<0){ break;}
			else{
				lbn[temp] = freeBlockNum;
				freeBlockNum = (DATABLKS_PER_DEVICE-1)-temp;
			}
		}
	}
	for(int i=0;i<DATABLKS_PER_DEVICE;i++){
		temp = 0;
		memset(buf,0,sizeof(buf));
		dd_read(i*PAGES_PER_BLOCK,buf);
		for(int j=SECTOR_SIZE;j<SECTOR_SIZE+4;j++) s[temp++] = buf[j];
		temp = *(int *)s;
		if(temp>=0) lbn[temp] = i;
	}
	return;
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_read(int lsn, char *sectorbuf)
{
	int index = lsn/PAGES_PER_BLOCK;
	int page = lsn%PAGES_PER_BLOCK;
	if(lbn[index]<0) return;
	int realPage = lbn[index]*PAGES_PER_BLOCK+page;
	char pagebuf[PAGE_SIZE];
	int t = dd_read(realPage,pagebuf);
	memcpy(sectorbuf,pagebuf,sizeof(sectorbuf));
	// 검증 코드
	//char s[4];
	//int len = 0;
	//char s[512];
	//for(int pass=0;pass<512;pass++) s[len++] = sectorbuf[pass];
	//len = *(int *)s;
	//printf("실페이지 %d %d\n",realPage,len);
	//for(int pass=516;pass<520;pass++) s[len++] = pagebuf[pass];	
	//index = *(int *)s;
	//printf("%d\n",index);
	//printf("realPage=%d lsnValue=%d\n",realPage,index);
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_write(int lsn, char *sectorbuf)
{
	int index = lsn/PAGES_PER_BLOCK;
	int page = lsn%PAGES_PER_BLOCK;
	int pageNum;
	char pagebuf[PAGE_SIZE];
	char spare[SPARE_SIZE];
	char s[4];
	int size = SECTOR_SIZE;
	memcpy(pagebuf,sectorbuf,sizeof(sectorbuf));
	if(lbn[index] == -1){ // 처음 쓴다
		int firstPage = ( ( DATABLKS_PER_DEVICE-1)-index)*PAGES_PER_BLOCK;
		pageNum = firstPage+page;
		if(page == 0){ // 첫 페이지에 쓴다
			memcpy(s,(char *)&index,sizeof(int));
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];	
			memcpy(s,(char *)&lsn,sizeof(int));
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			memset(s,0xFF,sizeof(s));
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			int sw = dd_write(pageNum,pagebuf);
			lbn[index] = (DATABLKS_PER_DEVICE-1)-index;
		}else{ // 첫 페이지에 쓰지 않는다
			char pagebuf2[PAGE_SIZE];
			memset(pagebuf2,0xFF,sizeof(SECTOR_SIZE));			

			memcpy(s,(char *)&index,sizeof(int));
			for(int i=0;i<4;i++) pagebuf2[size++] = s[i];
			memset(s,0xFF,sizeof(s));
			for(int i=0;i<4;i++) pagebuf2[size++] = s[i];
			for(int i=0;i<4;i++) pagebuf2[size++] = s[i];
			for(int i=0;i<4;i++) pagebuf2[size++] = s[i];
			dd_write(firstPage,pagebuf2);

			size = SECTOR_SIZE;

			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			memcpy(s,(char *)&lsn,sizeof(int));
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			memset(s,0xFF,sizeof(s));
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			for(int i=0;i<4;i++) pagebuf[size++] = s[i];
			dd_write(pageNum,pagebuf);
			lbn[index] = (DATABLKS_PER_DEVICE-1)-index;	
		}
		
	}else{ // 덮어쓰기 하는 경우 vs 넣기만 하는 경우 lsn으로 판단
		int insertPage = lbn[index]*PAGES_PER_BLOCK+page; // 실제 물리적 페이지
		int realOffset = size + 4; // 섹터 사이즈 건너뛰고 lbn건너 뛴다
		int temp = 0;
		char buf[PAGE_SIZE];
		dd_read(insertPage,buf);
		for(;realOffset<SECTOR_SIZE+8;realOffset++){
			s[temp++] = buf[realOffset];
		}
		temp = *(int *)s;
		if(temp >= 0){ // 갱신해야함
			char page[PAGE_SIZE];
			for(int i=0;i<PAGES_PER_BLOCK;i++){
				//freeBlockNum*PAGES_PER_BLOCK+i;
				if( (lbn[index]*PAGES_PER_BLOCK+i) == insertPage){
					if( i == 0) { // 첫페이지
						memcpy(s,(char *)&index,sizeof(int));
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
						memcpy(s,(char *)&lsn,sizeof(int));
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
						memset(s,0xFF,sizeof(s));
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
					}else{ // 첫페이지가 아님
						memset(s,0xFF,sizeof(s));
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
						memcpy(s,(char *)&lsn,sizeof(int));
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
						memset(s,0xFF,sizeof(s));
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
						for(int j=0;j<4;j++) pagebuf[size++]=s[j];
					}
					dd_write(freeBlockNum*PAGES_PER_BLOCK+i,pagebuf);
				}else{ // 그냥 붙이면 됨
					dd_read(lbn[index]*PAGES_PER_BLOCK+i,page);
					dd_write(freeBlockNum*PAGES_PER_BLOCK+i,page);
				}
			}
			dd_erase(lbn[index]);
			int t = lbn[index];
			lbn[index] = freeBlockNum;
			freeBlockNum = t;
		}else{ // 그냥 삽입하면 끝남
			if(page == 0){ // 처음 오프셋에 넣는 경우
				memcpy(s,(char *)&index,sizeof(int));
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];	
				memcpy(s,(char *)&lsn,sizeof(int));
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				memset(s,0xFF,sizeof(s));
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				dd_write(insertPage,pagebuf);
			}else{
				memset(s,0xFF,sizeof(s));
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				memcpy(s,(char *)&lsn,sizeof(int));
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				memset(s,0xFF,sizeof(s));
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				for(int i=0;i<4;i++) pagebuf[size++] = s[i];
				dd_write(insertPage,pagebuf);
			}		
		}
	}

	return;
}

void ftl_print()
{
	printf("lbn  pbn\n");
	for(int i=0;i<DATABLKS_PER_DEVICE;i++){
		printf("%d  %d\n",i,lbn[i]);
	}
	printf("free block's pbn=%d\n",freeBlockNum);
	return;
}
