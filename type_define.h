#ifndef H_TYPE_DEFINE
#define H_TYPE_DEFINE
#include<string>
#include<vector>
#include<unordered_set>
using std::unordered_set;
#include"regex\regex.h"
#include"regex\lex.h"
#include<fstream>
using std::ofstream;
using std::vector;
using std::ostream;
using std::string;
using std::basic_string;
using std::pair;
typedef string terminal; //终结符号
typedef string non_terminal; //非终结符号
typedef pair<string,bool> symbol; //符号(true为终结符号，false为非终结)
typedef pair<int,bool> lr_skip;//lr分析表中每一栏的属性（bool值为是否规约，int值为跳转的路径）
typedef pair<int, bool> op_property;//操作符的优先级(bool为是否是左结合)
const int ERROR = -1;//代表空路径
const string split_symbol = "->";//分割字符
struct Formula
{
	vector<symbol> sym;
	Formula(const string & _val);
};
struct Generation
{
	non_terminal left_value;//生成式左值 为E->T*B中的E
	Formula right_value;//生成式右端 如 E * E等
	int number;//生成式号
	Generation(const non_terminal & _l,const string & _r, int _num) :left_value(_l), right_value(_r), number(_num) {}
	friend ostream & operator << (ostream & out, const Generation & g);
};
struct First
{
	bool use;//代表这个非终端符号的first集合是否被探索
	non_terminal val;//非终端符号
	set<non_terminal> nonterminal_set;//该非终端符号first集合中的非终端符号
	set<terminal> terminal_set;//该非终端符号的first集合
	First() :val(), nonterminal_set(), terminal_set(),use(false) {}
	First(const non_terminal & _val) :val(_val), nonterminal_set(), terminal_set(), use(false) {}
	bool insert(const non_terminal & value);
};
struct ClosedSet
{
	set<terminal> inner_forward;//一个非终端符号的闭合集中产生的向前看字符
	non_terminal val;//非终端符号
	vector<Generation> right_value;//该非终端符号对应的所有的生成式
	set<symbol> directions;//方向
	ClosedSet(const non_terminal & _val = "") :val(_val), right_value(), directions(), inner_forward() {};
	void push_back(const Generation & t) 
	{ 
		right_value.push_back(t);
		directions.insert(t.right_value.sym[0]);
	}
};
bool isTerminal(const string & t);
#endif
