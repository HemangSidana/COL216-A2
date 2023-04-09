
#include <bits/stdc++.h>
using namespace std;
// indicator to know which stage is occupied or free
// when to remove the stall 
//
struct InstructionFetch(){
   string sig;
      string  ins;
      string rs;
      string rd;
      string rt;
};

struct InstructionDecode
{
  string sig;
      string  ins;
      string rs;
      string rd;
      string rt;
};

struct Execute(){
  string sig;
      string  ins;
      string rs;
      string rd;
      string rt;
};
struct Memory(){
 string sig;
      string  ins;
      string rs;
      string rd;
      string rt;
};

struct Writeback {
      string sig;
      string  ins;
      string rs;
      string rd;
      string rt;
};

int detect_EX_hazard(Execute a, InstructionDecode b){
     if (a.sig=="RegWrite" && (a.rd!="$0") && (a.rd==b.rs)){
      return 1;
     }
     else if (a.sig=="RegWrite" && (a.rd!="$0") && (a.rd==b.rt)){
      return 1;
     }
     else{
      return 0;
     }
}

int detect_Mem_hazard(Memory a, InstructionDecode b){
    if (a.sig=="RegWrite" && (a.rd!="$0") && (a.rd==b.rs)){
      return 1;
    }
    else if ((a.sig=="RegWrite" && (a.rd!="$0") && (a.rd==b.rt))){
      return 1;
    }
    else{
      return 0;
    }
}

int detect_lw_hazard(InstructionDecode a,InstructionFetch b){
   rs=a.rs;
   rt=a.rt;
   rd=a.rd;
   rs1=b.rs; rt1=b.rt; rd1=b.rd;
   if (a.sig=="lw" && ((rt==rs1) || (rt==rt1))){
    return 1;
   }
   else{
    return 0;
   }
}


int main(){

    return 0;
}