#ifndef H_TYPE_DEFINE
#define H_TYPE_DEFINE
#include<string>
#include<vector>
#include<unordered_set>
using std::unordered_set;
#include"regex\regex.h"
#include"regex\lex.h"
#include<fstream>
using std::ofstream;
using std::vector;
using std::ostream;
using std::string;
using std::basic_string;
using std::pair;
typedef string terminal; //�ս����
typedef string non_terminal; //���ս����
typedef pair<string,bool> symbol; //����(trueΪ�ս���ţ�falseΪ���ս�)
typedef pair<int,bool> lr_skip;//lr��������ÿһ�������ԣ�boolֵΪ�Ƿ��Լ��intֵΪ��ת��·����
typedef pair<int, bool> op_property;//�����������ȼ�(boolΪ�Ƿ�������)
const int ERROR = -1;//�����·��
const string split_symbol = "->";//�ָ��ַ�
struct Formula
{
	vector<symbol> sym;
	Formula(const string & _val);
};
struct Generation
{
	non_terminal left_value;//����ʽ��ֵ ΪE->T*B�е�E
	Formula right_value;//����ʽ�Ҷ� �� E * E��
	int number;//����ʽ��
	Generation(const non_terminal & _l,const string & _r, int _num) :left_value(_l), right_value(_r), number(_num) {}
	friend ostream & operator << (ostream & out, const Generation & g);
};
struct First
{
	bool use;//����������ն˷��ŵ�first�����Ƿ�̽��
	non_terminal val;//���ն˷���
	set<non_terminal> nonterminal_set;//�÷��ն˷���first�����еķ��ն˷���
	set<terminal> terminal_set;//�÷��ն˷��ŵ�first����
	First() :val(), nonterminal_set(), terminal_set(),use(false) {}
	First(const non_terminal & _val) :val(_val), nonterminal_set(), terminal_set(), use(false) {}
	bool insert(const non_terminal & value);
};
struct ClosedSet
{
	set<terminal> inner_forward;//һ�����ն˷��ŵıպϼ��в�������ǰ���ַ�
	non_terminal val;//���ն˷���
	vector<Generation> right_value;//�÷��ն˷��Ŷ�Ӧ�����е�����ʽ
	set<symbol> directions;//����
	ClosedSet(const non_terminal & _val = "") :val(_val), right_value(), directions(), inner_forward() {};
	void push_back(const Generation & t) 
	{ 
		right_value.push_back(t);
		directions.insert(t.right_value.sym[0]);
	}
};
bool isTerminal(const string & t);
#endif
