#pragma once
#include"regex.h"
#include<iostream>
//相关注释在实验报告中的重要数据结构中解释
using std::cout;
using std::wstring;
struct Token
{
	string typeName;
	string value;
	Token(const string & _t, const string & _v) :typeName(_t), value(_v) {};
};
class Lex
{
private:
	vector<Regex> list;
	vector<pair<Regex, string>> checklist;
	Regex replace;
	Regex* default_regex;
	void handle_str(string & str);
public:
	Lex() :list(), checklist(), replace("", "") {};
	Lex(const string & path);
	void analysis(const string & path, vector<Token> & result) const;
	bool isTerminal(const string & str) const;
};
std::ostream & operator<<(std::ostream & out,const Token & t);