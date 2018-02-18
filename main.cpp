#include<iostream>
#include<fstream>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
#include"yacc.h"
#include"regex\regex.h"
int main()
{
	Yacc y1("input/cfg1");
	y1.analysis("input/input.cpp");
	//Yacc y2("input/cfg2");
	//y2.analysis("input/input.txt");
	cin.get();
}