#pragma once
#include"cfg.h"
class Yacc
{
private:
	const string path;//输出文件的路径
	CFG * __cfg;
	const string filename;//输入的类yyac文件
public:
	static Lex lex;//进行匹配的lex文件
	Yacc(const string & path);
	void analysis(const string & path);//要分析的文本的路径
};