#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
// �ʿ��� ��� ��� ���ϰ� �Լ��� �߰��� �� ����

// ���� ��������� �����ϴ� ����� ���� �ٸ� �� ������ �ణ�� ������ �Ӵϴ�.
// ���ڵ� ������ ������ ������ ���� �����Ǳ� ������ ����� ���α׷����� ���ڵ� ���Ϸκ��� �����͸� �а� �� ����
// ������ ������ ����մϴ�. ���� �Ʒ��� �� �Լ��� �ʿ��մϴ�.
// 1. readPage(): �־��� ������ ��ȣ�� ������ �����͸� ���α׷� ������ �о�ͼ� pagebuf�� �����Ѵ�
// 2. writePage(): ���α׷� ���� pagebuf�� �����͸� �־��� ������ ��ȣ�� �����Ѵ�
// ���ڵ� ���Ͽ��� ������ ���ڵ带 �аų� ���ο� ���ڵ带 ���ų� ���� ���ڵ带 ������ ����
// ���� readPage() �Լ��� ȣ���Ͽ� pagebuf�� ������ ��, ���⿡ �ʿ信 ���� ���ο� ���ڵ带 �����ϰų�
// ���� ���ڵ� ������ ���� ��Ÿ�����͸� �����մϴ�. �׸��� �� �� writePage() �Լ��� ȣ���Ͽ� ������ pagebuf��
// ���ڵ� ���Ͽ� �����մϴ�. �ݵ�� ������ ������ �аų� ��� �մϴ�.
//
// ����: ������ �������κ��� ���ڵ�(���� ���ڵ� ����)�� �аų� �� �� ������ ������ I/O�� ó���ؾ� ������,
// ��� ���ڵ��� ��Ÿ�����͸� �����ϰų� �����ϴ� ��� ������ ������ ó������ �ʰ� ���� ���ڵ� ������ �����ؼ� ó���Ѵ�.

//
// ������ ��ȣ�� �ش��ϴ� �������� �־��� ������ ���ۿ� �о �����Ѵ�. ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
int allPage;
int allRecord;
int deletedPageNum;
int deletedRecordNum;
int read;
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
// ������ ������ �����͸� �־��� ������ ��ȣ�� �ش��ϴ� ���ڵ� ������ ��ġ�� �����Ѵ�. 
// ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
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
// ���ο� ���ڵ带 ������ �� �͹̳ηκ��� �Է¹��� ������ Person ����ü�� ���� �����ϰ�, pack() �Լ��� ����Ͽ�
// ���ڵ� ���Ͽ� ������ ���ڵ� ���¸� recordbuf�� �����. 
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
// �Ʒ��� unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� ���ڵ带 ����ü�� ��ȯ�� �� ����Ѵ�.
//
void unpack(const char *recordbuf, Person *p)
{
	//char str[15]; 
	int i=0,j=0,index=0;
	for(;i<strlen(recordbuf);i++){
		if(recordbuf[i] == '#'){
			switch(j){
				case 0 : p->id[index] = '\0'; break;
				case 1 : p->name[index] = '\0'; break;
				case 2 : p->age[index] = '\0'; break;
				case 3 : p->addr[index] = '\0'; break;
				case 4 : p->phone[index] = '\0'; break;
				case 5 : p->email[index] = '\0'; break;
				default : break;
			}
			index = 0;
			j++;
		}else{
			switch(j){
				case 0 : p->id[index++]=recordbuf[i]; break;
				case 1 : p->name[index++] = recordbuf[i]; break;
				case 2 : p->age[index++] = recordbuf[i]; break;
				case 3 : p->addr[index++] = recordbuf[i]; break;
				case 4 : p->phone[index++] = recordbuf[i]; break;
				case 5 : p->email[index++] = recordbuf[i]; break;
			}
		}
		//str[i] = recordbuf[i];
	}
	//str[i] = '\0';
	//strcpy(p->id,str);
	//printf("%s %s %s %s %s %s\n",p->id,p->name,p->age,p->addr,p->phone,p->email);
}

//
// ���ο� ���ڵ带 �����ϴ� ����� �����ϸ�, �͹̳ηκ��� �Է¹��� �ʵ尪���� ����ü�� ������ �� �Ʒ� �Լ��� ȣ���Ѵ�.
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
	if(allPage > 0){ // ���� �������� ������ �´�
		int first = 0;
		int sw = 0; // ���� �ش� ���� 0�̸� right ���� ���ϰų� ���� ���ڵ� ���� ��
		if(deletedPageNum!=-1 && deletedRecordNum!=-1){ // ���� ���ڵ� �ִ�.
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
				}else{ // ���� ���ϹǷ�
					prevPage = page;
					prevRecord = record;
					su = 4 + (record*8);
					for(i=0;i<4;i++) s[i] = deletePage[su++];
					int depth = *(int *)s;
					depth = depth + HEADER_AREA_SIZE;
					depth++; // �� ��ġ�� �ǳʶ�
					for(i=0;i<4;i++) s[i] = deletePage[depth++];
					page = *(int *)s;
					for(i=0;i<4;i++) s[i] = deletePage[depth++];
					record = *(int *)s;
					if(page==-1 && record==-1) break; 
					readPage(fp,deletePage,page);
					first++; // ��� ��Ī�� �ƴϴ�
				}
			}
		}
		if(sw==0){ // ���� ���ڵ尡 ���� Ȥ�� ����Ʈ ���� ����
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
			if( (limit>HEADER_AREA_SIZE) || (sum>DATA_AREA_SIZE) ){ // ��������
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
			}else{ // ������������ ����
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
	}else if(allPage == 0){ // �� �������� �����
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
// �ֹι�ȣ�� ��ġ�ϴ� ���ڵ带 ã�Ƽ� �����ϴ� ����� �����Ѵ�.
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
			val = *(int *)s; // �ε��� ������ ����
			for(j=0;j<4;j++) s[j] = pageBuf[offset++];
			val2 = *(int *)s; // ���� ������ ����
			offset = val + HEADER_AREA_SIZE; // ������ �ִ� ��ġ ������
			char recordBuf[val2];
			for(j=0;j<val2;j++) recordBuf[j] = pageBuf[offset++];
			unpack(recordBuf,&person);
			if(strcmp(person.id,id)==0){ // ��ġ ���ڿ� ã��
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

//
// �־��� ���ڵ� ����(recordfp)�� �̿��Ͽ� ���� �ε��� ����(idxfp)�� �����Ѵ�.
//
void createIndex(FILE *idxfp, FILE *recordfp)
{
	typedef struct sort{
		unsigned int value;
		//long long value;
		char key[14];
		char page[4];
		char record[4];
	}sort;

	int i,j,k,cnt,value,offset,index,len,record=0;
	char s[4];
	char pageBuf[PAGE_SIZE];
	//char recordBuf[21];

	rewind(recordfp);
	fread((void *)s,sizeof(s),1,recordfp);	
	cnt = *(int *)s; // ���� ������ ������
	fread((void *)s,sizeof(s),1,recordfp);
	i = *(int *)s; // ���� ���ڵ� ������
	sort sortedArray[i];

	for(i=0;i<cnt;i++){ // ������ ������ ��ŭ ������
		readPage(recordfp,pageBuf,i);
		for(j=0;j<4;j++) s[j] = pageBuf[j];
		value = *(int *)s;
		for(j=0;j<value;j++){
			offset = 4+8*j;
			for(k=0;k<4;k++) s[k] = pageBuf[offset++];
			index = *(int *)s; // �ε��� ������ ����
			offset = index + HEADER_AREA_SIZE;
			if(pageBuf[offset] == '*') continue;
			for(k=0;k<13;k++){
				if(pageBuf[offset] !='#') sortedArray[record].key[k] = pageBuf[offset++];
				else if(pageBuf[offset] == '#') break;
			}
			//printf("%d\n",k);
			sortedArray[record].key[k] = '\0';
			memcpy(sortedArray[record].page,&i,sizeof(int));
			memcpy(sortedArray[record].record,&j,sizeof(int));
			//printf("%d %d\n",*(int *)sortedArray[record].page,*(int *)sortedArray[record].record);
			sortedArray[record].value = atoi(sortedArray[record].key);
			record++;
		}
	}

	sort temp;
	for(i=0;i<record-1;i++){
		for(j=0;j<record-i-1;j++){
			if( sortedArray[j].value>sortedArray[j+1].value ){
				temp = sortedArray[j];
				sortedArray[j] = sortedArray[j+1];
				sortedArray[j+1] = temp;
			}
		}
	}

	char recordBuf[21];
	for(i=0;i<record;i++){
		//printf("%d %d\n",*(int *)sortedArray[i].page,*(int *)sortedArray[i].record);
		for(j=0;j<13;j++) recordBuf[j] = sortedArray[i].key[j];
		for(k=0;k<4;k++) recordBuf[j++] = sortedArray[i].page[k];
		for(k=0;k<4;k++) recordBuf[j++] = sortedArray[i].record[k];
		fwrite((void *)recordBuf,sizeof(recordBuf),1,idxfp);
		//printf("%d %d\n",*(int *)sortedArray[i].page,*(int *)sortedArray[i].record);
		//memset(recordBuf,0,sizeof(recordBuf));
	}
	memcpy(s,&record,sizeof(int));
	rewind(idxfp);
	fwrite((void *)s,sizeof(s),1,idxfp);
}

//
// �־��� ���� �ε��� ����(idxfp)�� �̿��Ͽ� �ֹι�ȣ Ű���� ��ġ�ϴ� ���ڵ��� �ּ�, �� ������ ��ȣ�� ���ڵ� ��ȣ�� ã�´�.
// �̶�, �ݵ�� ���� �˻� �˰������� ����Ͽ��� �Ѵ�.
//
void binarysearch(FILE *idxfp, const char *id, int *pageNum, int *recordNum)
{
	char key[13];
	char s[4];
	rewind(idxfp);
	fread((void *)s,sizeof(s),1,idxfp);
	int length = *(int *)s;
	int left = 0; int right = length-1; int mid;
	int shift;
	while(left<=right){
		read++;
		mid = (left + right)/2;
		shift = 4 + (21*mid);	
		fseek(idxfp,shift,SEEK_SET);
		fread((void *)key,sizeof(key),1,idxfp);
		//printf("%s %s\n",id,key);
		//printf("%d %d\n",strlen(key),strlen(id));
		unsigned int a = atoi(id); unsigned int b = atoi(key);
		//printf("%d %d\n",a,b);
		//printf("ã�°� = %d �߰��� = %d\n",a,b);
		if( a==b ){ // ã�� ���
			fread((void *)s,sizeof(s),1,idxfp);
			*pageNum = *(int *)s;
			fread((void *)s,sizeof(s),1,idxfp);
			*recordNum = *(int *)s;
			if(id[0] == '0'){ *pageNum = -1; *recordNum = -1;}
			break;
		}else if( a>b ){
			left = mid+1;
		}else{
			right = mid-1;
		}
		
	}
	
}

int main(int argc, char *argv[])
{
	FILE *fp;  // ���ڵ� ������ ���� ������
	fp = fopen(argv[2],"r+");
	char spare[16];
	char s[4];
	int i,j;
	int size=0;
	Person person;

	if(fp == NULL){ // ���ڵ� ������ ���� ���
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
	//fp2 = fopen(argv[3],"w");
	//printf("%d %d %d %d\n",allPage,allRecord,deletedPageNum,deletedRecordNum);
	//return ;
	if(strcmp(argv[1],"a")==0){ // ���ڵ� �߰�
		strcpy(person.id,argv[3]);
		strcpy(person.name,argv[4]);
		strcpy(person.age,argv[5]);
		strcpy(person.addr,argv[6]);
		strcpy(person.phone,argv[7]);
		strcpy(person.email,argv[8]);
		add(fp,&person);
	}else if(strcmp(argv[1],"d")==0){
		delete(fp,argv[3]);
	}else if(strcmp(argv[1],"i")==0){
		size = 0;
		FILE *fp2 = fopen(argv[3],"w");
		memcpy(s,(char *)&size,sizeof(int));
		fwrite((void *)s,sizeof(s),1,fp2);
		createIndex(fp2,fp);
		fclose(fp2);
	}else if(strcmp(argv[1],"b")==0){
		FILE *fp2 = fopen(argv[3],"r");
		int pageNumber=-1,recordNumber=-1;
		read=0;
		binarysearch(fp2,argv[4],&pageNumber,&recordNumber);	
		if(pageNumber == -1 && recordNumber == -1) printf("#reads:%d\nno persons\n",read);
		else{
			printf("#reads:%d\n",read);
			//printf("%d %d\n",pageNumber,recordNumber);
			char buf[PAGE_SIZE];
			readPage(fp,buf,pageNumber); // �ش�Ǵ� �������� ������ �´�.
			int offset = 4+8*recordNumber;
			int recordLength = offset+4;
			for(i=0;i<4;i++) s[i] = buf[offset++];
			offset=*(int *)s; offset+=HEADER_AREA_SIZE;
			for(i=0;i<4;i++) s[i] = buf[recordLength++];
			recordLength = *(int *)s; char realRecord[recordLength+1];
			j=0;
			for(i=offset;i<offset+recordLength;i++) realRecord[j++] = buf[i];
			realRecord[j] = '\0';
			unpack(realRecord,&person);
			printf("id=%s\n",person.id);
			printf("name=%s\n",person.name);
			printf("age=%s\n",person.age);
			printf("addr=%s\n",person.addr);
			printf("phone=%s\n",person.phone);
			printf("email=%s\n",person.email);	
		}  
		fclose(fp2);
	}
	fclose(fp);
	return 1;
}