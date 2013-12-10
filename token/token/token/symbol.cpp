#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

symbolTree * symTree;
symbolTree * symTreeHead;
symbolTree* next;
void initSymTree(){
	printf("init SymbolTree\n");
	symTree = (struct symbolTree*)malloc(sizeof(struct symbolTree));
	symTreeHead = symTree;
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
	symTree->symBol[i].level = symTree->level;
	//表项计数
	symTree->symCount++;
}

void insertStr(char *name, char* str,int kind){
	int i=symTree->symCount;
	strcpy(symTree->symBol[i].name,name);
	symTree->symBol[i].kind = kind;
	strcpy(symTree->symBol[i].str,str);
	symTree->symBol[i].level = symTree->level;
	symTree->symBol[i].size = 0;
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
	next->parent = symTree;       //父节点
	next->level = symTree->level +1;  	//层数加1
	
	int j=symTree->sonCount;

	symTree->son[j] = next;  //加入树中
	symTree = next;        //当前节点改变
	
}

symbolTree* findAtree(symbolTree *find, char* name){
	for(int i=0; i<  find->symCount ; i++ ){
		if(strcmp(find->symBol[i].name,name)==0)
			return find;
	}
	symbolTree* newfind = NULL;
	for(int i=0; i< find->sonCount; i++)
		newfind =findAtree(find->son[i],name);

	return newfind;
}
symbol* findPtree(symbolTree *find, char* name){
	for(int i=0; i<  find->symCount ; i++ ){
		if(strcmp(find->symBol[i].name,name)==0)
			return &(find->symBol[i]);
	}
//	symbol* here;
//	if(find->parent != NULL)
//		here=findPtree(find->parent,name);
//	else
//		return NULL;
	return NULL;
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