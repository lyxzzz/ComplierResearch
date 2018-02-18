#include"lex.h"
#include<fstream>
#include<Windows.h>
using std::ifstream;
using std::ofstream;
using std::wstring;
using std::cout;
using std::endl;
using std::getline;
Lex::Lex(const string & path) :list(), replace("{([A-Za-z])*}", "replace"), checklist()
{
	ifstream file;
	file.open(path);
	string str;
	list.push_back(Regex(" |\t|\n", "delim"));
	int type = 0;
	while (!file.eof())
	{
		getline(file, str);
		if (str == "$$")
		{
			++type;
			continue;
		}
		switch (type)
		{
		case 0:
		{
			int index = str.find_first_of('\t');
			string pattern = str.substr(index + 1, str.size());
			string name = str.substr(0, index);
			handle_str(pattern);
			if (name[0] != '\0')
			{
				//cout << name << ":" << pattern << endl;
				list.push_back(Regex(pattern, name));
			}
		}
		break;
		case 1:
		{
			int index = str.find_first_of('\t');
			string pattern = str.substr(0, index);
			string name = str.substr(index + 1, str.size());
			if (pattern[0] == '{')
			{
				pattern=pattern.substr(1, pattern.size() - 2);
				for (auto i : list)
				{
					if (i.getName() == pattern)
					{
						if (name == "DEFAULT")
						{
							default_regex = new Regex(i.getPattern(), i.getName());
						}
						else
						{
							checklist.push_back(make_pair(i, name));
						}
						break;
					}
				}
			}
			else
			{
				if (name == "DEFAULT")
				{
					default_regex = new Regex(pattern, name);
				}
				else
				{
					checklist.push_back(make_pair(Regex(pattern, name), name));
				}
			}
		}
		break;
		}	
	}
}
void Lex::handle_str(string & str)
{
	using std::size_t;
	using std::endl;
	vector<pair<int, int>> list = replace.group(str);
	if (list.size() == 0)
	{
		return;
	}
	size_t start = 0;
	size_t length = str.size();
	string temp;
	size_t index = 0;
	while (start < length)
	{
		if (index >= list.size())
		{
			temp = temp + str.substr(start);
			start = length;
		}
		else
		{
			auto i = list[index++];
			string name = str.substr(i.first, i.second);
			name = name.substr(1, name.size() - 2);
			string pattern;
			for (auto reg : this->list)
			{
				if (reg.getName() == name)
				{
					pattern = reg.getPattern();
					break;
				}
			}
			pattern = '(' + pattern + ')';
			temp = temp + str.substr(start, i.first - start) + pattern;
			start = i.first + i.second;
		}
	}
	str = temp;
}
void Lex::analysis(const string & path, vector<Token> & result) const
{
	using std::size_t;
	ifstream file;
	file.open(path);
	string str;
	while (!file.eof())
	{
		getline(file, str);
		size_t start = 0;
		string word;
		size_t index = 0;
		while (start < str.size())
		{
			string p = str.substr(start);
			//cout  << p <<endl;
			pair<int, string> temp(default_regex->greedy_match(p), "DEFAULT");
			for (auto it : checklist)
			{
				int i = it.first.greedy_match(p);
				if (i > temp.first)
				{
					temp = make_pair(i, it.second);
				}
			}
			if (temp.first <= 0)
			{
				break;
			}
			if (temp.second != "DEFAULT")
			{
				result.push_back(Token(temp.second, p.substr(0, temp.first)));
			}
			start = start + temp.first;
		}
	}
	for (auto i : result)
	{
		cout << i;
	}
	ofstream out;
	out.open(path + ".output",std::ios::ate);
	for (auto i : result)
	{
		out << i;
	}
	out.close();
}
std::ostream & operator<<(std::ostream & out,const Token & t)
{
	out << "{" << t.typeName << ":\t" << t.value << "}" << std::endl;
	return out;
}
bool Lex::isTerminal(const string & str) const
{
	for (auto i : this->checklist)
	{
		if (i.second == str)
		{
			return true;
		}
	}
	return false;
}