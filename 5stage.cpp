
#include <bits/stdc++.h>

#include "MIPS_Processor.hpp"
using namespace std;
// indicator to know which stage is occupied or free
// when to remove the stall
//
struct instruct{
	string rs;
	string rt;
	string rd;
	string type;
	vector<int> time;
	vector<int> value;
};

int hazard(int a,int b, instruct ( ins)[]){ // if rt of sw depends on i-1th instruction 
      if ((ins[a].rs==ins[b].rd || (ins[a].rt.find(ins[b].rd)!=string::npos && ins[a].rt.find("$")!=string::npos)) && ins[b].rd!="") {
		return 2;
	  }
	  else{
		return 0;
	  }
}
// 
int sw_hazard(int a,int b, instruct ( ins)[]){ // if rt of sw depends on i-1th instruction 
	  if ((ins[a].rt.find(ins[b].rd)!=string::npos && ins[a].rt.find("$")!=string::npos) && ins[b].rd!=""){
		return 2;
	  }
	  else if (ins[a].rs==ins[b].rd){
		return 2;
	  }
	  else{
		return 0;
	  }
}


 void MIPS_Architecture::executeCommandspipelined(	vector<vector<vector<int>>> p)
{
	vector<vector<int>> eval= p[1];
	vector<int> id = p[0][0] ;
    int m=id.size();
	if (commands.size() >= MAX / 4)
	{
		handleExit(MEMORY_ERROR, 0);
		return;
	}
	int n= commands.size();
	instruct* ins= new instruct[m]; 
	for(int i=0;i<m;i++){
		vector<string> command= commands[id[i]];
		ins[i].type= command[0];
		if (command[0]=="bne" || command[0]=="beq"){
				ins[i].rd=command[3];
				ins[i].rs=command[1];
				ins[i].rt=command[2];
				ins[i].value=eval[i];
			}
		else if (command[0]=="j"){
			ins[i].rd=command[1];
			ins[i].rs="";
			ins[i].rt="";
			ins[i].value=eval[i];
		}
		else if (command[0]=="lw"){
			ins[i].rd=command[1];
			ins[i].rt=command[2];
			ins[i].rs="";
			ins[i].value=eval[i];
		}

		else if (command[0]=="sw"){
			ins[i].rs=command[1];
			ins[i].rt=command[2];
			ins[i].rd="";
			ins[i].value=eval[i];
		}

		else{
		ins[i].rd=command[1];
		ins[i].rs=command[2];
		ins[i].rt=command[3]; 
		ins[i].value=eval[i];
		}
		for (int j=0; j<5; j++){
			(ins[i].time).push_back(-1);
		}
	}
	ins[id[0]].time={1,2,3,4,5};
	for(int i=1;i<m;i++){ 
		// remember to handle branch hazard
		if(ins[i-1].type=="bne" || ins[i-1].type=="beq"){
				(ins[i].time)[0]=(ins[i-1].time)[3];
				ins[i].time[1]=ins[i].time[0]+1;
				ins[i].time[2]=ins[i].time[1]+1;
				ins[i].time[3]=ins[i].time[2]+1;
				ins[i].time[4]=ins[i].time[3]+1;
		}
		else if(ins[i-1].type=="j"){
				(ins[i].time)[0]=(ins[i-1].time)[2];
				ins[i].time[1]=ins[i].time[0]+1;
				ins[i].time[2]=ins[i].time[1]+1;
				ins[i].time[3]=ins[i].time[2]+1;
				ins[i].time[4]=ins[i].time[3]+1;
		}
		else {			
			int y= hazard(i,i-1,ins);
			int z= 0; int x=0;
			if(i>1){z=hazard(i,i-2,ins); x=ins[i-2].time[3]+z;}
			ins[i].time[0]=ins[i-1].time[1];
			ins[i].time[1]=ins[i-1].time[2];
			ins[i].time[2]= max((ins[i-1].time[3]+y),x);
			ins[i].time[3]=ins[i].time[2]+1;
			ins[i].time[4]=ins[i].time[3]+1;			
		}
	}
	int s=ins[m-1].time[4];
	vector<int> ex;
	for(int i=0;i<m;i++){
		if(ins[i].type=="sw"){ex.push_back(ins[i].time[3]);}
		else{ex.push_back(ins[i].time[4]);}
	}
	for(auto x: ex){cout<<x<<" ";} cout<<endl;
	int j=0;
	vector<int> cur(33,0);
	for(int t=0; t<=s;t++){
		if(ex[j]==t){
			cur= eval[j]; j++;
			for(int i=0;i<32;i++){
				cout<< cur[i]<<" ";
			}
			cout<<endl;		
			for(int i=32;i<cur.size();i++){
				cout<< cur[i]<<" ";
			}
			cout<<endl;
		}
		else{
			for(int i=0;i<32;i++){
				cout<< cur[i]<<" ";
			}
			cout<<endl;
			cout<<0<<endl;
		}
	}
	//print
	cout<<s<<endl;
	string t;
	t.append(1+s,'.');
	vector<string> pipe(m,t);
	for(int i=0;i<m;i++){
		for(int j=0;j<5;j++){
			pipe[i][ins[i].time[j]]='|';
		}
	}
	for(auto x: pipe){
		cout<<x<<endl;
	}
	//print
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Required argument: file_name\n./MIPS_interpreter <file name>\n";
		return 0;
	}
	std::ifstream file(argv[1]);
	MIPS_Architecture *mips;
	if (file.is_open())
		mips = new MIPS_Architecture(file);
	else
	{
		std::cerr << "File could not be opened. Terminating...\n";
		return 0;
	}
	vector<vector<vector<int>>> p= mips->executeCommandsUnpipelined();
	mips->executeCommandspipelined(p);
	return 0;
}