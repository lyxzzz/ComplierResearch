#include"cfg.h"
#include"util.h"
using std::make_pair;
using std::pair;
ostream & operator << (ostream & out, const CFG & cfg)
{
	out << "statement:\n";
	for (const Generation & g : cfg.__cfg_expressions)
	{
		out << g.number << "." << g.left_value << "\t:";
		for (const symbol & val : g.right_value.sym)
		{
			out << "{" << val.first << "," << (val.second ? "terminal" : "nonterminal") << "}";
		}
		out << std::endl;
	}
	out << "nonterminal_list:\n";
	for (const pair<non_terminal, int> & val : cfg.__nonterminal_list)
	{
		out << val.first << ":" << val.second << std::endl;
	}
	out << "terminal_list:\n";
	for (const pair<terminal, int> & val : cfg.__terminal_list)
	{
		out << val.first << ":" << val.second << std::endl;
	}
	out << "first_list:\n";
	for (const First & val : cfg.__first_nonterminal)
	{
		out << val.val << "\t:[";
		for (terminal i : val.terminal_set)
		{
			out << i << ",";
		}
		out << "]\n";
	}
	out << "closed_set:\n";
	for (const ClosedSet & val : cfg.__closed_set)
	{
		out << val.val << "\n";
		for (auto i : val.right_value)
		{
			for (auto j : i.right_value.sym)
			{
				out << "{" << j.first << "," << (j.second ? "terminal" : "nonterminal") << "}";
			}
			out << std::endl;
		}
		out << "[";
		for (auto i : val.inner_forward)
		{
			out << i << ",";
		}
		out << "]\n";
		out << std::endl;
	}
	return out;
}
CFG::CFG(ofstream & out, const vector<string> & cfgs, const vector<string> & _op) :__terminal_list(), __nonterminal_list(), __cfg_expressions(),
__terminal_num(1), __nonterminal_num(1), __first_nonterminal(),
__closed_set(), op(), state_stack(), symbol_stack()
{
	for (const string & temp : cfgs)
	{
		int split_index = temp.find(split_symbol);
		non_terminal left = temp.substr(0, split_index);
		string str = temp.substr(split_index + split_symbol.size());
		vector<string> right;
		split(right, str, "|");
		for (string i : right)
		{
			Generation val(left, i, __cfg_expressions.size());
			__cfg_expressions.push_back(val);
			auto insert_result = this->__nonterminal_list.insert(make_pair(val.left_value, this->__nonterminal_num));
			if (insert_result.second)
			{
				this->__closed_set.push_back(ClosedSet(val.left_value));
				this->__closed_set[this->__nonterminal_num - 1].push_back(this->__cfg_expressions[this->__cfg_expressions.size() - 1]);
				++this->__nonterminal_num;
			}
			else
			{
				this->__closed_set[insert_result.first->second - 1].push_back(this->__cfg_expressions[this->__cfg_expressions.size() - 1]);

			}
			for (symbol j : val.right_value.sym)
			{
				bool plus = j.second ? this->__terminal_list.insert(make_pair(j.first, this->__terminal_num)).second : this->__nonterminal_list.insert(make_pair(j.first, this->__nonterminal_num)).second;
				if (plus)
				{
					if (j.second)
					{
						++this->__terminal_num;
					}
					else
					{
						this->__closed_set.push_back(ClosedSet(j.first));
						++this->__nonterminal_num;
					}
				}
			}
		}
	}
	this->__terminal_list.insert(make_pair("null", this->__terminal_num++));
	this->__first_nonterminal.resize(this->__nonterminal_num - 1);
	for (const pair<non_terminal, int> & temp : this->__nonterminal_list)
	{
		this->__first_nonterminal[temp.second-1] = First(temp.first);
	}
	for (First & temp:this->__first_nonterminal)
	{
		if (!temp.use)
		{
			analysis_first(temp);
		}
	}
	for (ClosedSet & temp : this->__closed_set)
	{
		non_terminal left = temp.val;
		for (auto it : temp.right_value)
		{
			if (it.right_value.sym[0].first == left)
			{
				if (it.right_value.sym.size() == 1)
				{
					temp.inner_forward.insert("null");
				}
				else
				{
					if (it.right_value.sym[1].second)
					{
						temp.inner_forward.insert(it.right_value.sym[1].first);
					}
					else
					{
						const First * t = &this->__first_nonterminal[this->__nonterminal_list[it.right_value.sym[1].first] - 1];
						temp.inner_forward.insert(t->terminal_set.begin(), t->terminal_set.end());
					}
				}
			}
		}
	}
	int num = 1;
	for (const string & i : _op)
	{
		vector<string> temp;
		split(temp, i, "|");
		for (const string & j : temp)
		{
			int index = j.find(',');
			terminal t = j.substr(1, index - 1);
			op[t] = make_pair(num, j[index + 1] == 'L');
		}
		++num;
	}
	for (auto i : this->__terminal_list)
	{
		if (op.find(i.first) == op.end())
		{
			op[i.first] = make_pair(0, true);
		}
	}
	//std::cout << *this;
	this->state_stack.push_back(0);
	this->__mytable.generate(out,this->__cfg_expressions[0].left_value, *this);
}
void CFG::analysis_first(First & val)
{
	if (val.use)
	{
		return;
	}
	val.use = true;
	for (const Generation & i : this->__cfg_expressions)
	{
		if (i.left_value == val.val)
		{
			for (std::size_t j = 0; j < i.right_value.sym.size(); ++j)
			{
				symbol value = i.right_value.sym[j];
				if (value.second)
				{
					val.terminal_set.insert(value.first);
					break;
				}
				else
				{
					if (val.insert(value.first))
					{
						First * temp = & this->__first_nonterminal[this->__nonterminal_list[value.first]-1];
						analysis_first(*temp);
						for (terminal sign : temp->terminal_set)
						{
							val.terminal_set.insert(sign);
						}
						if (temp->terminal_set.find("null") == temp->terminal_set.end())
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}
set<terminal> CFG::getFirst(const symbol & sym)
{
	set<terminal> result;
	if (sym.second)
	{
		result.insert(sym.first);
	}
	else
	{
		auto & temp = this->__first_nonterminal[this->__nonterminal_list[sym.first] - 1].terminal_set;
		result.insert(temp.begin(), temp.end());
	}
	return result;
}
void CFG::analysis(ofstream & out,const vector<Token> & str)
{
	bool end = true;
	vector<Token> vec = str;
	vec.push_back(Token("null","null"));
	std::size_t i = 0;
	while (end)
	{
		this->printNowState(out,vec, i);
		lr_skip skip = this->__mytable.next_step(*this, vec[i].typeName);
		if (skip.first == ERROR&&skip.second == false)
		{
			out << "ERROR!!" << std::endl;
			break;
		}
		else if (skip.first == ERROR&&skip.second == true)
		{
			end = false;
			out << "accept" << std::endl;
		}
		else
		{
			if (skip.second)
			{
				const Generation & temp = this->__cfg_expressions[skip.first];
				for (std::size_t cot = 0; cot < temp.right_value.sym.size(); ++cot)
				{
					this->state_stack.pop_back();
					this->symbol_stack.pop_back();
				}
				int skip = this->__mytable.move_step(*this, temp.left_value);
				if (skip == -1)
				{
					out << "ERROR!!" << std::endl;
					break;
				}
				else
				{
					this->state_stack.push_back(skip);
					this->symbol_stack.push_back(make_pair(temp.left_value,false));
				}
			}
			else
			{
				this->state_stack.push_back(skip.first);
				this->symbol_stack.push_back(make_pair(vec[i].typeName, true));
				++i;
			}
		}
	}
	//this->printNowState(vec,i);
}
void CFG::printNowState(ofstream & out,const vector<Token> & str, int index)
{
	out << "stack:{";
	for (auto j : this->state_stack)
	{
		out << j << ",";
	}
	out << "}\n" << "\tsymbol:";
	for (auto j : this->symbol_stack)
	{
		out << j.first  << " ";
	}
	out << "\n\tinput:";
	for (std::size_t j = index; j < str.size() - 1; ++j)
	{
		out << "(" << str[j].typeName  << "," <<str[j].value << ")" << " ";
		//out  << str[j].typeName<< " ";
		//if (str[j].typeName == "delimiter")
		//{
		//	out << "\n\t";
		//}
	}
	out << std::endl;
}