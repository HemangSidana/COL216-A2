#ifndef _BRANCH_PREDICTOR_HPP_
#define _BRANCH_PREDICTOR_HPP_
#include <bits/stdc++.h>
#include <vector>
#include <bitset>
#include <cassert>
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


struct BranchPredictor {
    virtual bool predict(uint32_t pc) = 0;
    virtual void update(uint32_t pc, bool taken) = 0;
};

struct SaturatingBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> table;
    SaturatingBranchPredictor(int value) : table(1 << 14, value) {}
    bool predict(uint32_t pc) {
        uint32_t pc1=pc & 0x3fff;
        bitset<2> prev=table[pc1];
        if (prev.test(1)){ 
            return true;}
        else{ return false; }
    }

    void update(uint32_t pc, bool taken) {
        uint32_t pc1=pc & 0x3fff;
        bitset<2> prev=table[pc1];
        prev=ret(prev,taken);
        table[pc1]=prev;
    }
};

struct BHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhrTable;
    std::bitset<2> bhr;
    BHRBranchPredictor(int value) : bhrTable(1 << 2, value), bhr(value) {}

    bool predict(uint32_t pc) {
        uint32_t pc1=bhr.to_ulong();
        bitset<2> prev=bhrTable[pc1];
        if (prev.test(1)){ 
            return true;}
        else{ return false; }
    }

    void update(uint32_t pc, bool taken) {
        uint32_t pc1=bhr.to_ulong();
        bitset<2> prev=bhrTable[pc1];
        prev=ret(prev,taken);
        bhrTable[pc1]=prev;
        bhr <<= 1;
        bhr[0] = taken;
    }
};

struct SaturatingBHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhrTable;
    std::bitset<2> bhr;
    std::vector<std::bitset<2>> table;
    std::vector<std::bitset<2>> combination;
    SaturatingBHRBranchPredictor(int value, int size) : bhrTable(1 << 2, value), bhr(value), table(1 << 14, value), combination(size, value) {
        assert(size <= (1 << 16));
    }

    bool predict(uint32_t pc) {
        uint32_t pc1= concat(pc,bhr.to_ulong()) && 0xFFFF ;
        bitset<2> prev=combination[pc1];
        if (prev.test(1)){ 
            uint32_t pc2=pc & 0x3fff;
            bitset<2> pred=table[pc2];
            if (pred.test(1)){ 
                return true;}
            else{ return false; }
        }
        else{
            uint32_t pc2=bhr.to_ulong();
            bitset<2> pred=bhrTable[pc2];
            if (pred.test(1)){ 
                return true;}
            else{ return false; }
        }

    }

    void update(uint32_t pc, bool taken) {
        uint32_t pc1= concat(pc,bhr.to_ulong()) && 0xFFFF ;
        bitset<2> prev=combination[pc1];
        uint32_t pc2=bhr.to_ulong();
        bitset<2> prev2=bhrTable[pc2];
        uint32_t pc3=pc & 0x3fff;
        bitset<2> prev3=table[pc3];
        bitset<2> ph=prev2 ^ prev3;
        int gh=ph.to_ulong();
        int check1=1,check2=1,check3=1,check4=1;
        if(prev2.test(1) ^ taken){
            check1=0;
        }
        if(prev3.test(1) ^ taken){
            check2=0;
        }
        if (prev2.test(0) ^ taken){check3=0;}
        if (prev3.test(0) ^ taken){check4=0;}
        prev3=ret(prev3,taken);
        table[pc3]=prev3;
        prev2=ret(prev2,taken);
        bhrTable[pc2]=prev2;
        bhr <<= 1;
        bhr[0] = taken;
        if((!check1 && check2) || (check1==check2 && !check3 && check4)){
            for (int i=0; i<gh;i++){
                prev=ret(prev,1);
            }

        }
        if((check1 && !check2) || (check1==check2 && !check4 && check3)){
            for (int i=0; i<gh;i++){
                prev=ret(prev,0);
            }
        }
        combination[pc1]=prev;
    }
};



#endif