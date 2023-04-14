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
	bitset<2> a("11");
	bitset<2> b("10");
	bitset<2> c("01");
	bitset<2> d("00");
	bool taken = true;
	// cout<<a.test(1)<<endl;
	// cout<<b.test(1)<<endl;
	// cout<<c.test(1)<<endl;
	// cout<<d.test(1)<<endl;
	// bitset<2> prev= ret(a,taken);
	// bitset<2> prev1= ret(b,taken);
	// bitset<2> prev2= ret(c,taken);
	// bitset<2> prev3= ret(d,taken);
	// bitset<2> prev4= ret(a,false);
	// bitset<2> prev5= ret(b,false);
	// bitset<2> prev6= ret(c,false);
	// bitset<2> prev7= ret(d,false);
	// //  cout << "test() at index 0: " << noboolalpha
	// //  << prev.test(1) << endl;
	// cout<<prev1.to_string()<<" fdd "<<endl;
	// cout<<prev2.to_string()<<" fdd "<<endl;
	// cout<<prev3.to_string()<<" fdd "<<endl;
	// cout<<prev4.to_string()<<" fdd "<<endl;
	// cout<<prev5.to_string()<<" fdd "<<endl;
	// cout<<prev6.to_string()<<" fdd "<<endl;
	// cout<<prev7.to_string()<<" fdd "<<endl;
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

