#ifndef M_REGEX_H
#define M_REGEX_H
#include "specific_symbol.h"
#include<set>
//相关注释在实验报告中的重要数据结构中解释
using std::set;
class DFA;
class Regex
{
private:
	Specific_symbol * symbol;
	State * nfa_root;
	DFA * dfa_state;
	const string name;
	const string pattern;
public:
	Regex() :name(""), pattern("") {};
	Regex(const string & re, const string & name);
	~Regex() {};
	bool match(const string & ma) const;
	int greedy_match(const string & ma) const;
	friend void output(Regex r);
	const string & getName() const { return name; };
	const string & getPattern() const { return pattern; };
	vector<pair<int, int>> group(const string & ma) const;
	vector<pair<int, int>> greedy_group(const string & ma) const;
	void output()const;
};
bool find_throughnull(State * _s);
void output(State* a,set<int> & s);
#endif