#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utility>
#include <map>
#include "token.h"
#include "error.h"

WT wordtype;

char *words[] = {
			" "," ","*",".","",
			"procedure","/",":","if","function",
			"<","'","","then","read",
			"<=","\"","STRCON","else","write",
			">","(","const","do",">=",
			")","integer","while","begin","=",
			"[","end","<>","]","char",
			":=","{","var","for","+",
			";","}","array","to","-",
			",","of"};

char *wordt[] =	{
			"IDEN","DOWNTOTK","MULT","PERIOD","INTCON",
			"PROCETK","DIV","COLON","IFTK","FUNCTK",
			"LSS","QMARK","CHARCON","THENTK","READTK",
			"LEQ","DQMARK","STRCON","ELSETK","WRITETK",
			"GRE","LPARENT","CONSTTK","DOTK","GEQ",
			"RPARENT","INTTK","WHILETK","BEGINTK","EQL",
			"LBRACK","ENDTK","NEQ","RBRACK","CHARTK",
			"ASSIGN","LBRACE","VARTK","FORTK","PLUS",
			"SEMICN","RBRACE","ARRAYTK","TOTK","MINU",
			"COMMA","OFTK"};

int lnum=1,lcounter=0;

FILE *input, *output;
bool endx = false;
map<string,WT> reserved;


char cbuff;
char tbuff[80],tcount,sbuff[80];  //line buff
int num;
using namespace std;


void initMap(){
	reserved.insert(make_pair("const",CONSTTK));
	reserved.insert(make_pair("integer",INTTK));
	reserved.insert(make_pair("char",CHARTK));
	reserved.insert(make_pair("var",VARTK));
	reserved.insert(make_pair("array",ARRAYTK));
	reserved.insert(make_pair("of",OFTK));
	reserved.insert(make_pair("if",IFTK));
	reserved.insert(make_pair("then",THENTK));
	reserved.insert(make_pair("else",ELSETK));
	reserved.insert(make_pair("do",DOTK));
	reserved.insert(make_pair("while",WHILETK));
	reserved.insert(make_pair("for",FORTK));
	reserved.insert(make_pair("to",TOTK));
	reserved.insert(make_pair("downto",DOWNTOTK));
	reserved.insert(make_pair("procedure",PROCETK));
	reserved.insert(make_pair("function",FUNCTK));
	reserved.insert(make_pair("read",READTK));
	reserved.insert(make_pair("write",WRITETK));
	reserved.insert(make_pair("begin",BEGINTK));
	reserved.insert(make_pair("end",ENDTK));
}

void getch(){
	if( (cbuff =fgetc(input)) == EOF)
		endx = true;
	lcounter++;
}


bool ischar(){
	if( (cbuff >= 'a' && cbuff <= 'z' )
		||(cbuff >= 'A' && cbuff <= 'Z'))
		return true;
	else
		return false;
}
bool isDigit(){
	if(cbuff >= '0' && cbuff <= '9')
		return true;
	else
		return false;
}
int token(){
	while(true){
		if(cbuff == EOF)
			return -1;
		if(cbuff == ' ' || cbuff == '\t'){
			getch();
			continue;
		}
		if(cbuff == '\n'){
			lnum++;
			lcounter = 0;
			getch();
			continue;
		}
		break;
	}
	if(ischar()){
		tcount = 0;
		for(int i=0; i<20; i++)
			tbuff[i] = 0;
		tbuff[tcount++] = cbuff;
		getch();
		while(ischar() || isDigit()){
			tbuff[tcount++] = cbuff;
			getch();
		}
		tbuff[tcount] = '\0';
		map<string,WT>::iterator it = reserved.find(tbuff); //²éÕÒ±êÊ¾·û
		if(it != reserved.end()){
			wordtype = it->second;
			fprintf(output,"%s %s\n",wordt[wordtype],words[wordtype]);
		}else{
			wordtype = IDEN;
			fprintf(output,"IDEN %s\n",tbuff);
		}

	}
	else if(isDigit()){
		num = 0;
		while(isDigit()){
			num = num*10 + cbuff - '0';
			getch();
		}
		wordtype = INTCON;
		fprintf(output,"INTCON %d\n",num);
	}
	else if(cbuff == '"'){
		getch();
		int i=0;
		while(cbuff != '"'){
			if(cbuff == 32|| cbuff == 33 || (cbuff >= 35 && cbuff <=126))
				sbuff[i++] = cbuff;
			else 
				error("wolegequ");
			getch();
		}
		sbuff[i] = '\0';
		wordtype = STRCON;
		fprintf(output,"STRCON %s\n",sbuff);
		getch();
	}
	else if(cbuff == ':'){
		getch();
		if(cbuff == '='){
			wordtype = ASSIGN;
			fprintf(output,"ASSIGN :=\n");
			getch();
		}
		else{
			wordtype = COLON;
			fprintf(output,"COLON :\n");
		}
	}
	else if(cbuff == '>'){
		getch();
		if(cbuff == '='){
			wordtype = GEQ;
			fprintf(output,"GEQ >=\n");
			getch();
		}
		else{
			wordtype = GRE;
			fprintf(output,"GRE >\n");
		}
		
	}
	else if(cbuff == '<'){
		getch();
		if(cbuff == '='){
			wordtype = LEQ;
			fprintf(output,"GRE <=\n");
			getch();
		}
		else if(cbuff == '>'){
			wordtype = NEQ;
			fprintf(output,"GRE <>\n");
			getch();
		}
		else{
			wordtype = LSS;
			fprintf(output,"GRE <\n");
		}
		
	}
	else if(cbuff == '='){
		wordtype = EQL;
		fprintf(output,"EQL =\n");
		getch();
	}
	else if(cbuff == '\''){
		getch();

		if(ischar() ||isDigit())
			num = cbuff;
		else
			error("const char must be number or letter ");

		wordtype = CHARCON;
		fprintf(output,"CHARCON %c\n",num);
		getch();
		getch();

	}
	else{
		switch(cbuff){
		case ':': wordtype = COLON; break;
		
		case '"': wordtype = DQMARK; break;
		case '(': wordtype = LPARENT; break;
		case ')': wordtype = RPARENT; break;
		case '[': wordtype = LBRACK; break;
		case ']': wordtype = RBRACK; break;
		case '{': wordtype = LBRACE; break;
		case '}': wordtype = RBRACE; break;
		case ',': wordtype = COMMA; break;
		case '+': wordtype = PLUS; break;
		case '-': wordtype = MINU; break;
		case '*': wordtype = MULT; break;
		case '/': wordtype = DIV; break;
		case ';': wordtype = SEMICN; break;
		case '.': wordtype = PERIOD; break;
		default: error("error");
		}
		fprintf(output,"%s %s\n",wordt[wordtype],words[wordtype]);
		getch();
	}
	return 0;
}

void num2String(int x, char *p){
	int i=0;

	if(x == 0){
		p[0] = '0';
		p[1] = '\0';
	}
	else{
		while(x>0){
			p[i++] = x%10 + '0';
			x=x/10;
		}
		p[i] = '\0';
	}
	int n =strlen(p);
	for(int i=0;i<n/2; i++){
		char a = p[i];
		p[i] = p[n-i-1];
		p[n-i-1] = a;
	}
}