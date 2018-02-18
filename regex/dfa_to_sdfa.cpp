#include"dfa.h"
bool DFA_State::operator== (const DFA_State * const a)
{
	if (this->end != a->end)
	{
		return false;
	}
	for (auto i : this->_next)
	{
		auto it = a->_next.find(i.first);
		if (it == a->_next.end())
		{
			return false;
		}
		else
		{
			if (it->second->_number != i.second->_number)
			{
				return false;
			}
		}
	}
	for (auto i : a->_next)
	{
		auto it = this->_next.find(i.first);
		if (it == this->_next.end())
		{
			return false;
		}
		else
		{
			if (it->second->_number != i.second->_number)
			{
				return false;
			}
		}
	}
	return true;
}
void DFA::simplify()
{
	unordered_map<int, DFA_State*> change;
	typedef vector<DFA_State*> my_list;
	my_list endlist;
	my_list temp;
	set<int> end_set;
	for (auto i : list)
	{
		if (i->end)
		{
			endlist.push_back(i);
			end_set.insert(i->_number);
		}
		bool repeat = false;
		for (auto it : temp)
		{
			if ((*it) == i)
			{
				change[i->_number] = it;
				repeat = true;
				break;
			}
		}
		if (!repeat)
		{
			temp.push_back(i);
		}
	}
	bool flag = true;
	set<char> direction;
	for (auto i : endlist)
	{
		bool roll = true;
		for (auto j : i->_next)
		{
			if (end_set.find(j.second->_number) == end_set.end())
			{
				roll = false;
				break;
			}
			direction.insert(j.first);
		}
		if (!roll)
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{
		for (auto i : endlist)
		{
			for (char var : direction)
			{
				if (i->_next.find(var) == i->_next.end())
				{
					flag = false;
					break;
				}
			}
			if (!flag)
			{
				break;
			}
		}
	}
	if (flag)
	{
		for (std::size_t i = 1; i < endlist.size(); ++i)
		{
			change[endlist[i]->_number] = endlist[0];
		}
		for (std::size_t i = 0; i < temp.size(); ++i)
		{
			if (!temp[i]->end)
			{
				s_list.push_back(temp[i]);
			}
		}
		s_list.push_back(endlist[0]);
	}
	else
	{
		s_list = temp;
	}
	for (auto i : s_list)
	{
		for each(auto var in i->_next)
		{
			auto find = change.find(var.second->_number);
			if (find != change.end())
			{
				i->_next[var.first] = find->second;
			}
		}
	}
	//std::cout << std::endl<< std::endl<< std::endl;
	//printSDFA();
	//std::cout << std::endl;
}
void DFA::printSDFA()
{
	printvector(this->s_list);
}
bool DFA::match(const string & ma)
{
	return find(ma, dfa_root);
}
bool DFA::find(const string & ma, const DFA_State * const now, int index) const
{
	if (std::size_t(index) >= ma.size())
	{
		return now->end;
	}
	auto it = now->_next.find(ma[index]);
	if (it != now->_next.end())
	{
		return find(ma, it->second, index + 1);
	}
	return false;
}
vector<pair<int,int>> DFA::group(const string & ma)
{
	vector<pair<int, int>> result;
	for (std::size_t i = 0; i < ma.size(); ++i)
	{
		int num = group_str(ma, dfa_root,i);
		if (num != -1)
		{
			result.push_back(make_pair(i, num));
		}
	}
	return result;
}
int DFA::group_str(const string & ma, const DFA_State * const now, int index) const
{
	if (now->end)
	{
		return 0;
	}
	else
	{
		if (std::size_t(index) >= ma.size())
		{
			return -1;
		}
		auto it = now->_next.find(ma[index]);
		if (it != now->_next.end())
		{
			int num = group_str(ma, it->second, index + 1);
			return num == -1 ? -1 : num + 1;
		}
		return -1;
	}
}
vector<pair<int, int>> DFA::greedy_group(const string & ma)
{
	using std::size_t;
	vector<pair<int, int>> result;
	size_t i = 0;
	while (i < ma.size())
	{
		int num = greedy_group_str(ma, dfa_root, i);
		if (num != -1)
		{
			result.push_back(make_pair(i, num));
			i += num;
		}
		else
		{
			++i;
		}
	}
	return result;
}
int DFA::greedy_group_str(const string & ma, const DFA_State * const now, int index) const
{
	if (std::size_t(index) > ma.size())
	{
		return now->end ? 1 : -1;
	}
	else
	{
		auto it = now->_next.find(ma[index]);
		if (it != now->_next.end())
		{
			int num = greedy_group_str(ma, it->second, index + 1);
			if (num == -1)
			{
				return it->second->end ? 1 : -1;
			}
			else
			{
				return num + 1;
			}
		}
		else
		{
			return -1;
		}
	}
}
int DFA::greedy_match(const string & m)
{
	using std::size_t;
	vector<pair<int, int>> result;
	int num = greedy_group_str(m, dfa_root,0);
	return num;
}