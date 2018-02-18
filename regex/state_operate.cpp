#include"state_operate.h"
#include<utility>
using std::make_pair;
State* normal(Specific_symbol* _sp, State * _state, char _val)
{
	State * temp = new State(_sp->number++, false, false);
	State * p = new State(_sp->number++,false, false);
	_state->next.insert(make_pair(char(0), temp));
	temp->next.insert(make_pair(_val, p));
	_sp->_stack->push(temp);
	_sp->_stack->push(p);
	return p;
}
State* nullfunc(Specific_symbol* _sp, State * _state, char _val)
{
	return _state;
}
State* initfunc(Specific_symbol* _sp, State * _state, char _val)
{
	State * start = new State(_sp->number++, true, false);
	State * temp = new State(_sp->number++, false, false);
	State * end = new State(_sp->number++, false, true);
	_sp->_stack->push(start, end);
	_sp->_stack->push(start);
	start->next.insert(make_pair(char(0), temp));
	_sp->_stack->push(temp);
	return temp;
}
State* asteriskfunc(Specific_symbol* _sp, State * _state, char _val)
{
	auto it = _sp->_stack->get();
	it.first->next.insert(make_pair(char(0), it.second));
	_sp->_stack->push(it.second);
	return it.second;
}
State* orfunc(Specific_symbol* _sp, State * _state, char _val)
{
	State * start = _sp->_stack->start.top();
	State * end = _sp->_stack->end.top();
	_state->next.insert(make_pair(char(0), end));
	_sp->_stack->push(start);
	State * temp = new State(_sp->number++, false, false);
	start->next.insert(make_pair(char(0), temp));
	_sp->_stack->push(temp);
	return temp;
}
State* lparenthesis(Specific_symbol* _sp, State * _state, char _val)
{
	State * start = new State(_sp->number++, false, false);
	_state->next.insert(make_pair(char(0), start));
	State * temp = new State(_sp->number++, false, false);
	State * end = new State(_sp->number++, false, false);
	_sp->_stack->push(start, end);
	_sp->_stack->push(start);
	start->next.insert(make_pair(char(0), temp));
	_sp->_stack->push(temp);
	return temp;
}
State* rparenthesis(Specific_symbol* _sp, State * _state, char _val)
{
	State * start = _sp->_stack->start.top();
	_sp->_stack->start.pop();
	State * end = _sp->_stack->end.top();
	_sp->_stack->end.pop();
	_state->next.insert(make_pair(char(0), end));
	_sp->_stack->push(start);
	_sp->_stack->push(end);
	return end;
}
State* finfunc(Specific_symbol* _sp, State * _state, char _val)
{
	State * start = _sp->_stack->start.top();
	_sp->_stack->start.pop();
	State * end = _sp->_stack->end.top();
	_sp->_stack->end.pop();
	_state->next.insert(make_pair(char(0), end));
	_sp->_stack->push(start);
	_sp->_stack->push(end);
	_sp->clear();
	return end;
}
State* switchfunc(Specific_symbol* _sp, State * _state, char _val)
{
	State * temp = new State(_sp->number++, false, false);
	auto it = _sp->_stack->get();
	it.first->next.insert(make_pair(char(0), temp));
	it.second->next.insert(make_pair(char(0), temp));
	_sp->_stack->push(temp);
	return temp;
}
State* collectionfunc(Specific_symbol* _sp, State * _state, char _val)
{
	if (_sp->_stack->connect)
	{
		for (char i = _sp->_stack->range[_sp->_stack->range.size() - 1] + 1; i <= _val; ++i)
		{
			_sp->_stack->range.push_back(i);
		}
		_sp->_stack->connect = false;
	}
	else
	{
		_sp->_stack->range.push_back(_val);
	}
	return _state;
}
State* connectfunc(Specific_symbol* _sp, State * _state, char _val)
{
	_sp->_stack->connect = true;
	return _state;
}
State* transfunc(Specific_symbol* _sp, State * _state, char _val)
{
	_sp->_trans = true;
	return _state;
}
State* leftcollectfunc(Specific_symbol* _sp, State * _state, char _val)
{
	_sp->_collection = true;
	State * temp = new State(_sp->number++, false, false);
	_state->next.insert(make_pair(char(0), temp));
	_sp->_stack->push(temp);
	return temp;
}
State* rightcollectfunc(Specific_symbol* _sp, State * _state, char _val)
{
	State * temp = new State(_sp->number++, false, false);
	_sp->_stack->push(temp);
	for (char i : _sp->_stack->range)
	{
		_state->next.insert(make_pair(i, temp));
	}
	_sp->_collection = false;
	return temp;
}
State* plusfunc(Specific_symbol* _sp, State * _state, char _val)
{
	auto it = _sp->_stack->get();
	it.first->next.insert(make_pair(char(0), it.second));
	return _state;
}