
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
      if (ins[a].rs==ins[b].rd || ins[b].rd.find(ins[a].rt)!=string::npos){
		return 2;
	  }
	  else{
		return 0;
	  }
}
// 
int sw_hazard(int a,int b, instruct ( ins)[]){ // if rt of sw depends on i-1th instruction 
	  if (ins[b].rd.find(ins[a].rt)!=string::npos){
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
		ins[id[i]].type= command[0];
		if (command[0]=="bne" || command[0]=="beq"){
				ins[id[i]].rd=command[3];
				ins[id[i]].rs=command[1];
				ins[id[i]].rt=command[2];
				ins[id[i]].value=eval[id[i]];
			}
		else if (command[0]=="j"){
			ins[id[i]].rd=command[1];
			ins[id[i]].rs="";
			ins[id[i]].rt="";
			ins[id[i]].value=eval[id[i]];
		}
		else if (command[0]=="lw"){
			ins[id[i]].rd=command[1];
			ins[id[i]].rs=command[2];
			ins[id[i]].rt="";
			ins[id[i]].value=eval[id[i]];
		}

		else if (command[0]=="sw"){
			ins[id[i]].rs=command[1];
			ins[id[i]].rt=command[2];
			ins[id[i]].rd="";
			ins[id[i]].value=eval[id[i]];
		}

		else{
		ins[id[i]].rd=command[1];
		ins[id[i]].rs=command[2];
		ins[id[i]].rt=command[3]; 
		ins[id[i]].value=eval[id[i]];
		}
		for (int j=0; j<5; j++){
			(ins[id[i]].time).push_back(-1);
		}
	}
	ins[0].time={1,2,3,4,5};
	for(int i=1;i<m;i++){ 
		// remember to handle branch hazard
		if(ins[id[i]].type=="bne" || ins[id[i]].type=="beq" || ins[id[i]].type=="j"){
				(ins[id[i]].time)[0]=(ins[i-1].time)[2]+1;
				ins[id[i]].time[1]=ins[id[i]].time[0]+1;
				ins[id[i]].time[2]=ins[id[i]].time[1]+1;
				ins[id[i]].time[3]=ins[id[i]].time[2]+1;
				ins[id[i]].time[4]=ins[id[i]].time[3]+1;
		}
		else {
			if(ins[id[i]].type=="sw"){
				int y= sw_hazard(id[i],id[i]-1,ins);
				int z= 0; int x=0;
				if(id[i]>1){z=sw_hazard(id[i],id[i]-2,ins); x=ins[id[i]-2].time[3]+z;}
				ins[id[i]].time[0]=ins[id[i]-1].time[1];
				ins[id[i]].time[1]=ins[id[i]-1].time[2];
				ins[id[i]].time[2]= max((ins[id[i]-1].time[3]+y),x);
				ins[id[i]].time[3]=ins[id[i]].time[2]+1;
				ins[id[i]].time[4]=ins[id[i]].time[3]+1;
			}
			else{
				int y= hazard(id[i],id[i]-1,ins);
				int z= 0; int x=0;
				if(id[i]>1){z=hazard(id[i],id[i]-2,ins); x=ins[id[i]-2].time[3]+z;}
				(ins[id[i]].time)[0]=(ins[id[i]-1].time)[1];
				ins[id[i]].time[1]=ins[id[i]-1].time[2];
				ins[id[i]].time[2]= max((ins[id[i]-1].time[3]+y),x);
				ins[id[i]].time[3]=ins[id[i]].time[2]+1;
				ins[id[i]].time[4]=ins[id[i]].time[3]+1;
			}
		}
	}
	int s=ins[m-1].time[4];
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