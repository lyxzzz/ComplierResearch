#ifndef M_SOPERATE_H
#define M_SOPERATE_H
#include"regex.h"
//���ע����ʵ�鱨���е���Ҫ���ݽṹ�н���
State* normal(Specific_symbol* _sp, State * _state, char _val);
State* nullfunc(Specific_symbol* _sp, State * _state, char _val);
State* initfunc(Specific_symbol* _sp, State * _state, char _val);
State* finfunc(Specific_symbol* _sp, State * _state, char _val);
State* asteriskfunc(Specific_symbol* _sp, State * _state, char _val);
State* orfunc(Specific_symbol* _sp, State * _state, char _val);
State* switchfunc(Specific_symbol* _sp, State * _state, char _val);
State* lparenthesis(Specific_symbol* _sp, State * _state, char _val);
State* rparenthesis(Specific_symbol* _sp, State * _state, char _val);
State* collectionfunc(Specific_symbol* _sp, State * _state, char _val);
State* connectfunc(Specific_symbol* _sp, State * _state, char _val);
State* transfunc(Specific_symbol* _sp, State * _state, char _val);
State* leftcollectfunc(Specific_symbol* _sp, State * _state, char _val);
State* rightcollectfunc(Specific_symbol* _sp, State * _state, char _val);
State* plusfunc(Specific_symbol* _sp, State * _state, char _val);
#endif


