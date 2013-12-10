#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utility>
#include <map>

using namespace std;

enum WT{	IDEN,DOWNTOTK,MULT,PERIOD,INTCON,
			PROCETK,DIV,COLON,IFTK,FUNCTK,
			LSS,QMARK,CHARCON,THENTK,READTK,
			LEQ,DQMARK,STRCON,ELSETK,WRITETK,
			GRE,LPARENT,CONSTTK,DOTK,GEQ,
			RPARENT,INTTK,WHILETK,BEGINTK,EQL,
			LBRACK,ENDTK,NEQ,RBRACK,CHARTK,
			ASSIGN,LBRACE,VARTK,FORTK,PLUS,
			SEMICN,RBRACE,ARRAYTK,TOTK,MINU,
			COMMA,OFTK} wordtype;
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
char cbuff;
char tbuff[80],tcount,sbuff[80];  //line buff
int lnum,lcounter;
int num,errornum;
char *id;
FILE *input, *output;
map<string,WT> reserved;
bool endx = false;

void getch(){
	if( (cbuff =fgetc(input)) == EOF)
		endx = true;
	lcounter++;
}

void error(){
	errornum++;
	cout<< "line " << lnum << "letter "<<errornum << "error";
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
		int i,j;
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
				error();
			getch();
		}
		wordtype = STRCON;
		fprintf(output,"STRCON %s\n",sbuff);
		getch();
	}
	else if(cbuff == ':'){
		getch();
		if(cbuff == '='){
			wordtype = ASSIGN;
			fprintf(output,"ASSIGN :=\n");
		}
		else{
			wordtype = COLON;
			fprintf(output,"COLON :\n");
		}
		getch();
	}
	else if(cbuff == '>'){
		getch();
		if(cbuff == '='){
			wordtype = GEQ;
			fprintf(output,"GEQ >=\n");
		}
		else{
			wordtype = GRE;
			fprintf(output,"GRE >\n");
		}
		getch();
	}
	else if(cbuff == '<'){
		getch();
		if(cbuff == '='){
			wordtype = LEQ;
			fprintf(output,"GRE <=\n");
		}
		else if(cbuff == '>'){
			wordtype = NEQ;
			fprintf(output,"GRE <>\n");
		}
		else{
			wordtype = LSS;
			fprintf(output,"GRE <\n");
		}
		getch();
	}
	else if(cbuff == '='){
		wordtype = EQL;
		fprintf(output,"EQL =\n");
		getch();
	}
	else{
		switch(cbuff){
		case ':': wordtype = COLON; break;
		case '\'': wordtype = QMARK; break;
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
		default: error();
		}
		fprintf(output,"%s %s\n",wordt[wordtype],words[wordtype]);
		getch();
	}

	return 0;
}

int main(int argc,char *argv[]){
	if(argc == 2){
		if((input = fopen(argv[1],"r"))==NULL)
			cout<<"File doesn't exist"<<endl;
	}
	else if((input = fopen("input.txt","r"))==NULL)
			cout<<"File doesn't exist"<<endl;
	output = fopen("11061046_token.txt","w");


	errornum = 0;
	lnum = 1;
	lcounter = 0;
	tcount = 0;
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
	printf("start\n");
	int i=1;
	getch();
	while(!endx){
		fprintf(output,"%d ",i++);
		token();
		printf("%d %s %d \n",i,words[wordtype],wordtype);
	}
	return 0;
}