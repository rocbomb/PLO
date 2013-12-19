#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

symbolTree * symTree;
symbolTree * symTreeHead;
symbolTree * next;

char constString[40][40] = {
	"=str0",
	"=str1",
	"=str2",
	"=str3",
	"=str4",
	"=str5",
	"=str6",
	"=str7",
	"=str8",
	"=str9",
	"=str10",
	"=str11",
	"=str12",
	"=str13",
	"=str14",
	"=str15",
	"=str16",
	"=str17",
	"=str18",
	"=str19",
};
char *strName[] = {
	"@str0",
	"@str1",
	"@str2",
	"@str3",
	"@str4",
	"@str5",
	"@str6",
	"@str7",
	"@str8",
	"@str9",
	"@str10",
	"@str11",
	"@str12",
	"@str13",
	"@str14",
	"@str15",
	"@str16",
	"@str17",
	"@str18",
	"@str19",
};
int  strcounter = 0;

char* insertstring(char *p){
	strcpy(constString[strcounter] , p);
	return strName[strcounter++];
}
char* insertStr(char *name, char* str,int kind){
//	int i=symTree->symCount;
//	strcpy(symTree->symBol[i].name,name);
//	symTree->symBol[i].kind = kind;
//	strcpy(symTree->symBol[i].str,str);
//	symTree->symBol[i].level = symTree->level;
//	symTree->symBol[i].size = 0;
	//表项计数
//	symTree->symCount++;
	return insertstring(str);
}

/*
char* getSymString(){
	if(--strcounter >= 0){
		return constString[strcounter];
	}else
		return NULL;
}
*/

void initSymTree(){
	printf("init SymbolTree\n");
	symTree = (struct symbolTree*)malloc(sizeof(struct symbolTree));
	symTreeHead = symTree;
	strcpy(symTree->name,"main");
	symTree->sonCount = 0;
	symTree->symCount = 0;
	symTree->level = 0;
	symTree->parent = NULL;
}

void insertInt(char *name, int value,int kind , int type){
	int i=symTree->symCount;
	strcpy(symTree->symBol[i].name,name);
	symTree->symBol[i].kind = kind;
	symTree->symBol[i].type = type;
	symTree->symBol[i].value = value;
	symTree->symBol[i].size = 0;
	symTree->symBol[i].level = symTree->level;
	//表项计数
	symTree->symCount++;
}



void addTYPE(int n,int type, int size){
	for(int i= symTree->symCount-1; i>symTree->symCount-n-1; i--){
		symTree->symBol[i].type = type;
		symTree->symBol[i].size = size;
	}
}


void insertAPF(char *name, int size,int kind){
	int i=symTree->symCount;
	strcpy(symTree->symBol[i].name,name);
	symTree->symBol[i].kind = kind;
	symTree->symBol[i].size = size;
	symTree->symBol[i].level = symTree->level;
	//表项计数
	symTree->symCount++;

	//新建一个子节点
	next = (struct symbolTree*)malloc(sizeof(struct symbolTree));
	strcpy(next->name,name);
	next->sonCount = 0;
	next->symCount = 0;
	next->size     = size;
	next->parent = symTree;       //父节点
	next->level = symTree->level +1;  	//层数加1
	
	int j=symTree->sonCount;
	symTree->sonCount++;
	symTree->son[j] = next;  //加入树中
	symTree = next;        //当前节点改变

}

void findAtree(char* name){
	for(int i=0; i< symTree->sonCount; i++){
		if(strcmp(name,symTree->son[i]->name) == 0){
			symTree = symTree->son[i];
			break;
		}
	}
}


symbol* findPtree(symbolTree *find, char* name){
	for(int i=0; i<  find->symCount ; i++ ){
		if(strcmp(find->symBol[i].name,name)==0)
			return &(find->symBol[i]);
	}
	symbol* here = NULL;
	if(find->parent != NULL)
		here=findPtree(find->parent,name);
	return here;
}



symbol* findIDEN(char* name){
	symbol * find = NULL;
	find = findPtree(symTree,name);
	return find;
}





void addParentSize(int size){
	int i = symTree->parent->symCount;
	//保存proc的参数个数
	symTree->parent->symBol[i-1].size = size;
	symTree->size = size;
}
void addFuncTYPE(int type){
	int i = symTree->parent->symCount;
	//保存proc的参数个数
	symTree->parent->symBol[i-1].type = type;
}


//返回上一层树
void symbacklv(){
	symTree = symTree->parent;
}

//计算offset
void sonoffset(symbolTree *here){
	int offset = (here->level+1)*(4);
	for(int i=0; i<here->size; i++){
		offset += 4;
		here->symBol[i].offset = offset;
	}

	//变量部分
	offset = 4;
	//4 的位置是 返回值
	for(int i=here->size; i<here->symCount; i++){
		if(here->symBol[i].size > 0){
			offset += here->symBol[i].size*4;
			here->symBol[i].offset = offset;
		}else{
			offset += 4;
			here->symBol[i].offset = offset;
		}
	}
	for(int i=0; i<here->sonCount; i++){
		sonoffset(here->son[i]);
	}
}

void countoffset(){
	int offset = 4;
	for(int i=0; i<symTreeHead->symCount; i++){
		if(symTreeHead->symBol[i].size > 0){
			offset += symTreeHead->symBol[i].size*4;
			symTreeHead->symBol[i].offset = offset;
		}else{
			offset += 4;
			symTreeHead->symBol[i].offset = offset;
		}
	}
	for(int i=0; i<symTreeHead->sonCount; i++){
		sonoffset(symTreeHead->son[i]);
	}
}
symbolTree *findpfP(char *find, symbolTree *here){
	for(int i=0; i< here->sonCount; i++ )
		if(strcmp(here->son[i]->name,find) == 0)
			return here->son[i];
	if(here->parent != NULL)
		here = findpfP(find,here->parent);
	else
		return NULL;
	return here;
}

symbolTree* findpf(char *find)
{
	for(int i=0; i< symTree->sonCount; i++ )
		if(strcmp(symTree->son[i]->name,find) == 0)
			return symTree->son[i];
	symbolTree *here = NULL;
	if(symTree->parent != NULL)
		here = findpfP(find,symTree->parent);
	return here;
}