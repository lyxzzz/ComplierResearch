#pragma once
#include"cfg.h"
class Yacc
{
private:
	const string path;//����ļ���·��
	CFG * __cfg;
	const string filename;//�������yyac�ļ�
public:
	static Lex lex;//����ƥ���lex�ļ�
	Yacc(const string & path);
	void analysis(const string & path);//Ҫ�������ı���·��
};