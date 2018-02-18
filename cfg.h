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
	vector<Generation> __cfg_expressions;//һ��cfg�е����б��ʽ
	vector<First> __first_nonterminal;//һ��cfg�����з��ն˷��ŵ�first����
	unordered_map<terminal, int> __terminal_list;//һ��cfg�е������ս����
	unordered_map<non_terminal, int> __nonterminal_list;//cfg�е����з��ս����
	vector<ClosedSet> __closed_set;//cfg�е������ս���Ŷ�Ӧ�ıպϼ���
	unordered_map<terminal, op_property> op;//��yyac�ļ��ж����cfg�������ս���ŵ����ȼ������ҽ����
	LR_ParsingTable __mytable;//cfg��Ӧ��lr(1)������
	int __terminal_num;//cfg���ܹ����ս���Ÿ���
	int __nonterminal_num;//cfg�����з��ս���ŵĸ���
	void analysis_first(First & sym);//����first����
	vector<int> state_stack;//�����ı�ʱ��״̬ջ
	vector<symbol> symbol_stack;//�����ı�ʱ�ķ���ջ
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
	set<terminal> getFirst(const symbol & sym);//���first����
	void analysis(ofstream & out, const vector<Token> & str);//�����ı�
};
#endif // H_CFG_READER
