#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utility>
#include <map>
#include "token.h"
#include "error.h"
#include "gAnalyse.h"
#include "symbol.h"
#include "quaternion.h"
#include "quate2X86.h"
using namespace std;

//extern WT wordtype;
extern char *words[];
extern char *wordt[];
extern int lnum,lcounter;
extern FILE *input, *output;
extern map<string,WT> reserved;
extern bool endx;

int main(int argc,char *argv[]){
	char filename[100];
	printf("请输入文件名：");
	scanf("%s",filename);

	if((input = fopen("qsort.pas","r"))==NULL)
			cout<<"File doesn't exist"<<endl;

	output = fopen("11061046_token.txt","w");

	FILE *outSIYUANSI;
	outSIYUANSI = fopen("quaternion.txt","w");
	FILE *outASM;
	outASM = fopen("out.asm","w");
//	lnum = 1;
//	lcounter = 0;
//	tcount = 0;
	initMap();
	initSymTree();
	printf("start\n");
	int i=1;
	getch();
//	while(!endx){
//		fprintf(output,"%d ",i++);
//		token();
//		printf("%d %s %d \n",i,words[wordtype],wordtype);
//	}
	program();

	quateout(outSIYUANSI);
	countoffset();
	quate2x86(outASM);
	return 0;
}