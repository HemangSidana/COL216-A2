#include <bits/stdc++.h>
#include "BranchPredictor.hpp"

using namespace std;

int you(string s)
{
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << s;
	ss >> x;
	return x;
}

int main()
{
	SaturatingBHRBranchPredictor Pred(3,65536);
	// Pred(0);
	std::ifstream file("branchtrace.txt");
	string str;
	int count = 0;
	int accuracy = 0;
	while (getline(file, str))
	{
		string c = "";
		string d = "";
		for (int i = 0; i < 8; i++)
		{
			c += str[i];
		}
		d += str[9];
		int y = you(d);
		int x = you(c);
		bool b = Pred.predict(x);
		// cout<<str<<endl;
		if (b == y)
		{
			accuracy++;
		}
		count++;
		Pred.update(x, y);
		// cout<<count<<endl;
	}
	cout << count << " " << accuracy << endl;
	return 0;
}