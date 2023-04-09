
#include <bits/stdc++.h>
using namespace std;

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
        map<string, tuple<string,string>> mymap;
		mymap["IF"]=(make_tuple("","0"));
		mymap["ID"]=(make_tuple("","0"));
        mymap["EX"]=(make_tuple("","0"));
        mymap["MEM"]=(make_tuple("","0"));
        mymap["WB"]=(make_tuple("","0"));
        std::map<std::string, tuple<string,string>>::iterator it = mymap.begin();
        mymap["IF"] = make_tuple("","1");
        if (get<1>mymap["IF"]=="1"){
          cout<<123<<endl;
        }
  // Iterate through the map and print the elements
  while (it != mymap.end())
  {
    cout << "Key: " << it->first << ", Value: "<< get<1>(it->second) << endl;
    ++it;
  }

    return 0;
}