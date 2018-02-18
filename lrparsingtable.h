#include"cfg.h"
#ifndef LR_TABLE
#define LR_TABLE
#include"type_define.h"
#include<xhash>
using std::hash;
/*
���hash<T1,T2>�Ľṹ
*/
struct pairhash 
{ 
	template<class T1, class T2>    
	size_t operator() (const pair<T1, T2> &x) const 
	{ 
		hash<T1> h1;        
		hash<T2> h2;        
		return h1(x.first) ^ h2(x.second); 
	} 
};

struct LR_Generation
{
	Generation generation;//LR����ʽ��Ӧ������ʽ
	set<terminal> forward;//LR����ʽ����ǰ���ַ�
	int dot_index;//���λ��
	LR_Generation(const Generation & _g,int _index=0) :generation(_g), forward(),dot_index(_index) {};
	LR_Generation(const non_terminal & _l, const string & _r, int _num,int _index = 0) :generation(Generation(_l, _r, _num)), forward(), dot_index(_index) {};
	bool operator ==(const LR_Generation & _val);
	friend ostream & operator << (ostream & out, const LR_Generation & g);
};
struct LR_Status
{
	vector<LR_Generation> raw;//LR״̬�ĺ�������ʽ
	int status_num;//״̬��
	terminal now_symbol;//���LR״̬���Թ�Լ����now_symbolָ���Լʽ���е����һ���ս��ַ�(�����Ƚ����ȼ������ҽ����)
	vector<lr_skip> terminal_skip;//�ս���ŵ���ת
	vector<int> non_terminal_skip;//���ս���ŵ���ת
	LR_Status(int _num = -1) :raw(), status_num(_num), terminal_skip(), non_terminal_skip(), now_symbol("null") {}
	void push_back(const LR_Generation & _val);
	bool operator ==(const LR_Status & _val);
	friend ostream & operator << (ostream & out, const LR_Status & LR_Status);
};

class CFG;
class LR_ParsingTable
{
private:
	friend class CFG;
	enum {default_dot_index=0};
	vector<LR_Status> __state;
	int __number;
	void analysis_closedset(const set<terminal> & ori_forward, const non_terminal & nt, unordered_map<non_terminal, ClosedSet> & closedset, CFG & cfg);//�����պϼ��ϲ�����״̬
	bool analysis_generation(int & now_num, unordered_map<symbol, LR_Status, pairhash> & directions,LR_Generation & g, LR_Status & status, CFG & cfg);//��������ʽ������״̬
	void analysis_status(int index, CFG & cfg);//����״̬
public:
	LR_ParsingTable() : __number(0), __state() {};
	void generate(ofstream & out,const non_terminal & start,CFG & cfg);//����LR������
	lr_skip next_step(CFG & cfg, const terminal & next_sym);//��һ����ת
	int move_step(CFG & cfg, const non_terminal & next_sym);//�ƶ�
};
#endif