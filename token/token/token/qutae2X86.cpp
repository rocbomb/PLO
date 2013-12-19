#include "quate2X86.h"
#include "quaternion.h"
#include "symbol.h"
#include <stdlib.h>
#include <string.h>

void genx86(struct Quate* quate,FILE* output);
void genLabel(struct Quate* quate,FILE* output);
int strcount = 0;

void initASM(FILE *output){
	fprintf(output,";-----------------\n");
	fprintf(output,";	ml.exe /c /coff 1.asm\n");
	fprintf(output,";	link.exe /SUBSYSTEM:CONSOLE /OPT:NOREF 1.obj\n");
	fprintf(output,".386\n");
	fprintf(output,".model flat, stdcall\n");
	fputc('\n',output);
	fprintf(output,"option casemap:none\n");
	fputc('\n',output);
	fprintf(output,"includelib E:\\masm32\\lib\\kernel32.lib\n");
	fprintf(output,"includelib E:\\masm32\\lib\\msvcrt.lib\n");
	fputc('\n',output);
	fprintf(output,"include E:\\masm32\\include\\msvcrt.inc\n");
	fprintf(output,"include E:\\masm32\\include\\kernel32.inc\n");
	fputc('\n',output);
	fprintf(output,".DATA\n");
	fprintf(output,"print_int    DB \"%%d\",0\n");
	fprintf(output,"print_char   DB \"%%c\",0\n");
	fprintf(output,"print_string DB \"%%s\",0\n");
	fprintf(output,"scanf_int    DB \"%%d\",0\n");
	fprintf(output,"scanf_char   DB \"%%c\",0\n");
	for(int i = 0; i < strcounter; i++){
		fprintf(output,"%s   DB \"%s\",0\n",strName[i],constString[i]);
	}
	fputc('\n',output);
	fprintf(output,"MAX@main@ DWORD 1024\n");
	fprintf(output,"MIN@main@ DWORD -1024\n");
	fprintf(output,"ZERO@main@ DWORD 0\n");
	fprintf(output,"CHAR@main@ DWORD 67\n");
	fputc('\n',output);
	fputc('\n',output);
	fprintf(output,".CODE\n");
}
int quate2x86(FILE *output){
	int instrmax = instrCount;
	int instrCounter = 0;
	struct Quate* quate;
	initASM(output);
	for(;instrCounter<instrmax; instrCounter++){
		quate = &QuateTable[instrCounter];
		genx86(quate,output);
//		printf("%s\n",quate->target);
	}

	return 0;
}

void useaddr(symbol *one, FILE *output){
		if(one->kind == VAR){  //变量
			if(one->level == symTree->level){//同层变量
				fprintf(output,"\tlea EAX,dword ptr [EBP-%d]\n",one->offset);
			}else{
				//非同层访问常量
				int off = symTree->level - one->level;
				off = off*4+4;
				fprintf(output,"\tmov EBX,dword ptr [EBP+%d]\n",off);	//先将apb移到EBX里
				fprintf(output,"\tlea EAX,dword ptr[EBX-%d]\n",one->offset);		//再根据EBX和offset得到数据,移动到EAX、
			}
		}else if(one->kind == POINTER){//指针型！！！！传地址  获取
			fprintf(output,"\tmov EAX,dword ptr[EBP+%d]  ;pointer\n",one->offset);		//指针,是地址,移动到EBX、
		}else if(one->kind == PARA){//形参,只是内容
			fprintf(output,"\tlea EAX,dword ptr[EBP+%d]  ;para\n",one->offset);
		}else if(one->kind == FUNCT){ //返回值 在EBP-4
			fprintf(output,"\tlea EAX,dword ptr[EBP-4]  ;use return\n");
		}
}

void usevalue(symbol *one, FILE *output){
		if(one->kind == CONST){  //常量,直接替换
			fprintf(output,"\tmov EAX, %d\n",one->value);
		}
		else if(one->kind == VAR){  //变量
			if(one->level == symTree->level){//同层变量
				fprintf(output,"\tmov EAX,dword ptr [EBP-%d]\n",one->offset);
			}else{
				//非同层访问常量
				int off = symTree->level - one->level;
				off = off*4+4;
				fprintf(output,"\tmov EBX,dword ptr [EBP+%d]\n",off);	//先将apb移到EBX里
				fprintf(output,"\tmov EAX,dword ptr[EBX-%d]\n",one->offset);		//再根据EBX和offset得到数据,移动到EAX、
			}
		}else if(one->kind == POINTER){//指针型！！！！传地址  获取
			fprintf(output,"\tmov EBX,dword ptr[EBP+%d]  ;pointer\n",one->offset);		//指针,是地址,移动到EBX、
			fprintf(output,"\tmov EAX,dword ptr[EBX]\n");		//再根据EBX中的地址,移动到EAX、
		}else if(one->kind == PARA){//形参,只是内容
			fprintf(output,"\tmov EAX,dword ptr[EBP+%d]  ;para\n",one->offset);
		}else if(one->kind == FUNCT){ //返回值 在EBP-4
			fprintf(output,"\tmov EAX,dword ptr[EBP-4]  ;use return\n");
		}
}


void usevalue2(symbol *one, FILE *output){  //ECX, EDX 版本
		if(one->kind == CONST){  //常量,直接替换
			fprintf(output,"\tmov ECX, %d\n",one->value);
		}
		else if(one->kind == VAR){  //变量
			if(one->level == symTree->level){//同层变量
				fprintf(output,"\tmov ECX,dword ptr [EBP-%d]\n",one->offset);
			}else{
				//非同层访问常量
				int off = symTree->level - one->level;
				off = off*4+4;
				fprintf(output,"\tmov EDX,dword ptr [EBP+%d]\n",off);	//先将apb移到EDX里
				fprintf(output,"\tmov ECX,dword ptr[EDX-%d]\n",one->offset);		//再根据EDX和offset得到数据,移动到ECX、
			}
		}else if(one->kind == POINTER){//指针型！！！！传地址  获取
			fprintf(output,"\tmov EDX,dword ptr[EBP+%d]  ;pointer\n",one->offset);		//指针,是地址,移动到EDX、
			fprintf(output,"\tmov ECX,dword ptr[EDX]\n");		//再根据EDX中的地址,移动到ECX、
		}else if(one->kind == PARA){//形参,只是内容
			fprintf(output,"\tmov ECX,dword ptr[EBP+%d]  ;para\n",one->offset);
		}else if(one->kind == FUNCT){ //返回值 在EBP-4
			fprintf(output,"\tmov ECX,dword ptr[EBP-4]  ;use return\n");
		}
}
//保存值 EAX
void savevalue(symbol *tar, FILE *output){
		if(tar->kind == VAR){  //变量
			if(tar->level == symTree->level){//同层变量
				fprintf(output,"\tmov dword ptr [EBP-%d],EAX\n",tar->offset);
			}else{
				//非同层访问常量
				int off = symTree->level - tar->level;
				off = off*4+4;
				fprintf(output,"\tmov EBX,dword ptr [EBP+%d]\n",off);	//先将apb移到EBX里
				fprintf(output,"\tmov dword ptr[EBX-%d],EAX\n",tar->offset);		//再根据EBX和offset得到数据,保存EAX
			}
		}else if(tar->kind == POINTER){//指针型！！！！传地址  获取
			fprintf(output,"\tmov EBX,dword ptr[EBP+%d]  ;get pointer\n",tar->offset);		//指针,是地址,移动到EBX、
			fprintf(output,"\tmov dword ptr[EBX],EAX\n");		//再根据EAX中的地址,移动到[EBx]内存处
		}else if(tar->kind == PARA){//形参,只是内容
			fprintf(output,"\tmov dword ptr[EBP+%d] ,EAX ; save para\n",tar->offset);
		}else if(tar->kind == FUNCT){ //返回值 在EBP-4
			fprintf(output,"\tmov dword ptr[EBP-4], EAX  ;save return\n");
		}
}


void genx86(struct Quate* quate,FILE* output){
	int num;
	symbol *one;
	symbol *two;
	symbol *tar;
	one = findIDEN(quate->one);
	two = findIDEN(quate->two);
	tar = findIDEN(quate->target);
	switch (quate->ins)
	{
	case LABEL:
		if(strcmp(quate->p,"[bg]main") == 0){
			fprintf(output,"start:\n");
			fprintf(output,"\tpush EBP\n");
			fprintf(output,"\tmov EBP,ESP\n");
			symTree = symTreeHead;
			int mm = symTree->symBol[symTree->symCount-1].offset;
			fprintf(output,"\tsub ESP,%d\n",mm);
		}else if(strcmp(quate->p,"end_main") == 0){
			fprintf(output,"\tpush 0\n");
			fprintf(output,"\tCALL ExitProcess\n");
			fprintf(output,"\tLEAVE  \n");
			fprintf(output,"\tRET 0\n");
			fprintf(output,"END start\n");
		}else if(quate->p[0] == '['){
			switch (quate->p[1]){
			case 's':		//proc func 开始
				findAtree(quate->p+4);
				break;
			case 'b':		//proc func 实际开始代码
				fprintf(output,"\nroc_%s PROC\n",quate->p+4);
				fprintf(output,"\tpush EBP\n");
				fprintf(output,"\tmov EBP,ESP\n");
				int here;
				here = symTree->symBol[symTree->symCount-1].offset;
				fprintf(output,"\tsub ESP,%d\n",here);
				break;
			case 'e':		//proc func 结束了
				fprintf(output,"\tmov	eax, dword ptr[ebp - 4]\n");
				fprintf(output,"\tleave \n");
				fprintf(output,"\tret 0\n");
				fprintf(output,"roc_%s ENDP\n\n",quate->p+4);
				symbacklv();

				break;
			}
		}
		else
			fprintf(output,"%s:\n",quate->p);
		break;

	case ASSIGNstr:
		if(quate->one[0] == '#'){
			fprintf(output,"\t;ASSIGN im\n");
			fprintf(output,"\tmov EAX, %s\n",quate->one+1);
		}else{
			//都将one的值放到EAX里面
			//再把EAX的值放到tar
			usevalue(one,output);
		}
		savevalue(tar,output);

		break;
	case SW: //tar 保存到 addr one + offset two
		 //数组层数分析
		if(one->level == symTree->level){ //本层
			fprintf(output,"\tlea EAX,dword ptr [EBP-%d];array addr\n",one->offset);
		}else{	//前几层
			int off = symTree->level - one->level;
			off = off*4+4;
			fprintf(output,"\tmov EBX,dword ptr [EBP+%d]\n",off);	//先将apb移到EBX里
			fprintf(output,"\tlea EAX,dword ptr[EBX-%d] ;array addr\n",one->offset);		//再根据EBX和offset  得到实际地址 保存在 EAX
		}
		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output); //offset 保存在 ECX里面
		fprintf(output,"\tshl ECX,2\n"); //乘4...
		fprintf(output,"\tadd EAX,ECX ;array addr + offset\n"); //实际数组地址计算 保存到EAX
		
		//要保存的数
		if(quate->target[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->target+1);
		}else
			usevalue2(tar,output); //tar 保存到 ECX
		fprintf(output,"\tmov dword ptr [EAX], ECX \n"); //根据EAX的地址 保存ECX的值
		break;
	case LW:
		//计算地址部分与sw一样
		if(one->level == symTree->level){ //本层
			fprintf(output,"\tlea EAX,dword ptr [EBP-%d];array addr\n",one->offset);
		}else{	//前几层
			int off = symTree->level - one->level;
			off = off*4+4;
			fprintf(output,"\tmov EBX,dword ptr [EBP+%d]\n",off);	//先将apb移到EBX里
			fprintf(output,"\tlea EAX,dword ptr[EBX-%d] ;array addr\n",one->offset);		//再根据EBX和offset  得到实际地址 保存在 EAX
		}
		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output); //offset 保存在 ECX里面
		fprintf(output,"\tshl ECX,2\n"); //乘4...
		fprintf(output,"\tadd EAX,ECX ;array addr + offset\n"); //实际数组地址计算 保存到EAX
		fprintf(output,"\tmov ECX,dword ptr[EAX] ;get array addr + offset value\n"); //取出元素
		//由于lw四元式生成时,必定保存在temp里面,这里就直接同层寻址即可
		fprintf(output,"\tmov dword ptr[EBP-%d],ECX \n",tar->offset); 
		break;
	case ADDR:
		//计算地址部分与sw一样
		if(one->level == symTree->level){ //本层
			fprintf(output,"\tlea EAX,dword ptr [EBP-%d];array addr\n",one->offset);
		}else{	//前几层
			int off = symTree->level - one->level;
			off = off*4+4;
			fprintf(output,"\tmov EBX,dword ptr [EBP+%d]\n",off);	//先将apb移到EBX里
			fprintf(output,"\tlea EAX,dword ptr[EBX-%d] ;array addr\n",one->offset);		//再根据EBX和offset  得到实际地址 保存在 EAX
		}
		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output); //offset 保存在 ECX里面
		fprintf(output,"\tshl ECX,2\n"); //乘4...
		fprintf(output,"\tadd EAX,ECX ;array addr + offset\n"); //实际数组地址计算 保存到EAX
		//保存地址
		fprintf(output,"\tmov dword ptr[EBP-%d],EAX \n",tar->offset); 
		break;
	case ADD:

		if(quate->one[0] == '#'){ //立即数
			fprintf(output,"\tmov EAX,%s \n",quate->one+1);
		}else
			usevalue(one,output);

		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output);
		fprintf(output,"\tadd EAX,ECX\n"); //EAX =　EAX + ECX
		savevalue(tar,output);
		break;
	case SUB:

		if(quate->one[0] == '#'){ //立即数
			fprintf(output,"\tmov EAX,%s \n",quate->one+1);
		}else
			usevalue(one,output);

		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output);
		fprintf(output,"\tsub EAX,ECX\n"); //EAX =　EAX + ECX
		savevalue(tar,output);
		break;
	case MUL:

		if(quate->one[0] == '#'){ //立即数
			fprintf(output,"\tmov EAX,%s \n",quate->one+1);
		}else
			usevalue(one,output);

		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output);
		fprintf(output,"\timul ECX\n"); //EDX:EAX =　EAX*ECX
		savevalue(tar,output);
		break;
	case DIVstr:

		if(quate->one[0] == '#'){ //立即数
			fprintf(output,"\tmov EAX,%s \n",quate->one+1);
		}else
			usevalue(one,output);

		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output);
		fprintf(output,"\tcdq\n");
		fprintf(output,"\tidiv ECX\n"); //EDX:EAX =　EAX*ECX
		savevalue(tar,output);
		break;
	case JMP:
		fprintf(output,"\tjmp %s\n",quate->target);
		break;
	case LST:	//	rd, rs, rt   //rd > rs 则 rt=1  否则 rt=0
	case LEST:   // 大于等于 置位
	case EQST:	//rd, rs, rt   //相等置1 否则置0
	case SST:  //小于置位
	case SEST:  //小于等于置位
	case NEST:  //不等于置位
		if(quate->one[0] == '#'){ //立即数
			fprintf(output,"\tmov EAX,%s \n",quate->one+1);
		}else
			usevalue(one,output);

		if(quate->two[0] == '#'){ //立即数
			fprintf(output,"\tmov ECX,%s \n",quate->two+1);
		}else
			usevalue2(two,output);
		fprintf(output,"\tcmp EAX,ECX\n");
		if(quate->ins == LST) //大于置位  成立 不跳转 
			fprintf(output,"\tjle %s\n",quate->target);
		else if(quate->ins == LEST)
			fprintf(output,"\tjl %s\n",quate->target);
		else if(quate->ins == EQST)
			fprintf(output,"\tjne %s\n",quate->target);
		else if(quate->ins == NEST)
			fprintf(output,"\tje %s\n",quate->target);
		else if(quate->ins == SST)
			fprintf(output,"\tjge %s\n",quate->target);
		else if(quate->ins == SEST)
			fprintf(output,"\tjg %s\n",quate->target);
	break;

	case PUSH:
		if(quate->one[0]== '@'){
			one = findIDEN(quate->one+1);
			useaddr(one,output);
			fprintf(output,"\tpush EAX\n");
		}
		else if(quate->one[0] == '#'){
			fprintf(output,"\tmov EAX,%s \n",quate->one+1);
			fprintf(output,"\tpush EAX\n");
		}
		else
		{
			usevalue(one,output);
			fprintf(output,"\tpush EAX\n");
		}
		break;
	case CALL:
		fprintf(output,"  ;apd area\n");
		int herelv;
		herelv = symTree->level;
		symbolTree* callTree;
		callTree = findpf(quate->one);
		int calllv;
		calllv = callTree->level;
		if(calllv == herelv+1){
			for(int i = herelv;i>0;i--){
				fprintf(output,"\tmov  EAX, dword ptr [EBP + %d] \n",(i+1)*4);
				fprintf(output,"\tpush EAX\n");
			}
			fprintf(output,"\tpush EBP\n");

		}
		else{
			for(int i = herelv;i>herelv-calllv;i--){
				fprintf(output,"\tmov  EAX, dword ptr [EBP + %d] \n",(i+1)*4);
				fprintf(output,"\tpush EAX\n");
			}
		}
		fprintf(output,"\tcall roc_%s\n",quate->one);
		if(one->kind == FUNCT){
			savevalue(tar,output);
		}
		break;
	case WRITE:
		if(quate->target[0] == '#'){
			num = atoi(quate->target+1);
			fprintf(output,"\tpush %d\n",num);
			fprintf(output,"\tpush offset print_int\n");
			fprintf(output,"\tcall crt_printf\n");
			fprintf(output,"\tadd  ESP ,  8\n");
			fprintf(output,"\tpush 10\n");
			fprintf(output,"\tpush offset print_char\n");
			fprintf(output,"\tcall crt_printf\n");
			fprintf(output,"\tadd  ESP ,  8\n");
		}else if(quate->target[0] == '@'){
			fprintf(output,"\tpush offset %s\n",strName[strcount++]);
			fprintf(output,"\tpush offset print_string\n");
			fprintf(output,"\tcall crt_printf\n");
			fprintf(output,"\tadd  ESP ,  8\n");
			fprintf(output,"\tpush 10\n");
			fprintf(output,"\tpush offset print_char\n");
			fprintf(output,"\tcall crt_printf\n");
			fprintf(output,"\tadd  ESP ,  8\n");
		}else
		{
			usevalue(tar,output);
			fprintf(output,"\tpush EAX\n");
			if(tar->type == INT)
				fprintf(output,"\tpush offset print_int\n");
			else
				fprintf(output,"\tpush offset print_char\n");
			fprintf(output,"\tcall crt_printf\n");
			fprintf(output,"\tadd  ESP ,  8\n");
			fprintf(output,"\tpush 10\n");
			fprintf(output,"\tpush offset print_char\n");
			fprintf(output,"\tcall crt_printf\n");
			fprintf(output,"\tadd  ESP ,  8\n");
		}
		break;
	case READ:
		useaddr(tar,output);
		fprintf(output,"\tpush EAX\n");
		if(tar->type == INT)
			fprintf(output,"\tpush offset scanf_int\n");
		else
			fprintf(output,"\tpush offset scanf_char\n");
		fprintf(output,"\tcall crt_scanf\n");
		fprintf(output,"\tadd  ESP ,  8\n");
		break;
	default:
		break;
	}
}
