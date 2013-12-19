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
	int size;
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
char* insertStr(char *name, char* str,int kind);
symbol* findIDEN(char* name);
symbolTree* findpf(char *find);
void findAtree(char* name); //生成汇编时 符号表层次进入
void addTYPE(int n,int type, int size);  //var定义时 插入多个变量的类型
void addParentSize(int size);  //保存para+pointer个数
void addFuncTYPE(int type);     //保存Function 的类型、
void symbacklv();     //返回上一层
void countoffset();   //计算偏移  运行栈
//char* getSymString();    //获得字符串 直接处理
extern char constString[40][40];
extern int  strcounter;
extern char *strName[];
#endif



/*
		   |           |
		   -------------
		   | para[3]   |
		   -------------
		   | para[2]   |
		   -------------
		   | para[1]   |
		   -------------
		   |  abp[1]   |
		   -------------
		   |  abp[2]   |
		   -------------
		   |  abp[3]   |
		   -------------
		   | retADDR   |
		   -------------
	 EBP-> | prevEBP   |
		   -------------
		   | result    |
		   -------------
		   | otherVAR  |
		   -------------
		   | ....      |
		   -------------
	 ESP-> | ....      |
		   -------------
		   |           |
		   -------------
		   |           |
		   -------------
		   |           |

*/