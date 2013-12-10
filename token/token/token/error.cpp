#include "error.h"
#include "token.h"
#include <iostream>

int errornum = 0;
//extern int lnum;
//extern int lcounter;

void error(char *p){
	errornum++;
	std::cout<< "######line " << lnum << "letter "<<lcounter<< "error" << p <<std::endl;
}