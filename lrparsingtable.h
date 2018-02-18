#include"cfg.h"
#ifndef LR_TABLE
#define LR_TABLE
#include"type_define.h"
#include<xhash>
using std::hash;
/*
添加hash<T1,T2>的结构
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
	Generation generation;//LR生成式对应的生成式
	set<terminal> forward;//LR生成式的向前看字符
	int dot_index;//点的位置
	LR_Generation(const Generation & _g,int _index=0) :generation(_g), forward(),dot_index(_index) {};
	LR_Generation(const non_terminal & _l, const string & _r, int _num,int _index = 0) :generation(Generation(_l, _r, _num)), forward(), dot_index(_index) {};
	bool operator ==(const LR_Generation & _val);
	friend ostream & operator << (ostream & out, const LR_Generation & g);
};
struct LR_Status
{
	vector<LR_Generation> raw;//LR状态的核心生成式
	int status_num;//状态号
	terminal now_symbol;//如果LR状态可以规约，则now_symbol指向规约式子中的最后一个终结字符(用来比较优先级和左右结合性)
	vector<lr_skip> terminal_skip;//终结符号的跳转
	vector<int> non_terminal_skip;//非终结符号的跳转
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
	void analysis_closedset(const set<terminal> & ori_forward, const non_terminal & nt, unordered_map<non_terminal, ClosedSet> & closedset, CFG & cfg);//分析闭合集合并加入状态
	bool analysis_generation(int & now_num, unordered_map<symbol, LR_Status, pairhash> & directions,LR_Generation & g, LR_Status & status, CFG & cfg);//分析生成式并加入状态
	void analysis_status(int index, CFG & cfg);//分析状态
public:
	LR_ParsingTable() : __number(0), __state() {};
	void generate(ofstream & out,const non_terminal & start,CFG & cfg);//生成LR分析表
	lr_skip next_step(CFG & cfg, const terminal & next_sym);//下一次跳转
	int move_step(CFG & cfg, const non_terminal & next_sym);//移动
};
#endif