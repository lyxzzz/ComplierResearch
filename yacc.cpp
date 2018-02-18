#include"yacc.h"
using std::ifstream;
Lex Yacc::lex("lex/mylex.lex");
Yacc::Yacc(const string & path):path("output/"),filename(path)
{
	ifstream my_cfg;
	my_cfg.open(filename);
	vector<string> input1;
	vector<string> input2;
	string temp;
	int trans = true;
	while (!my_cfg.eof())
	{
		std::getline(my_cfg, temp);
		if (temp == "$$")
		{
			trans = false;
			continue;
		}
		if (trans)
		{
			if (temp != "")
			{
				input1.push_back(temp);
			}
		}
		else
		{
			input2.push_back(temp);
		}

	}
	ofstream out;
	out.open(this->path + "table_" + filename.substr(filename.find_last_of('/') + 1));
	__cfg = new CFG(out, input1, input2);
	out.close();
	std::cout << "analysis CFG success!" << std::endl;
}
void Yacc::analysis(const string & path)
{
	vector<Token> token;
	lex.analysis(path, token);
	ofstream out;
	out.open(this->path + "result_" + filename.substr(filename.find_last_of('/') + 1));
	__cfg->analysis(out, token);
	std::cout << "analysis String success!" << std::endl;
}