#ifndef _TYPE_
#define _TYPE_

//kind
#define VAR 1
#define PROC 2
#define CONST 3
#define FUNCT 4
#define PARA  5
#define POINTER 6

//type
#define INT 1
#define CHAR 2
#define STRING 3



typedef struct symbol
{
	char name[20];
	int kind;
	int type;
	int value;
	char str[100];
	int level;
	int offset;
	int size;
};

typedef struct symbolTree{
	symbolTree * parent;
	char name[20];
	int level;
	int symCount;
	int sonCount;
	symbol symBol[100];
	symbolTree  * son[10];
};

extern symbolTree * symTree;
extern symbolTree * symTreeHead;
void initSymTree();
void insertInt(char *name, int value,int kind, int type);
void insertAPF(char *name, int size,int kind);
symbol* findIDEN(char* name);
void addTYPE(int n,int type, int size);  //var定义时 插入多个变量的类型
void addParentSize(int size);  //保存para+pointer个数
void addFuncTYPE(int type);     //保存Function 的类型、
void symbacklv();
#endif