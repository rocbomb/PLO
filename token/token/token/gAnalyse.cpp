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
char *id;
extern FILE *input, *output;
extern map<string,WT> reserved;
extern bool endx;

int kind = 0;
int idtype = 0;
int numchar;
int size;
char idname[20];

void factor(){
	printf("Factor start!!!\n",wordtype);

	if(wordtype == IDEN){
		
		//call faction   array and id
		strcpy(idname,tbuff);
		if(findIDEN(idname) == NULL)
			error("can not found id");
		token();
		if(wordtype == LBRACK){
			token();
			expression();
			if(wordtype == RBRACK){
				;
			}
			token();
		}
		else
		if(wordtype == LPARENT){
			real_para_table();
		}
	}
	else if(wordtype == INTCON){
		;
		token();
	}
	else if(wordtype == LPARENT){
		token();
		expression();
		if(wordtype == RPARENT){
			;
		}
		token();
	}
	else
		error("factor error");
	printf("Factor end$$$ \n");

}
void term(){
	printf("	Term start\n");
	while(true){
		cout << wordtype;
		factor();
		if(wordtype == MULT){
		}
		else if(wordtype == DIV){
		}
		else
			break;
		token();
	}

	printf("	Term end\n");
	
}
void expression(){
	if(wordtype == PLUS)
		;
	else if(wordtype == MINU){
		;
	}
	while (true)
	{
		printf("	Term start\n");
		while(true){
			cout << wordtype;

			factor();

			if(wordtype == MULT){
			}
			else if(wordtype == DIV){
			}
			else
				break;
			token();
		}

		printf("	Term end\n");

		if(wordtype == PLUS)
			;
		else if(wordtype == MINU){
			;
		}else
			break;
		token();
	}
}
void condition(){
	expression();
	switch(wordtype){
	case LSS: 
		break;
	case LEQ:
		break;
	case NEQ:
		break;
	case EQL:
		break;
	case GEQ:
		break;
	case GRE:
		break;
	default:
		error("condition symbol error");
	}
	token();
	expression();
}
void real_para_table(){
	if(wordtype == LPARENT)
		;
	else
		error("real_para_table missing '('");
	
	//检查实在参数表的个数与类型
	int paracounter=0;
	strcpy(idname,tbuff);
	symbol * here;
	here = findIDEN(idname);

	while(true){
		token();
		expression();
		paracounter++;
		if(wordtype == COMMA)
			;
		else
			break;
	}
	if(paracounter != here->size){
		error("实在参数表个数错误");
	}
	if(wordtype == RPARENT)
		;
	else
		error("real_para_table missing ')'");
	token();
}
void statement(){
	cout<<"statement analyse start  "<<wordtype<<"\n";
	switch (wordtype){
	case IDEN:
		printf("	Assign stat start\n");
		token();
		if(wordtype == LPARENT){
			real_para_table();
		}
		else if(wordtype == LBRACK){
			token();
			expression();
			if(wordtype == RBRACK)
				;
			else
				error("array mising ']'");
			token();
		}
		if(wordtype == ASSIGN){
			token();
			expression();
		}
		printf("	Assign stat end\n");
		break;
	case IFTK:
		token();
		condition();
		if(wordtype == THENTK)
			;
		else
			error("if statement missing then");

		token();
		statement();
		if(wordtype == ELSETK){
			token();
			statement();
		}
		break;
	case WHILETK:
		printf("!!!!WHILETK start\n");
		token();
		condition();
		if(wordtype == DOTK){
			token();
			statement();
		}
		else
			error("while statement missing \"do\"");

		printf("$$$$$WHILETK end\n");
		break;
	case FORTK:
		token();
		if(wordtype == IDEN){
			strcpy(idname,tbuff);
			if(findIDEN(idname) == NULL)
				error("can not found id");	
		}
		else
			error("if statement error");

		token();
		if(wordtype == ASSIGN)
			;
		else
			error("if statement error");

		token();
		expression();

		if(wordtype == TOTK){
		}
		else if(wordtype == DOWNTOTK)
		{
		}
		else
			error("if statement error  missing TO or DOWNTO");

		token();
		expression();

		if(wordtype == DOTK){
			token();
			statement();
		}
		else
			error("if statement error \"do\"");

		break;
	case BEGINTK:
		printf("mult statment start\n");
		while(true){
			token();
			statement();
			if (wordtype != SEMICN)
				break;
		}
		printf("come\n");
		if(wordtype != ENDTK)
			error("lost endTK");
		else token();

		printf("mult statment end\n");
		break;
	case WRITETK:
		token();
		if(wordtype == LPARENT){
		}else
			error("missing '('");

		token();
		if(wordtype == STRCON){
			token();
			if(wordtype == COMMA){
				expression();
			}
		}
		else{
			expression();
		}
		
		if(wordtype == RPARENT){
		}else
			error("missing '('");

		token();
		break;


	case READTK:
		printf("Read stat start\n");
		token();
		if(wordtype == LPARENT)
			;
		else
			error("read statement missing '('");
		
		do{
			;
			token();
			if(wordtype == IDEN){
			strcpy(idname,tbuff);
			if(findIDEN(idname) == NULL)
				error("can not found id");		
			}
			else
				error("read statement parameter error");
			token();
		}while(wordtype == COMMA);

		if(wordtype == RPARENT)
			;
		else
			error("read statement missing ')'");

		token();
		printf("Read stat END\n");
		break;
	default:
		error("statement error");
	cout<<"statement succeed\n";
	}
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
	if(wordtype == ARRAYTK){
		token();
		size = 0;
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
		cout<<"const start\n";
		token();
		while(true){
			//tbuff 标示符名称
			kind = CONST;
			if(wordtype != IDEN )
				error("");
			strcpy(idname,tbuff);
			token();
			if(wordtype != ASSIGN)
				error(" lost assign ");
			if(wordtype == EQL){
				error(" = error  auto change to ：= ");
				wordtype = ASSIGN;
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
				error("");
		}
		cout<<"const end\n";
	}
	if(wordtype == VARTK){
		cout<<"var start\n";
		token();
		int varcounter=0;
		while(true){
			varcounter++;  // '：' 前记录几个变量
			if(wordtype != IDEN)
				error("var def id error");
			strcpy(idname,tbuff);
			token();
			insertInt(idname,0,VAR,INT); //先默认为0 整型
			if(wordtype == COMMA){
				token();
				continue;
			}
			
			if(wordtype == COLON){
				token();
			}else
				error("var define lost : ");
			
			type();
			if(wordtype != SEMICN)
				error("");

			addTYPE(varcounter,idtype,size);
			varcounter = 0;
			token();
			if(wordtype == IDEN)
				continue;
			else{
				break;
			}
		}
		cout<<"var end\n";
	}
	if(wordtype == PROCETK || wordtype == FUNCTK){
		while(true){
			if(wordtype == PROCETK){
				token();
				//设置类型 proc
				kind = PROC;
				
				if(wordtype != IDEN)
					error("procedure id lost");
				//复制过程名
				strcpy(idname,tbuff);
				insertAPF(idname,0,kind);
				token();
				if(wordtype == LPARENT)
					parameter_table();
				else if(wordtype == SEMICN)
					;
				else error("procedure");
				if(wordtype != SEMICN){
					error("lost ; \n");
				}
				token();
				block();
				if(wordtype != SEMICN)
					error("");
				printf("procedure Stat Succeed\n");

				symbacklv(); //符号表返回上一层
			}
			else if(wordtype == FUNCTK){
				token();
				kind = FUNCT;
				if(wordtype != IDEN)
					error("procedure id lost");
				strcpy(idname,tbuff);

				insertAPF(idname,0,kind);  //添加function 的符号表
				token();
				if(wordtype == LPARENT)
					parameter_table();
				else if(wordtype == COLON)
					;
				else error("procedure");
				
				//function的类型添加
				token();
				basic_type();
				addFuncTYPE(idtype);
				
				
				if(wordtype != SEMICN)
					error("FUNCTK error");
				token();
				block();
				if(wordtype != SEMICN)
					error("lost ; error");
				symbacklv(); //符号表返回上一层
			}

			token();
			if(wordtype == PROCETK || wordtype == FUNCTK)
				continue;
			break;
		}
	}

	if(wordtype == BEGINTK){
		printf("mult statment start\n");
		
		while(true){
			token();
			statement();
			if (wordtype != SEMICN)
				break;
		}
		if(wordtype != ENDTK)
			error("lost endTK");
		else token();

		printf("mult statment end\n");
	}
	printf("block Succeed\n");

}
void program(){
	token();
	block();
	if(wordtype != PERIOD)
		error("lost a '.'\n");
	printf("\n#########gAnalyse Succeed#########\n");
}