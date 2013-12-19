#ifndef _QUATE_
#define _QUATE_
#include <stdio.h>
enum instr
{
	LABEL,
	ADD,	//rd, rs, rt   //rt = rd+rs
	SUB,	//rd, rs, rt   //rt = rd-rs
	MUL,	//rd, rs, rt   //rt = rd*rs
	DIVstr,	//	rd, rs, rt   //rt = rd/rs
	NEG,	    //Src1,Des	
	ASSIGNstr,  //Src1,Des	  赋值Des=Src
	LST,	//	rd, rs, rt   //rd > rs 则 rt=1  否则 rt=0
	LEST,   // 大于等于 置位
	EQST,	//rd, rs, rt   //相等置1 否则置0
	SST,   //小于置位
	SEST,  //小于等于置位
	NEST,  //不等于置位
	BEQ, //	rd, rs, rt   //rd == rs 则跳转到rt处
	JMP, //   rd, rs, rt   //跳转到rd处  rs rt为零
	SW, //  rd, rs, rt   //保存rd 到rt+rs  rt为地址，rs为offset
	LW,  //  rd, rs, rt   //读取rt+rs地址处的内容 填入rs
	CALL,  //	rd, rs, rt   //函数调用，rd为调用函数名，rs为参数个数，rt为返回值
	RET,		//Des						函数返回，Des为返回值
	READ,  //	rd, rs, rt	 //				读入并写入rt
	WRITE, //  rd, rs, rt	 //			将rt输出
	NOP,	  //							空语句
	END,		//						函数结束标志
	PUSH,
	ADDR,
};

typedef struct Quate
{
	instr ins;
	char  target[20];
	char  one[20];
	char  two[20];
	char  p[30];
};

extern int instrCount;
extern int lableCount;
extern int tempCount;
extern Quate  QuateTable[1000];
void gen(instr ins, char *one,char* two, char* tar);
void setLabel(char *p);
char * getTemp();
char * getlabel();
void quateout(FILE *file);
void toPoint(char *p);
void toConst(char *p);
void num2Neg(char *p);
#endif