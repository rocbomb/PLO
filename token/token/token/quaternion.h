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
	ASSIGNstr,  //Src1,Des	  ��ֵDes=Src
	LST,	//	rd, rs, rt   //rd > rs �� rt=1  ���� rt=0
	LEST,   // ���ڵ��� ��λ
	EQST,	//rd, rs, rt   //�����1 ������0
	SST,   //С����λ
	SEST,  //С�ڵ�����λ
	NEST,  //��������λ
	BEQ, //	rd, rs, rt   //rd == rs ����ת��rt��
	JMP, //   rd, rs, rt   //��ת��rd��  rs rtΪ��
	SW, //  rd, rs, rt   //����rd ��rt+rs  rtΪ��ַ��rsΪoffset
	LW,  //  rd, rs, rt   //��ȡrt+rs��ַ�������� ����rs
	CALL,  //	rd, rs, rt   //�������ã�rdΪ���ú�������rsΪ����������rtΪ����ֵ
	RET,		//Des						�������أ�DesΪ����ֵ
	READ,  //	rd, rs, rt	 //				���벢д��rt
	WRITE, //  rd, rs, rt	 //			��rt���
	NOP,	  //							�����
	END,		//						����������־
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