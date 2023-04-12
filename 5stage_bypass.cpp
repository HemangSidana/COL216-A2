
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

// 
int lw_hazard(int a,int b, instruct ( ins)[]){ // if rt of sw depends on i-1th instruction 
    if ((ins[a].rs==ins[b].rd || (ins[b].rd.find(ins[a].rt)!=string::npos && ins[a].rt.find("$")!=string::npos)) && ins[b].rd!="" ) {
            return 1;
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
	   cout<<"Hello"<<endl;
		for (int j=0; j<5; j++){
			(ins[i].time).push_back(-1);
		}
	}
	ins[id[0]].time={1,2,3,4,5};
	for(int i=1;i<m;i++){ 
		// remember to handle branch hazard
		if(ins[i-1].type=="bne" || ins[i-1].type=="beq" || ins[i-1].type=="j"){
				(ins[i].time)[0]=(ins[i-1].time)[2]+1;
				ins[i].time[1]=ins[i].time[0]+1;
				ins[i].time[2]=ins[i].time[1]+1;
				ins[i].time[3]=ins[i].time[2]+1;
				ins[i].time[4]=ins[i].time[3]+1;
		}
		else {
			if(ins[i-1].type=="lw" && lw_hazard(i,i-1,ins)){
				
				(ins[i].time)[0]=(ins[i-1].time)[1];
				ins[i].time[1]=ins[i-1].time[2];
				ins[i].time[2]= ins[i-1].time[3]+1;
				ins[i].time[3]=ins[i].time[2]+1;
				ins[i].time[4]=ins[i].time[3]+1;
			}
			else{
				(ins[i].time)[0]=(ins[i-1].time)[1];
				ins[i].time[1]=ins[i-1].time[2];
				ins[i].time[2]= ins[i-1].time[3];
				ins[i].time[3]=ins[i-1].time[4];
				ins[i].time[4]=ins[i].time[3]+1;
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