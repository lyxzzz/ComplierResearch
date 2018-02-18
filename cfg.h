#ifndef H_CFG
#define H_CFG
#include"type_define.h"
#include<unordered_map>
#include<iostream>
#include"lrparsingtable.h"
using std::unordered_map;
using std::ostream;
class CFG
{
private:
	vector<Generation> __cfg_expressions;//一个cfg中的所有表达式
	vector<First> __first_nonterminal;//一个cfg中所有非终端符号的first集合
	unordered_map<terminal, int> __terminal_list;//一个cfg中的所有终结符号
	unordered_map<non_terminal, int> __nonterminal_list;//cfg中的所有非终结符号
	vector<ClosedSet> __closed_set;//cfg中的所有终结符号对应的闭合集合
	unordered_map<terminal, op_property> op;//在yyac文件中定义的cfg中所有终结符号的优先级和左右结合性
	LR_ParsingTable __mytable;//cfg对应的lr(1)分析表
	int __terminal_num;//cfg中总共的终结符号个数
	int __nonterminal_num;//cfg中所有非终结符号的个数
	void analysis_first(First & sym);//分析first集合
	vector<int> state_stack;//分析文本时的状态栈
	vector<symbol> symbol_stack;//分析文本时的符号栈
	void printNowState(ofstream & out, const vector<Token> & str,int index);
public:
	CFG(ofstream & out,const vector<string> & cfgs,const vector<string> & _op);
	friend ostream & operator << (ostream & out, const CFG & cfg);
	friend void LR_ParsingTable::generate(ofstream & out,const non_terminal & start, CFG & cfg);
	friend void LR_ParsingTable::analysis_status(int index, CFG & cfg);
	friend bool LR_ParsingTable::analysis_generation(int & now_num, unordered_map<symbol, LR_Status, pairhash> & directions, LR_Generation & g, LR_Status & status, CFG & cfg);
	friend void LR_ParsingTable::analysis_closedset(const set<terminal> & ori_forward, const non_terminal & nt, unordered_map<non_terminal, ClosedSet> & closedset, CFG & cfg);
	friend int LR_ParsingTable::move_step(CFG & cfg, const non_terminal & next_sym);
	friend lr_skip LR_ParsingTable::next_step(CFG & cfg, const terminal & next_sym);
	set<terminal> getFirst(const symbol & sym);//获得first集合
	void analysis(ofstream & out, const vector<Token> & str);//分析文本
};
#endif // H_CFG_READER
