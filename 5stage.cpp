
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

int hazard(int a,int b, instruct ( ins)[]){
      if (ins[a].rs==ins[b].rd || ins[a].rt==ins[b].rd){
		if(ins[a].type=="sw"){return 1;}
		return 2;
	  }
	  else{
		return 0;
	  }
}


 void MIPS_Architecture::executeCommandspipelined()
{
	vector<vector<int>> z= mips->executeCommandsUnpipelined();

	if (commands.size() >= MAX / 4)
	{
		handleExit(MEMORY_ERROR, 0);
		return;
	}
	int n= commands.size();
	instruct* ins= new instruct[n]; 
	for(int i=0;i<n;i++){
		vector<string> command= commands[i];
		ins[i].type= command[0];
		if (command[0]=="bne" || command[0]=="beq"){
				ins[i].rd=command[3];
				ins[i].rs=command[1];
				ins[i].rt=command[2];
			}
		else if (command[0]=="j"){
			ins[i].rd=command[1];
			ins[i].rs="";
			ins[i].rt="";
		}
		else if (command[0]=="lw"){
			ins[i].rd=command[1];
			ins[i].rs=command[2];
			ins[i].rt="";
		}

		else if (command[0]=="sw"){
			ins[i].rs=command[1];
			ins[i].rt=command[2];
			ins[i].rd="";
		}

		else{
		ins[i].rd=command[1];
		ins[i].rs=command[2];
		ins[i].rt=command[3]; 
		}
		for (int j=0; j<5; j++){
			(ins[i].time).push_back(-1);
		}
	}
	ins[0].time={1,2,3,4,5};
	for(int i=1;i<n;i++){ // remember to handle branch hazard
		if(ins[i].type=="bne" || ins[i].type=="beq" || ins[i].type=="j"){
			
			continue;
		}
		int y= hazard(i,i-1,ins);
		int z= 0; int x=0;
		if(i>1){z=hazard(i,i-2,ins); x=ins[i-2].time[3]+z;}
		if(ins[i].type="sw"){
			ins[i].time[0]=ins[i-1].time[1];
			ins[i].time[1]=ins[i-1].time[2];
			ins[i].time[2]=ins[i-1].time[3];
			ins[i].time[3]=(ins[i-1].time[4]+y);
			ins[i].time[4]=ins[i].time[3]+1;
			continue;
		}
		(ins[i].time)[0]=(ins[i-1].time)[1];
		ins[i].time[1]=ins[i-1].time[2];
		ins[i].time[2]= max((ins[i-1].time[3]+y),x);
		ins[i].time[3]=ins[i].time[2]+1;
		ins[i].time[4]=ins[i].time[3]+1;
	}
	cout<<ins[n-1].time[4];
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
	
	mips->executeCommandspipelined();
	return 0;
}