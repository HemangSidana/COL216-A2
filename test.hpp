#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <bits/stdc++.h>
using namespace std;

int getBinaryLength(int n)
{
    int length = 0;
    while (n > 0) {
        length += 1;
        n /= 2;
    }
    return length;
}
int concat(int m, int n)
{
    // find binary length of n
    int length = getBinaryLength(n);
 
    // left binary shift m and then add n
    return (m << length) + n;
}

bitset<2> ret(bitset<2> prev, bool taken){
	if (taken)
	{
		if ( prev.to_ulong()==2)
		{
			prev.set(1);
			prev.set(0);
			return prev;
		}
		else if (prev.to_ulong()==3)
		{
			return prev;
		}
        else if (prev.to_ulong()==1)
		{
			prev.reset(0);
			prev.set(1);
			return prev;
		}
		else
		{
			prev.set(0);
			prev.reset(1);
			return prev;
		}
	}

	else{
		if (prev.to_ulong()==3)
		{
			prev.reset(0);
			return prev;
		}
		else if (prev.to_ulong()==2)
		{
			prev.reset(1);
			prev.set(0);
			return prev;
		}
		else
		{
			prev.reset(0);
			prev.reset(1);
			return prev;
		}
		}
		
}

#endif