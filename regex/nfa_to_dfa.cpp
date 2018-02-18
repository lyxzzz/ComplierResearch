#include"dfa.h"
void e_set(DFA_State * set)
{
	nfaset temp = set->_unmap;
	for (auto it = temp.begin(); it != temp.end(); ++it)
	{
		e_find(it->second, set);
	}
}
void e_find(State* state, DFA_State * set)
{
	for (auto i = state->next.begin(); i != state->next.end(); ++i)
	{
		if (i->first != 0)
		{
			set->direction.insert(i->first);
		}
	}
	auto it = state->next.find(char(0));
	if (it != state->next.end())
	{
		auto range = state->next.equal_range(char(0));
		for (auto i = range.first; i != range.second; ++i)
		{
			if (i->second->end)
			{
				set->end = true;
			}
			if (set->_unmap.insert(make_pair(i->second->num, i->second)).second)
			{
				e_find(i->second, set);
			}
		}
	}
}
void move(char a,DFA_State * from,DFA_State * to)
{
	for (auto i = from->_unmap.begin(); i != from->_unmap.end(); ++i)
	{
		auto range = i->second->next.equal_range(a);
		for (auto j = range.first; j != range.second; ++j)
		{
			to->insert(j->second);
		}
	}
}
void DFA_State::insert(State* s)
{
	this->_unmap.insert(make_pair(s->num, s));
}
bool DFA_State::operator== (const DFA_State & a)
{
	for (auto it = this->_unmap.begin(); it != this->_unmap.end(); ++it)
	{
		if (a._unmap.find(it->first) == a._unmap.end())
		{
			return false;
		}
	}
	for (auto it = a._unmap.begin(); it != a._unmap.end(); ++it)
	{
		if (this->_unmap.find(it->first) == this->_unmap.end())
		{
			return false;
		}
	}
	return true;
}
void DFA::trans(State* root)
{
	DFA_State * r = new DFA_State(this->dfa_num++);
	r->insert(root);
	e_set(r);
	this->list.push_back(r);
	dfa_root = r;
	for (std::size_t index = 0; index < this->list.size(); ++index)
	{
		DFA_State* p = list[index];
		for (auto i = p->direction.begin(); i != p->direction.end(); ++i)
		{
			DFA_State * temp = new DFA_State(this->dfa_num++);
			move(*i, p, temp);
			e_set(temp);
			bool repeat = false;
			for (auto j : this->list)
			{
				if ((*j) == (*temp))
				{
					delete temp;
					temp = j;
					repeat = true;
					--this->dfa_num;
					break;
				}
			}
			if (!repeat)
			{
				this->list.push_back(temp);
			}
			p->_next.insert(make_pair(*i, temp));
		}
	}
	//printDFA();
}
void DFA::printDFA()
{
	printvector(this->list);
}
void printvector(const vector<DFA_State*> & list)
{
	std::cout << list.size() << std::endl;
	using std::cout;
	using std::endl;
	for (auto i : list)
	{
		cout << "num : " << i->_number << "\tstate:" << (i->end ? "stop" : "go") << "  : {";
		for (auto j : i->_unmap)
		{
			cout << j.first << ",";
		}
		cout << "}";
		cout << "\n\r";
		for (auto j : i->_next)
		{
			cout << j.first << ":" << j.second->_number << "\t";
		}
		cout << endl << "******************************************************" << endl;
	}
}
