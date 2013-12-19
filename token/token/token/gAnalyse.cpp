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

using namespace std;

//extern WT wordtype;
extern char *words[];
extern char *wordt[];

extern int lnum,lcounter;
char *id;
extern FILE *input, *output;
extern map<string,WT> reserved;
extern bool endx;



int kind = 0;
int idtype = 0;
int numchar;
int size;
char idname[20];

char one[20];
char two[20];
void real_para_table(symbolTree * hereTree);
void expression(char *end);

void findFENHAO(){
	while(wordtype != SEMICN && wordtype != ENDTK && wordtype != ELSETK)
		token();
}

void factor(char *end){
	//printf("Factor start!!!\n");
	char opa[20];
	char geshu[20];
	char herename[20];
	if(wordtype == IDEN){
		
		//call faction   array and id
		strcpy(herename,tbuff);
		symbol *p = findIDEN(herename);
		
		if(p->kind != PROC && p->kind != FUNCT)
		{
			strcpy(end,tbuff);
			if(findIDEN(herename) == NULL)
				error("can not found id");
			token();

			//数组
			if(wordtype == LBRACK){
				token();
				strcpy(end,getTemp());
				
				expression(geshu);
				if(wordtype == RBRACK){
					;
				}
				token();
				gen(LW,herename,geshu,end);
			}

		}else{ //函数
			num2String(p->size,opa); //参数个数
			if(p->kind == FUNCT){
				//返回值
				strcpy(end,getTemp());
				
			}else{
				end[0] = '\0';
			}
			symbolTree* hereTree;
			hereTree = findpf(herename);
			token();
			if(wordtype == LPARENT)
				real_para_table(hereTree);
			gen(CALL,herename,opa,end);
		}
	}
	else if(wordtype == INTCON){

		//转成字符串
		num2String(num,end);
		toConst(end);
		token();
	}
	else if(wordtype == LPARENT){
		token();
		expression(end);
		if(wordtype == RPARENT){
			;
		}
		token();
	}
	else
		error("factor error");


}
void term(char *end){


	instr op = ADD;
	char opa[20];
	char opb[20];
	factor(opa);

	char *tem  = NULL;
	//strcpy(end,opa);
	while(true){
		if(wordtype == MULT){
			op = MUL;
		}
		else if(wordtype == DIV){
			op = DIVstr;
		}
		else{
			strcpy(end,opa);
			break;
		}

		token();
		factor(opb);

		if(tem == NULL)
			tem = getTemp();
		gen(op,opa,opb,tem);
		strcpy(opa,tem);
	}

	
}
void expression(char *end){

	int minus=0;
	char opa[20];
	char opb[20];

	
	if(wordtype == PLUS)
		token();
	else if(wordtype == MINU){
		minus=1;
		token();
	}

	char *tem  = NULL;
	instr op = NOP;
	term(opa);

	if(minus == 1 && opa[0] != '#' )
		gen(SUB,"#0",opa,opa);
	else if(minus == 1)
		num2Neg(opa);

	while (true)
	{
		if(wordtype == PLUS)
			op = ADD;
		else if(wordtype == MINU){
			op = SUB;
		}else{
			strcpy(end,opa);
			break;
		}
		token();
		term(opb);


		if(tem == NULL)
			tem = getTemp();
		gen(op,opa,opb,tem);
		strcpy(opa,tem);
	}

	


	//return expz;
}
void condition(char *judge){
	char opa[20]="0";
	char opb[20];
	expression(opa);
	instr op;
	switch(wordtype){
	case LSS:      //小于
		op = SST;
		break;
	case LEQ:     //小于等于
		op = SEST;
		break;
	case NEQ:     //不等于
		op = NEST;
		break;
	case EQL:     //等于
		op = EQST;
		break;
	case GEQ:     //大于等于
		op = LEST;
		break;
	case GRE:   //大于
		op = LST;
		break;
	default:
		error("condition symbol error");
	}

	token();
	expression(opb);
//	strcpy(judge,getTemp());
	gen(op,opa,opb,judge);

}
void real_para_table(symbolTree* hereTree){
	char parass[20];
	if(wordtype == LPARENT)
		;
	else
		error("real_para_table missing '('");
	
	//检查实在参数表的个数与类型
	int paracounter=0;
	strcpy(parass,tbuff);
	symbol * here,*p;
	here = findIDEN(parass);

	char para[20][20];
	while(true){
		token();
		if(hereTree->symBol[paracounter].kind == POINTER)
		{
			int arrayFlag=0;
			char tar[20];
			strcpy(idname,tbuff);
			strcpy(tar,tbuff);
			p = findIDEN(idname);
			token();
			char assignhere[20];

			char offset[20];
			//if(arrayFlag == 0)

			if(p->kind == FUNCT){
				para[paracounter][0] = '@';
				para[paracounter][1] = '\0';
				strcat(para[paracounter],tar);
			}
			else {
				if(wordtype == LBRACK){
					arrayFlag = 1;
					token();
					expression(offset);
					if(wordtype == RBRACK)
						;
					token();
				}

				if(arrayFlag == 1){
					strcpy(assignhere,getTemp()); //临时变量
					gen(ADDR,tar,offset,assignhere); //
					strcpy(para[paracounter],assignhere);
				}
				else{
					para[paracounter][0] = '@';
					para[paracounter][1] = '\0';
					strcat(para[paracounter],tar);
				}
			}
		}
		else
			expression(para[paracounter]);
//		gen(PUSH,para[paracounter],"","");
		paracounter++;
		if(wordtype == COMMA)
			;
		else
			break;
	}
	if(paracounter != here->size){
		error("实在参数表个数错误");
	}
	//反向压入栈
	while(paracounter--){
		gen(PUSH,para[paracounter],"","");
	}
	if(wordtype == RPARENT)
		;
	else
		error("real_para_table missing ')'");
	token();
}
void statement(){
	int arrayFlag = 0;
	symbol *p;

	char one[20];
	char two[20];
	char three[20];

	switch (wordtype){
	case IDEN:

		char tar[20];

		strcpy(idname,tbuff);
		strcpy(tar,tbuff);
		p = findIDEN(idname);
		
		token();
		char assignhere[20];

		char offset[20];
		//if(arrayFlag == 0)

		if(p->kind == PROC){
			//寻找PROC的树
			symbolTree* hereTree;
			hereTree = findpf(idname);
			if(wordtype == LPARENT)
				real_para_table(hereTree);
			num2String(p->size,offset);
			gen(CALL,tar,offset,"");
		}
		else {
			if(wordtype == LBRACK){

				arrayFlag = 1;
				token();
				expression(offset);

				if(wordtype == RBRACK)
					;
				else{
					error("array mising ']'");
					findFENHAO();
					goto statend;
				}
				token();
			}
			if(wordtype == ASSIGN || wordtype == EQL){
				if(wordtype == EQL){
					error(" = 自动补齐为 ：= ");
				}
				token();
				expression(assignhere);

				if(wordtype != SEMICN && wordtype != ENDTK && wordtype != ELSETK){
					error("赋值语句错误");
					findFENHAO();
				}
			}
			if(arrayFlag == 1)
				gen(SW ,tar,offset,assignhere);
			else
				gen(ASSIGNstr,assignhere,"",tar);

		}

		break;
	case IFTK:
		token();
		char one[20];
		

		char *end1;
		char *end2;
		end1 = getlabel();
		condition(end1);
//		gen(BEQ,one,"#0",end1);

		if(wordtype == THENTK)
			;
		else{
			error("if statement missing then");
			findFENHAO();
			goto statend;
		}
		token();
		statement();
		if(wordtype == ELSETK){
			token();
			end2 = getlabel();
			gen(JMP,"","",end2);
			setLabel(end1);
			
			statement();

			setLabel(end2);
			
		}else
			setLabel(end1);
		break;
	case WHILETK:
		char *startwhile;
		char *endwhile;

		startwhile = getlabel();
		endwhile = getlabel();

		setLabel(startwhile);


		token();
		

		char judge[20];
		condition(endwhile);
		
//		gen(BEQ,judge,"#0",endwhile);

		if(wordtype == DOTK){
			token();
			statement();
		}
		else{
			error("while statement missing \"do\"");
			findFENHAO();
			goto statend;
		}
		gen(JMP,"","",startwhile);
		setLabel(endwhile);

		break;
	case FORTK:
		token();
		char *forstart;
		char *forend;
		char *forloop ;
		char *forcount;
		char stari[20];
		char starA[20];
		char endA[20];
		forstart = getlabel();
		forend = getlabel();
		forloop = getlabel();
		forcount = getlabel();

		instr loopop;
		if(wordtype == IDEN){
			strcpy(starA,tbuff);

			if(findIDEN(starA) == NULL)
				error("can not found id");	
		}
		else{
			error("if statement error");
			findFENHAO();
			goto statend;
		}
		token();
		if(wordtype == ASSIGN)
			;
		else{
			error("if statement error");
			findFENHAO();
			goto statend;
		}
		token();
		expression(stari);
		//for i=0 赋值
		gen(ASSIGNstr,stari,"",starA);


		if(wordtype == TOTK){
			loopop = ADD;
		}
		else if(wordtype == DOWNTOTK)
		{
			loopop = SUB;
		}
		else
			error("if statement error  missing TO or DOWNTO");

		//判断部分
		setLabel(forloop);

		token();
		expression(endA);

		char *fortemp;
		fortemp = getTemp();
		if(loopop == ADD)
			gen(SEST,starA,endA,forend);
		else
			gen(LEST,starA,endA,forend);
		//gen(BEQ,fortemp,"#0",forend);
		gen(JMP,"","",forstart);

		//add ++ OR --
		setLabel(forcount);
		gen(loopop,starA,"#1",starA);
		gen(JMP,"","",forloop);

		if(wordtype == DOTK){
			

			//statement start
			setLabel(forstart);
			token();
			statement();

			//for stat end  跳转
			gen(JMP,"","",forcount);
			setLabel(forend);
		}
		else{
			error("if statement error \"do\"");
			findFENHAO();
			goto statend;
		}
		break;
	case BEGINTK:

		while(true){
			token();
			statement();
			if (wordtype != SEMICN)
				break;
		}

		if(wordtype != ENDTK)
			error("lost endTK");
		else token();

		break;
	case WRITETK:
		token();
		if(wordtype == LPARENT){
		}else{
			error("missing '('");
			findFENHAO();
			goto statend;
		}
		token();
		char writenum[20];
		char *strname;
		if(wordtype == STRCON){

			token();
			strname = insertStr("string",sbuff,STRCON);
			gen(WRITE,"","",strname);
			if(wordtype == COMMA){
				expression(writenum);

				//write("sss",a);

				gen(WRITE,"","",writenum);
			}
		}
		else{
			expression(writenum);
			gen(WRITE,"","",writenum);
		}
		
		if(wordtype == RPARENT){
		}else{
			error("missing '('");
			findFENHAO();
			goto statend;
		}
		token();
		break;


	case READTK:

		token();
		if(wordtype == LPARENT)
			;
		else{
			error("read statement missing '('");
			findFENHAO();
			goto statend;
		}
		do{
			;
			token();
			if(wordtype == IDEN){
			strcpy(idname,tbuff);
			if(findIDEN(idname) == NULL)
				error("can not found id");		
			}
			else{
				error("read statement parameter error");
				findFENHAO();
				goto statend;
			}
			token();

			gen(READ,"","",idname);

		}while(wordtype == COMMA);

		if(wordtype == RPARENT)
			;
		else{
			error("read statement missing ')'");
			findFENHAO();
			goto statend;
		}
		token();

		break;
	case SEMICN:
		break;
	case ENDTK:
		break;
	default:{
		error("statement error");
		findFENHAO();
		}
	}
	statend:
	;
}
void basic_type(){
	if(wordtype == INTTK)
		idtype = INT;
	else if(wordtype == CHARTK)
		idtype = CHAR;
	else
		error("basic type input error: must be 'int' or 'char' ");
	token();
}
void parameter_table(){
	if(wordtype == LPARENT)
		;
	else
		error("parameter_table missing '('");
	int paracounter = 0;
	token();
	while(true){
		int counter=0;
		
		if(wordtype == VARTK)
			{kind = POINTER;
			token();
		}
		else
			kind = PARA;
		while(true){
			counter++;
			paracounter++;
			if(wordtype == IDEN){
				strcpy(idname,tbuff);
				//储存形式参数表
				insertInt(idname,0,kind,INT);
			}
			else
				error("");

			token();
			if(wordtype == COMMA)
				token();
			else
				break;
		}
		
		if(wordtype == COLON){
		}
		else
			error("colon error missing \n");
		token();
		basic_type();

		addTYPE(counter,idtype,0);
		if(wordtype == SEMICN)
			token();
		else
			break;
	}
	addParentSize(paracounter);  //proc 或者 func 定义完毕 设置参数个数

	if(wordtype == RPARENT)
		;
	else
		error("parameter_table missing ')'");
	token();


}
void type(){
	size = 0;
	if(wordtype == ARRAYTK){
		token();
		
		if(wordtype == LBRACK){
			token();
			if(wordtype == INTCON)
				size = num;//数组大小
			else
				error("array def error:array lenth");
		}
		else
			error("array def missing '['");
		token();
		if(wordtype == RBRACK){
			token();
		}
		else
			error("array missing ']'");
	}
	if(wordtype == OFTK){
			token();
	}
	basic_type();
	//exit:
}
void constX(){
	
	switch (wordtype){
	case INTCON:
		idtype = INT;
		break;
	case PLUS:
		token();
		if(wordtype == INTCON){
			idtype = INT;
			numchar = num;
		}
		else
			error("'+' 后面应该有数字");
		break;
	case MINU:
		token();
		if(wordtype == INTCON){
			idtype = INT;
			num = -1*num;
			numchar = num;
		}
		else
			error("'-' 后面应该有数字");
		break;
	case CHARCON:
		idtype = CHAR;
		numchar = num;
		break;
	default:
		error("const 参数错误");
	}
	token();
}
void block(){
	cout<<"block analyse start\n";
	if(wordtype == CONSTTK){

		token();
		while(true){
			//tbuff 标示符名称
			kind = CONST;
			if(wordtype != IDEN )
				error("");
			strcpy(idname,tbuff);
			token();
			if(wordtype != EQL){
				error(" = error ");
				wordtype = EQL;
			}

			token();
			constX();
			//获得了type
			insertInt(idname,numchar,kind,idtype);
			if(wordtype == COMMA){
				token();
				continue;
			}
			else if(wordtype == SEMICN){
				token();
				break;
			}else
				error("CONST缺失分号，自动补齐");
		}
constend:
		;

	}
	if(wordtype == VARTK){

		token();
		int varcounter=0;
		while(true){
			varcounter++;  // '：' 前记录几个变量
			if(wordtype != IDEN){
				error("var def id error");
				findFENHAO();
				goto varend;
			}
			strcpy(idname,tbuff);
			token();
			insertInt(idname,0,VAR,INT); //先默认为0 整型
			if(wordtype == COMMA){
				token();
				continue;
			}
			
			if(wordtype == COLON){
				token();
			}else{
				error("var define lost : ");
				findFENHAO();
				goto varend;
			}
			type();
			if(wordtype != SEMICN)
				error("VAR 丢失分号，自动添加");

			addTYPE(varcounter,idtype,size);
			varcounter = 0;
			token();
			if(wordtype == IDEN)
				continue;
			else{
				break;
			}
		}
varend:
		;

	}
	if(wordtype == PROCETK || wordtype == FUNCTK){
		while(true){
			if(wordtype == PROCETK){
				token();
				//设置类型 proc
				kind = PROC;
				
				if(wordtype != IDEN)
					error("procedure id error");
				//复制过程名
				strcpy(idname,tbuff);


				/**
				*设置标签  过程名 + _label
				*/
				char proclabel[50];
				char procname[50];
				strcpy(procname,tbuff);
				strcpy(proclabel,"[st]");
				strcat(proclabel,procname);
//				strcat(proclabel,"_label");
				setLabel(proclabel);

				insertAPF(idname,0,kind);
				token();
				if(wordtype == LPARENT)
					parameter_table();
				else if(wordtype == SEMICN)
					addParentSize(0);
				else {
					error("procedure");
					findFENHAO();
				}
				if(wordtype != SEMICN){
					error("lost ; \n");
				}
				token();
				block();
				if(wordtype != SEMICN)
					error("");
				//proc 和 function 结束
				strcpy(proclabel,"[ed]");
				strcat(proclabel,procname);
				setLabel(proclabel);

				symbacklv(); //符号表返回上一层
			}
			else if(wordtype == FUNCTK){
				token();
				kind = FUNCT;
				if(wordtype != IDEN)
					error("procedure id lost");
				strcpy(idname,tbuff);

				char funclabel[50];
				char funcname[50];
				strcpy(funcname,tbuff);
				strcpy(funclabel,"[st]");
				strcat(funclabel,funcname);
//				strcat(funclabel,"_label");
				setLabel(funclabel);

				insertAPF(idname,0,kind);  //添加function 的符号表
				token();
				if(wordtype == LPARENT)
					parameter_table();
				else if(wordtype == COLON)
					addParentSize(0);
				else error("function");
				
				//function的类型添加
				token();
				basic_type();
				addFuncTYPE(idtype);
				
				
				if(wordtype != SEMICN)
					error("FUNCTK error");
				token();
				block();
				if(wordtype != SEMICN){
					error("lost ; error");
				}

				//proc 和 function 结束
				strcpy(funclabel,"[ed]");
				strcat(funclabel,funcname);
				setLabel(funclabel);

				symbacklv(); //符号表返回上一层
			}

			token();
			if(wordtype == PROCETK || wordtype == FUNCTK)
				continue;
			break;
		}
	}

	//代码段真正开始
	strcpy(idname,"[bg]");
	strcat(idname,symTree->name);
	setLabel(idname);
	
	if(wordtype == BEGINTK){

		
		while(true){
			token();
			statement();
			if (wordtype != SEMICN)
				break;
		}
		if(wordtype != ENDTK)
			error("lost endTK");
		else token();
	}


	printf("block Succeed\n");
}
void program(){
	token();
	block();
	if(wordtype != PERIOD)
		error("lost a '.'\n");
	setLabel("end_main");
	printf("\n#########gAnalyse Succeed#########\n");
}