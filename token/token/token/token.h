#ifndef _TOKEN_H_

#define _TOKEN_H_
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
			COMMA,OFTK
};
extern WT wordtype;
extern char *words[];

extern char *wordt[];
extern int lnum,lcounter;
extern char tbuff[80];
extern char sbuff[80];
extern int num;
extern FILE *input, *output;
extern bool endx;
extern map<string,WT> reserved;
void getch();
void initMap();
int token();
void num2String(int x, char *p);
#endif
