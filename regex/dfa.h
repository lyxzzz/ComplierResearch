#ifndef H_NFADFA_M
#define H_NFADFA_M
#include"regex.h"
#include<unordered_map>
#include<vector>
//相关注释在实验报告中的重要数据结构中解释
using std::vector;
using std::unordered_map;
#include<set>
using std::set;
typedef unordered_map<int, State*> nfaset;
struct DFA_State
{
	nfaset _unmap;
	set<char> direction;
	int _number;
	bool end;
	bool isAnalysis;
	unordered_map<char, DFA_State*> _next;
	DFA_State(int num) :_unmap(), _number(num), _next(), end(false), direction(), isAnalysis(false){};
	void insert(State*);
	bool operator== (const DFA_State & a);
	bool operator== (const DFA_State * const a);
	~DFA_State() {};
};
class DFA
{
private:
	vector<DFA_State*> list;
	vector<DFA_State*> s_list;
	DFA_State * dfa_root;
	int dfa_num;
	bool find(const string & ma, const DFA_State * const now, int index = 0) const;
	int group_str(const string & ma, const DFA_State * const now, int index) const;
	int greedy_group_str(const string & ma, const DFA_State * const now, int index) const;
public:
	DFA() :list(),s_list(),dfa_root(nullptr), dfa_num(0) {};
	void trans(State* root);
	void simplify();
	void printDFA();
	void printSDFA();
	bool match(const string & ma);
	vector<pair<int, int>> group(const string & ma);
	vector<pair<int, int>> greedy_group(const string & ma);
	int greedy_match(const string & m); 
};
void printvector(const vector<DFA_State*> & list);
void e_set(DFA_State * set);
void e_find(State* state, DFA_State * set);
void move(char a, DFA_State * from, DFA_State * to);
#endif // !H_NFADFA_M
