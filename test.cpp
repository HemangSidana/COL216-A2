
#include <bits/stdc++.h>

#include "MIPS_Processor.hpp"
using namespace std;
// indicator to know which stage is occupied or free
// when to remove the stall
//
struct stage
{
	int id = -1;
	string sig;
	string ins;
	string rs;
	string rd;
	string rt;
};


int detect_EX_hazard(stage a,  stage b)
{
	if (a.id<0 || b.id<0){return 0;}
	if (a.sig == "RW" && (a.rd == b.rs))
	{
		return 1;
	}
	else if (a.sig == "RW" && (a.rd == b.rt))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

int detect_Mem_hazard(stage a, stage b)
{  
	if (a.id<0 || b.id<0){return 0;}
	if (a.sig == "MR" && (a.rd == b.rs))
	{
		return 1;
	}
	else if ((a.sig == "MR" && (a.rd == b.rt)))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

int detect_lw_hazard( stage a, stage b)
{
	string rs = a.rs;
	string rt = a.rt;
	string rd = a.rd;
	string rs1 = b.rs;
	string rt1 = b.rt;
    string rd1 = b.rd;
	if (a.sig == "lw" && ((rt == rs1) || (rt == rt1)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

string signal(string instruct)
{
	if (instruct == "lw")
	{
		return "MR";
	}
	else if (instruct == "beq" || instruct == "j" || instruct == "bne")
	{
		return "NA";
	}
	else
	{
		return "RW";
	}
}

void assign(stage &a, stage &b){  
	a.id=b.id;
	a.sig=signal(b.ins); // I think we need to flush rs,rd,rt of b because we are checking for the MEM stage
	a.ins=b.ins;
	a.rs=b.rs;
	a.rd=b.rd;
	a.rt=b.rt;
	b.id=-1;
	b.sig="b";
	b.ins="c";
	b.rd="d";
	b.rs="e"; b.rt="f";
}
void assign2(stage &a, stage &b){  
	a.id=b.id;
	a.sig=signal(b.ins); // I think we need to flush rs,rd,rt of b because we are checking for the MEM stage
	a.ins=b.ins;
	a.rs=b.rs;
	a.rd=b.rd;
	a.rt=b.rt;
}


 void MIPS_Architecture::executeCommandspipelined()
{
	
	if (commands.size() >= MAX / 4)
	{
		handleExit(MEMORY_ERROR, 0);
		return;
	}
    int counter=-1;
	int clockCycles = 0;
	int cycle=0;
	stage IF,IF_prev; 
	stage ID,ID_prev;
	stage EX,EX_prev;
	stage MEM,MEM_prev;
	stage WB,WB_prev;
	int n=commands.size();
	while (counter < n )
	{
		cycle++;
		++clockCycles;
		cout<<"@@@@@@@ "<<counter<<endl;
		// vector<string> command;
		// if (counter<n-1 && IF.id==-1) { command= commands[counter+1];}
		// else{
		// 	 command = commands[counter];
		// }
		// for (auto x : command){
		// 	cout<<x<<endl;
		// }
		// if (instructions.find(command[0]) == instructions.end())
		// {
		// 	handleExit(SYNTAX_ERROR, clockCycles); // command not found
		// 	return;
		// }
		if (WB.id == -1 && MEM.id != -1)
		{
			WB.id = MEM.id;
			assign2(MEM_prev,MEM);
			assign( WB,  MEM);
			WB.id=-1;
			if (counter==n-1){counter++; cycle++;}
		}
		if (MEM.id == -1 && EX.id != -1)
		{
			assign2(EX_prev,EX);
            assign(MEM,EX);
			
		}
		if (EX.id == -1 && ID.id != -1) 
		{   cout<<((detect_EX_hazard(EX_prev,ID))==0 && detect_Mem_hazard(MEM,ID)==0)<<endl;
			if ((detect_EX_hazard(EX_prev,ID))==0 && detect_Mem_hazard(MEM,ID)==0) {
			assign2(ID_prev,ID);
            assign(EX,ID);
			exit_code ret = (exit_code) instructions[EX.ins](*this, EX.rd, EX.rs,EX.rt);
			if (ret != SUCCESS)
			{
				handleExit(ret, clockCycles);
				return;
			}
			++commandCount[counter];
			if (EX.sig=="NA"){ counter=PCnext-1;}
		    // counter = PCnext;
			}
		}
		if (ID.id == -1 && IF.id != -1)
		{
			assign2(IF_prev,ID_prev);
           assign(ID,IF);
		}
		if (IF.id == -1 && counter<n-1)
		{   
			// cout<<"HI"<<endl;
			counter++;
			vector<string> command = commands[counter];
			
			IF.id=counter;
			IF.sig=signal(command[0]);
			IF.ins=command[0];
            IF.rd=command[1];
			IF.rs=command[2];
			IF.rt=command[3];
			// counter=PCnext;
			// if (PCbranch == -1)
			// {
			// 	counter++;
			// }
			// else
			// {
			// 	counter = PCbranch;
			// 	PCbranch = -1;
			// }
		}
		printRegisters(clockCycles);
	}
	cout<<"cycle: "<<cycle<<endl;
	handleExit(SUCCESS, clockCycles);
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