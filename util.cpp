#include"util.h"
#include<iostream>
using std::cout;
using std::endl;
void split(vector<string> & res, string str, const string & sym)
{
	int index = str.find(sym);
	while (index != string::npos)
	{
		if (index != 0)
		{
			res.push_back(str.substr(0, index));
		}
		str = str.substr(index + sym.size());
		index = str.find(sym);
	}
	if (str.size() > 0)
	{
		res.push_back(str);
	}
}