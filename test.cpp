/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <bits/stdc++.h>

using namespace std;

bitset<2> ret(bitset<2> prev, bool taken){
	if (taken)
	{
		if ( not(prev.test(1)))
		{
			prev.set(1, true);
			return prev;
			cout<<"hello"<<prev.test(0)<<endl;
		}
		else if (prev.test(1))
		{
			prev.set(1,true);
		}
		else
		{
			prev.set(0, true);
			prev.set(1, false);
		}
	}

	else{
			if (prev.test(1) )
		{
			prev.set(1, false);
		}
		else if (prev.test(0))
		{
			prev.set(0, false);
			prev.set(1,true);
		}
		else
		{
			prev.set(0, false);
			prev.set(1, false);
		}
		}
		return prev;
}

int main()
{
	bitset<2> a("11");
	bitset<2> b("10");
	bitset<2> c("01");
	bitset<2> d("00");
	bool taken = true;
	bitset<2> prev= ret(a,taken);
	bitset<2> prev1= ret(b,taken);
	bitset<2> prev2= ret(c,taken);
	bitset<2> prev3= ret(d,taken);
	bitset<2> prev4= ret(a,false);
	bitset<2> prev5= ret(b,false);
	bitset<2> prev6= ret(c,false);
	bitset<2> prev7= ret(d,false);
	//  cout << "test() at index 0: " << noboolalpha
	//  << prev.test(1) << endl;
	cout<<prev.test(0)<<" "<<prev.test(1)<<endl;
	cout<<prev1.test(0)<<" "<<prev1.test(1)<<endl;
	cout<<prev2.test(0)<<" "<<prev2.test(1)<<endl;
	cout<<prev3.test(0)<<" "<<prev3.test(1)<<endl;
	cout<<prev4.test(0)<<" "<<prev4.test(1)<<endl;
	cout<<prev5.test(0)<<" "<<prev5.test(1)<<endl;
	cout<<prev6.test(0)<<" "<<prev6.test(1)<<endl;
	cout<<prev7.test(0)<<" "<<prev7.test(1)<<endl;

	return 0;
}
