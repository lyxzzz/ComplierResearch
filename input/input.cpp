#include<iostream>
int main()
{
	double temp1 = 3 + 5 * 6 - 7 / 8;
	int i1, i2, i3;
	int i4 = i1 = i2 = i3 = 3 - 7 * (5 + 6);
	for (int i = 0; i < 10; ++i)
	{
		if (i < 5)
		{
			std::cout << i << endl;
		}
		else
		{
			std::cout << i + 5 << endl;
		}
	}
}