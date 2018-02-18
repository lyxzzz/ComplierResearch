#include"regex.h"
#include<iostream>
#include<algorithm>
#include"dfa.h"
using std::cout;
using std::transform;
Regex::Regex(const string & re, const string & _name) :symbol(Specific_symbol::getInstance()), dfa_state(new DFA()), name(_name),pattern(re)
{
	func init = symbol->find('^');
	State* p = init(symbol, nfa_root, '^');
	nfa_root = symbol->getStart();
	for (auto i : re)
	{
		func op = symbol->find(i);
		p = (*op)(symbol,p, i);
	}
	func fin = symbol->find('$');
	fin(symbol, p, '$');
	//output(*this);
	//std::cout << std::endl;
	dfa_state->trans(nfa_root);
	dfa_state->simplify();
}
void output(Regex r)
{
	set<int> i;
	output(r.nfa_root, i);
}
void output(State* a, set<int> & s)
{
	using std::cout;
	using std::endl;
	cout << a->num<< "\tstate:"<<(a->end?"stop":"go") << "\t";
	for (auto i : a->next)
	{
		if (i.first != 0)
		{
			cout << i.first << ":" << i.second->num << "\t";
		}
		else
		{
			cout << "E" << ":" << i.second->num << "\t";
		}
	}
	cout << endl;
	for (auto i : a->next)
	{
		if (s.insert(i.second->num).second)
		{
			output(i.second, s);
		}
	}
}
bool Regex::match(const string & ma) const
{
	bool flag = dfa_state->match(ma);
	//cout << (flag ? "true" : "false") << std::endl;
	return flag;
}
vector<pair<int, int>> Regex::group(const string & ma) const
{
	return dfa_state->group(ma);
}
vector<pair<int, int>> Regex::greedy_group(const string & ma) const
{
	return dfa_state->greedy_group(ma);
}
bool find_throughnull(State * _s)
{
	if (_s->end)
	{
		return true;
	}
	else
	{
		auto range = _s->next.equal_range(char(0));
		for (auto it = range.first; it != range.second; ++it)
		{
			if (find_throughnull(it->second))
			{
				return true;
			}
		}
		return false;
	}
}
void Regex::output()const
{
	cout << "=====================================DFA======================================" << std::endl;
	dfa_state->printDFA();
	cout << std::endl << std::endl << std::endl;
	cout << "=====================================SDFA======================================" << std::endl;
	dfa_state->printSDFA();
}
int Regex::greedy_match(const string & ma) const
{
	return dfa_state->greedy_match(ma);
}