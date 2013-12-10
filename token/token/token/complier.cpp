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
using namespace std;

//extern WT wordtype;
extern char *words[];
extern char *wordt[];
extern int lnum,lcounter;
extern FILE *input, *output;
extern map<string,WT> reserved;
extern bool endx;

int main(int argc,char *argv[]){
	if(argc == 2){
		if((input = fopen(argv[1],"r"))==NULL)
			cout<<"File doesn't exist"<<endl;
	}
	else if((input = fopen("input.pas","r"))==NULL)
			cout<<"File doesn't exist"<<endl;
	output = fopen("11061046_token.txt","w");


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
	return 0;
}