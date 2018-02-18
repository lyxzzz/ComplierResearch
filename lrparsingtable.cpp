#include"lrparsingtable.h"
#include<iostream>
#include<fstream>
using std::ofstream;
using std::cout;
using std::endl;
void init_state(LR_Status & s,int i1,int i2);
bool LR_Generation::operator ==(const LR_Generation & _val)
{
	if (this->dot_index != _val.dot_index)
	{
		return false;
	}
	return this->generation.number == _val.generation.number;
}
bool LR_Status::operator ==(const LR_Status & _val)
{
	if (this->raw.size() != _val.raw.size())
	{
		return false;
	}
	else
	{
		for (auto i : this->raw)
		{
			bool find = false;
			for (auto j : _val.raw)
			{
				if (i == j)
				{
					if (i.forward == j.forward)
					{
						find = true;
						break;
					}
				}
			}
			if (!find)
			{
				return false;
			}
		}
	}
	return true;
}
void LR_Status::push_back(const LR_Generation & _val)
{
	for (LR_Generation & temp : this->raw)
	{
		if (temp == _val)
		{
			temp.forward.insert(_val.forward.begin(), _val.forward.end());
			return;
		}
	}
	this->raw.push_back(_val);
}
ostream & operator << (ostream & out, const LR_Status & t)
{
	out << "*************************************************************\n";
	out << "state:" << t.status_num << "\t:" << endl;
	for (auto i : t.raw)
	{
		out << i.generation;
		out << ",dot=" << i.dot_index << ",[";
		for (auto j : i.forward)
		{
			out << j << ",";
		}
		out << "]";
		out << endl;
	}
	for (auto i : t.terminal_skip)
	{
		out << "[" << i.first << "," << (i.second?"r":"s") <<  "]";
	}
	out << endl;
	for (auto i : t.non_terminal_skip)
	{
		out << i << ",";
	}
	out << endl;
	out << "*************************************************************\n";
	return out;
}
ostream & operator << (ostream & out, const LR_Generation & g)
{
	out << g.generation;
	out << ",dot=" << g.dot_index << ",[";
	for (auto j : g.forward)
	{
		out << j << ",";
	}
	out << "]";
	out << endl;
	return out;
}
void LR_ParsingTable::generate(ofstream & out,const non_terminal & start, CFG & cfg)
{
	//ofstream out;
	//out.open("output/cfg2");
	LR_Status init(__number++);
	init.raw.push_back(LR_Generation("init", start, ERROR));
	init.raw[0].forward.insert("null");
	this->__state.push_back(init);
	for (std::size_t index = 0; index < this->__state.size(); ++index)
	{
		analysis_status(index, cfg);
		out << this->__state[index];
	}
}
void LR_ParsingTable::analysis_status(int index, CFG & cfg)
{
	init_state(this->__state[index], cfg.__terminal_num, cfg.__nonterminal_num);
	int this_num = 0;
	unordered_map<symbol, LR_Status,pairhash> directions;
	unordered_map<non_terminal,ClosedSet> closedset;
	bool flag = false;
	for (auto g : this->__state[index].raw)
	{
		if (!analysis_generation(this_num, directions, g, this->__state[index], cfg))
		{
			non_terminal sym = g.generation.right_value.sym[g.dot_index].first;
			set<terminal> next_forward;
			if (std::size_t(g.dot_index + 1) >= g.generation.right_value.sym.size())
			{
				next_forward.insert(g.forward.begin(),g.forward.end());
			}
			else
			{
				auto & t_set = cfg.getFirst(g.generation.right_value.sym[g.dot_index + 1]);
				next_forward.insert(t_set.begin(), t_set.end());
			}
			analysis_closedset(next_forward, sym, closedset, cfg);
		}
	}
	for (auto it : closedset)
	{
		for (auto val : it.second.right_value)
		{
			LR_Generation temp(val);
			temp.forward = it.second.inner_forward;
			analysis_generation(this_num, directions, temp, this->__state[index], cfg);
		}
	}
	for (auto it : directions)
	{
		bool find = false;
		LR_Status* state = &it.second;
		for (auto val : this->__state)
		{
			if (val == it.second)
			{
				state = &val;
				find = true;
				break;
			}
		}
		if (!find)
		{
			state->status_num = this->__number++;
			this->__state.push_back(*state);
		}
		if (it.first.second)
		{
			auto & coll = this->__state[index].terminal_skip[cfg.__terminal_list[it.first.first] - 1];
			if (this->__state[index].now_symbol=="null"|| (coll.first==ERROR&&coll.second==false))
			{
				coll = make_pair(state->status_num, false);
			}
			else
			{
				op_property op1 = cfg.op[this->__state[index].now_symbol];
				op_property op2 = cfg.op[it.first.first];
				if (op1.first < op2.first)
				{
					coll = make_pair(state->status_num, false);
				}
				else if (op1.first == op2.first)
				{
					if (!op1.second)
					{
						coll = make_pair(state->status_num, false);
					}
				}
			}
		}
		else
		{
			this->__state[index].non_terminal_skip[cfg.__nonterminal_list[it.first.first] - 1] = state->status_num;
		}
	}
}
bool LR_ParsingTable::analysis_generation(int & now_num, unordered_map<symbol, LR_Status, pairhash> & directions, LR_Generation & g, LR_Status & status, CFG & cfg)
{
	if (g.dot_index == g.generation.right_value.sym.size())
	{
		for (int i = g.dot_index - 1; i >= 0; --i)
		{
			if (g.generation.right_value.sym[i].second)
			{
				status.now_symbol = g.generation.right_value.sym[i].first;
				break;
			}
		}
		for (const terminal & temp : g.forward)
		{
			status.terminal_skip[cfg.__terminal_list[temp] - 1] = make_pair(g.generation.number, true);
		}
		return true;
	}
	else
	{
		const symbol & temp = g.generation.right_value.sym[g.dot_index];
		if (directions.find(temp) == directions.end())
		{
			directions.insert(make_pair(temp, LR_Status(now_num++)));
		}
		LR_Status & state = directions[temp];
		LR_Generation value = g;
		++value.dot_index;
		state.push_back(value);
		return temp.second;
	}
}
void LR_ParsingTable::analysis_closedset(const set<terminal> & ori_forward, const non_terminal & nt, unordered_map<non_terminal, ClosedSet> & closedset, CFG & cfg)
{
	ClosedSet & cs = cfg.__closed_set[cfg.__nonterminal_list[nt] - 1];
	set<terminal> this_forward(ori_forward);
	this_forward.insert(cs.inner_forward.begin(), cs.inner_forward.end());
	bool next = false;
	if (closedset.find(nt) == closedset.end())
	{
		ClosedSet temp = cs;
		temp.inner_forward = this_forward;
		closedset.insert(make_pair(nt, temp));
		next = true;
	}
	else
	{
		ClosedSet & temp = closedset[nt];
		for (terminal i : this_forward)
		{
			if (temp.inner_forward.insert(i).second)
			{
				next = true;
				break;
			}
		}
	}
	if (next)
	{
		unordered_map<non_terminal, set<terminal>> next_set;
		for (auto i : closedset[nt].right_value)
		{
			if ((!i.right_value.sym[0].second) && i.right_value.sym[0].first != nt)
			{
				if (next_set.find(i.right_value.sym[0].first) == next_set.end())
				{
					set<terminal> next_forward;
					if (i.right_value.sym.size() == 1)
					{
						next_forward.insert(this_forward.begin(),this_forward.end());
					}
					else
					{
						auto & t_set = cfg.getFirst(i.right_value.sym[1]);
						next_forward.insert(t_set.begin(), t_set.end());
					}
					next_set[i.right_value.sym[0].first] = next_forward;
				}
				else
				{
					set<terminal> & next_forward = next_set[i.right_value.sym[0].first];
					if (i.right_value.sym.size() == 1)
					{
						next_forward.insert("null");
					}
					else
					{
						auto & t_set = cfg.getFirst(i.right_value.sym[1]);
						next_forward.insert(t_set.begin(), t_set.end());
					}
				}
			}
		}
		for (const pair<non_terminal, set<terminal>> & it : next_set)
		{
			analysis_closedset(it.second, it.first, closedset, cfg);
		}
	}
}
void init_state(LR_Status & s, int i1, int i2)
{
	s.terminal_skip.resize(i1-1);
	s.non_terminal_skip.resize(i2 - 1);
	for (int i = 0; i < i1-1; ++i)
	{
		s.terminal_skip[i] = make_pair(ERROR, false);
	}
	for (int i = 0; i < i2 - 1; ++i)
	{
		s.non_terminal_skip[i] = ERROR;
	}
}
lr_skip LR_ParsingTable::next_step(CFG & cfg, const terminal & next_sym)
{
	const LR_Status & state = this->__state[cfg.state_stack[cfg.state_stack.size() - 1]];
	return state.terminal_skip[cfg.__terminal_list[next_sym] - 1];
}
int LR_ParsingTable::move_step(CFG & cfg, const non_terminal & next_sym)
{
	const LR_Status & state = this->__state[cfg.state_stack[cfg.state_stack.size() - 1]];
	return state.non_terminal_skip[cfg.__nonterminal_list[next_sym] - 1];
}