#include "quaternion.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "symbol.h"

Quate QuateTable[1000];
char *label[51]={
	"_lab1","_lab2","_lab3","_lab4","_lab5",
	"_lab6","_lab7","_lab8","_lab9","_lab10",
	"_lab11","_lab12","_lab13","_lab14","_lab15",
	"_lab16","_lab17","_lab18","_lab19","_lab20",
	"_lab21","_lab22","_lab23","_lab24","_lab25",
	"_lab26","_lab27","_lab28","_lab29","_lab30",
	"_lab31","_lab32","_lab33","_lab34","_lab35",
	"_lab36","_lab37","_lab38","_lab39","_lab40",
	"_lab41","_lab42","_lab43","_lab44","_lab45",
	"_lab46","_lab47","_lab48","_lab49","_lab50",
	"_lab51",
};
char *temp[51]={
	"_temp1","_temp2","_temp3","_temp4","_temp5",
	"_temp6","_temp7","_temp8","_temp9","_temp10",
	"_temp11","_temp12","_temp13","_temp14","_temp15",
	"_temp16","_temp17","_temp18","_temp19","_temp20",
	"_temp21","_temp22","_temp23","_temp24","_temp25",
	"_temp26","_temp27","_temp28","_temp29","_temp30",
	"_temp31","_temp32","_temp33","_temp34","_temp35",
	"_temp36","_temp37","_temp38","_temp39","_temp40",
	"_temp41","_temp42","_temp43","_temp44","_temp45",
	"_temp46","_temp47","_temp48","_temp49","_temp50",
	"_temp51",
};
char * instrx[] = 
{
	"LABEL",
	"ADD",	//rd", rs", rt   //rt = rd+rs
	"SUB",	//rd", rs", rt   //rt = rd-rs
	"MUL",	//rd", rs", rt   //rt = rd*rs
	"DIV",	//	rd", rs", rt   //rt = rd/rs
	"NEG",
	"ASSIGN",  //Src1",Des	  赋值Des=Src
	"LST",	//	rd", rs", rt   //rd > rs 则 rt=1  否则 rt=0
	"LEST",   // 大于等于 置位
	"EQST",	//rd", rs", rt   //相等置1 否则置0
	"SST",   //小于置位
	"SEST",  //小于等于置位
	"NEST",
	"BEQ", //	rd", rs", rt   //rd == rs 则跳转到rt处
	
	"JMP", //   rd", rs", rt   //跳转到rd处  rs rt为零
	"SW", //  rd", rs", rt   //保存rd 到rt+rs  rt为地址，rs为offset
	"LW",  //  rd", rs", rt   //读取rt+rs地址处的内容 填入rs
	"CALL",  //	rd", rs", rt   //函数调用，rd为调用函数名，rs为参数个数，rt为返回值
	"RET",		//Des						函数返回，Des为返回值
	"READ",  //	rd", rs", rt	 //				读入并写入rt
	"WRITE", //  rd", rs", rt	 //			将rt输出
	"NOP",	  //							空语句
	"END",		//						函数结束标志
	"PUSH",
	"ADDR",
};
int instrCount=0;
int lableCount = 0;
int tempCount = 0;
void gen(instr ins, char *one,char* two, char* tar){
	QuateTable[instrCount].ins =ins;
	strcpy(QuateTable[instrCount].one,one);
	strcpy(QuateTable[instrCount].two,two);
	strcpy(QuateTable[instrCount].target,tar);
	QuateTable[instrCount].p[0] = '\0';
	instrCount++;
}
char * getlabel(){
	return label[ lableCount++];
}
void setLabel(char *p){
	QuateTable[instrCount].ins =LABEL;
	QuateTable[instrCount].one[0] ='\0';
	QuateTable[instrCount].two[0] ='\0';
	QuateTable[instrCount].target[0] ='\0';

	strcpy(QuateTable[instrCount].p, p);
	instrCount++;
}

char * getTemp(){
	insertInt(temp[tempCount],0,VAR,INT);
	return temp[ tempCount++];
}

void quateout(FILE *file){
	for(int i=0; i<instrCount; i++)
	{
		if(QuateTable[i].ins != LABEL){
		printf("%8s,%8s,%8s,%8s\n",instrx[QuateTable[i].ins],
			QuateTable[i].one,QuateTable[i].two,QuateTable[i].target);
		fprintf(file,"%8s,%8s,%8s,%8s\n",instrx[QuateTable[i].ins],
			QuateTable[i].one,QuateTable[i].two,QuateTable[i].target);
		}
		else{
			printf("%s:\n",QuateTable[i].p);
			fprintf(file,"%s:\n",QuateTable[i].p);
		}
	}
}

void toConst(char *p){
	int n= strlen(p);
	//n++;
	while(n>=0){
		p[n+1] = p[n];
		n--;
	}
	p[0] = '#';
}

void toPoint(char *p){
	int n= strlen(p);
	//n++;
	while(n>=0){
		p[n+1] = p[n];
		n--;
	}
	p[0] = '*';
}


//负数
void num2Neg(char *p){
	int n = strlen(p);

	while(n>0){
		p[n+1] = p[n];
		n--;
	}
	p[1] = '-';
}