
#include <bits/stdc++.h>
using namespace std;

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