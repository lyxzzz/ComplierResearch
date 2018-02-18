#include<unordered_map>
#include<string>
#include<stack>
#include"regex.h"
#include<iostream>
#include<utility>
#ifndef M_SPECIFIC_H
#define M_SPECIFIC_H
#include<map>
#include<vector>
//相关注释在实验报告中的重要数据结构中解释
using std::multimap;
using std::string;
using std::vector;
struct State
{
public:
	bool start;
	bool end;
	int num;
	multimap<char, State*> next;
	State(int _n, bool _s, bool _e) :num(_n), start(_s), end(_e), next() {};
	~State() {};
};
using std::stack;
using std::unordered_map;
using std::string;
using std::pair;
using std::make_pair;
class Specific_symbol;
typedef State* (*func)(Specific_symbol*,State *, char val);
class Specific_symbol
{
private:
	struct State_Stack
	{
		stack<State*> start;
		stack<State*> end;
		vector<char> range;
		bool connect;
		State* second;
		State* first;
		State_Stack() :start(), end(), second(nullptr), first(nullptr),range(),connect(false) {}
		void push(State* _s) { second = first; first = _s; };
		void push(State* _s, State* _e) { start.push(_s); end.push(_e); };
		pair<State*, State*> get() { return make_pair(first, second); };
		~State_Stack() 
		{	delete second; 
			delete first;
			while (!start.empty())
			{
				delete start.top();
				delete end.top();
				start.pop();
				end.pop();
			}
		}
	};
	int number;
	static Specific_symbol* singleton;
	bool _trans;
	bool _collection;
	unordered_map<char, func> symbol;
	State_Stack* _stack;
	Specific_symbol();
	Specific_symbol(const Specific_symbol&) = delete;
	Specific_symbol & operator = (const Specific_symbol&) = delete;
	void clear();
public:
	static Specific_symbol* getInstance();
	~Specific_symbol();
	void insert(char, func);
	func find(char);
	State* getStart();
	friend State* initfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* normal(Specific_symbol* _sp, State * _state, char _val);
	friend State* asteriskfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* orfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* lparenthesis(Specific_symbol* _sp, State * _state, char _val);
	friend State* rparenthesis(Specific_symbol* _sp, State * _state, char _val);
	friend State* finfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* switchfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* collectionfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* connectfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* transfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* leftcollectfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* rightcollectfunc(Specific_symbol* _sp, State * _state, char _val);
	friend State* plusfunc(Specific_symbol* _sp, State * _state, char _val);
};
#endif
