#include "specific_symbol.h"
#include "state_operate.h"
Specific_symbol* Specific_symbol::singleton = new Specific_symbol();
Specific_symbol* Specific_symbol::getInstance()
{
	return singleton;
}
Specific_symbol::Specific_symbol() :symbol(), number(0),_trans(false),_collection(false)
{
	this->_stack = new Specific_symbol::State_Stack();
	this->insert('^', initfunc);
	this->insert('$', finfunc);
	this->insert('(', lparenthesis);
	this->insert(')', rparenthesis);
	this->insert('|', orfunc);
	this->insert('*', asteriskfunc);
	this->insert('?', switchfunc);
	this->insert('\\', transfunc);
	this->insert('[', leftcollectfunc);
	this->insert('-', connectfunc);
	this->insert(']', rightcollectfunc);
	this->insert('+', plusfunc);
}
Specific_symbol::~Specific_symbol() {}
void Specific_symbol::insert(char sym, func p)
{
	symbol[sym] = p;
}
func Specific_symbol::find(char sym)
{
	if (_trans)
	{
		_trans = false;
		return normal;
	}
	else if (_collection)
	{
		if (sym != ']'&&sym != '-')
		{
			return collectionfunc;
		}
		else
		{
			return symbol.find(sym)->second;
		}
	}
	else
	{
		auto it = symbol.find(sym);
		return it != symbol.end() ? it->second : normal;
	}
}
void Specific_symbol::clear()
{
	this->number = 0;
	_trans = false;
	_collection = false;
	this->_stack = new Specific_symbol::State_Stack();
}
State* Specific_symbol::getStart()
{
	return this->_stack->start.top();
}