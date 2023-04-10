#include "MIPS_Processor.hpp"
#include <bits/stdc++.h>
using namespace std;



struct instruct{
	// start cycle of IF,ID,EX,MEM,WB
	vector<int> cycle;
	string sig;
	string ins;
	string rs;
	string rd;
	string rt;

};

struct instruct ins[1000000];

string signal(string p){
	if (p == "sw"){
		return "MR";
		}
	else if (p == "beq" || p == "j" || p == "bne"){
		return "NA";
	}
	else{
		return "RW";
	}
}


//functions
int hazard_ex(int i){
	if(i>0 && ins[i-1].cycle[3]<=i && (ins[i].rd==ins[i-1].rs || ins[i].rd==ins[i-1].rt)){
		return 1;
	}
	else if(i>1 && ins[i-2].cycle[4]<=i && (ins[i].rd==ins[i-2].rs || ins[i].rd==ins[i-2].rt)){
		return 1;
	}
	else{
		return 0;
	}
}

int hazard_mem(int i){
	if(i>0 && ins[i-1].cycle[3]<=i && (ins[i].rs==ins[i-1].rd || ins[i].rs==ins[i-1].rd)){
		return 1;
	}
	else{
		return 0;
	}
}

int hazard_ex2(int i){
	if(i>0 && ins[i-1].cycle[3]<=i && (ins[i].rd==ins[i-1].rd)){
		return 1;
	}
	else{
		return 0;
	}
}

//functions


 void MIPS_Architecture::executeCommandspipelined(){

	int n=commands.size();
	if (n >= MAX / 4){
		handleExit(MEMORY_ERROR, 0);
		return;
	}
	int counter= -1;
	// instruction index corresponding to IF,ID,EX,MEM,WB
	int IF,ID,EX,MEM,WB=-1;
	// 1 means available and 0 means structural hazard
	vector<int> avail(5,1); 
	int cycles= 0;
	
	for(int i=0;i<n;i++){
		vector<string> command = commands[i];
		ins[i].ins = command[0];
		ins[i].rs = command[1];
		ins[i].rt = command[2];
		ins[i].rd = command[3];
		ins[i].sig = signal(command[0]);
		for(int j=0;j<5;j++){
			ins[i].cycle.push_back(-1);
		}
	}

	while(counter<n){
		cycles++;
		if(avail[4] && !avail[3]){
			WB=counter;
			ins[counter].cycle[4]=WB;
			if(counter==n-1){break;}
		}
		if(avail[3] && !avail[2]){
			if(ins[counter].sig=="MR" && hazard_mem(counter)){
				avail[3]=0;
			}
			else{
				MEM=counter; ins[counter].cycle[3]=MEM; avail[3]=1;
			}
		}
		if(avail[2] && !avail[1]){
			if(ins[counter].sig=="RW" && hazard_ex(counter)){
				avail[2]=0;
			}
			else if(ins[counter].sig=="MR" && hazard_ex2(counter)){
				avail[2]=0;
			}
			else{
				EX=counter; ins[counter].cycle[2]=EX; avail[2]=1;
			}
		}
		if(avail[1] && !avail[0]){
			ID=counter;
			ins[counter].cycle[1]=ID;
		}	
		if(avail[0]){
			counter++;
			IF=counter;
			ins[counter].cycle[0]=IF;
		}
	}
	cout<<cycles<<endl;
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
	// executeCommandspipelined(mips);
	mips->executeCommandspipelined();
	return 0;
}

