#include"type_define.h"
#include"util.h"
#include<ostream>
#include"yacc.h"
using std::ostream;
Regex split_terminal("'[ -~]'","split");
bool isTerminal(const string & t)
{
	return Yacc::lex.isTerminal(t);
}
Formula::Formula(const string & _val) :sym()
{
	vector<string> group;
	split(group, _val);
	for (const string & i : group)
	{
		if (split_terminal.match(i))
		{
			sym.push_back(make_pair(i.substr(1,1), true));
		}
		else
		{
			sym.push_back(make_pair(i, Yacc::lex.isTerminal(i)));
		}
	}
}
bool First::insert(const non_terminal & value)
{
	return value == this->val ? false : this->nonterminal_set.insert(value).second;
}
ostream & operator << (ostream & out, const Generation & g)
{
	out << g.number << "." << g.left_value << "\t:";
	for (const symbol & val : g.right_value.sym)
	{
		out << "{" << val.first << "," << (val.second ? "terminal" : "nonterminal") << "}";
	}
	out << std::endl;
	return out;
}