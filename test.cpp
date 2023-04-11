
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


int detect_EX_hazard(stage a,  stage b) // remember to incalculate hazard for lw in EX and MEM hazard
{
	if (a.id<0 || b.id<0){return 0;}
	// if ( ((a.sig == "RW" &&a.rd == b.rs || (b.rs.find(a.rd))!=string::npos)))
	if ( ((a.rd == b.rs || (b.rs.find(a.rd))!=string::npos)))
	{
		return 1;
	}
	// else if ((a.sig == "RW" && (a.rd == b.rt || (b.rt.find(a.rd))!=string::npos)))
	else if (( (a.rd == b.rt || (b.rt.find(a.rd))!=string::npos)))
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
	// if ( ((a.sig == "RW" &&a.rd == b.rs || (b.rs.find(a.rd))!=string::npos)))
	if ( ((a.rd == b.rs || (b.rs.find(a.rd))!=string::npos)))
	{
		return 1;
	}
	// else if ((a.sig == "RW" && (a.rd == b.rt || (b.rt.find(a.rd))!=string::npos)))
	else if (( (a.rd == b.rt || (b.rt.find(a.rd))!=string::npos)))
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
	if (a.sig == "MR" && ((rd == rs1) || (rd == rt1)))
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
    // int PCcurr=-1;
	int clockCycles = 0;
	int cycle=0;
	stage IF,IF_prev; 
	stage ID,ID_prev;
	stage EX,EX_prev;
	stage MEM,MEM_prev;
	stage WB,WB_prev;
	bool stall=false;
	bool branch=false;
	int n=commands.size();
    vector<int> eval(n,0);
	while (PCcurr < n )
	{
		if (cycle>40){break;}
		cycle++;
		++clockCycles;
		// vector<string> command;
		// if (PCcurr<n-1 && IF.id==-1) { command= commands[PCcurr+1];}
		// else{
		// 	 command = commands[PCcurr];
		// }
		// for (auto x : command){
		// 	cout<<x<<endl;
		// }
		// if (instructions.find(command[0]) == instructions.end())
		// {
		// 	handleExit(SYNTAX_ERROR, clockCycles); // command not found
		// 	return;
		// }
		cout<<cycle<<" cycle "<<endl;
		cout<<"PCcurr "<<PCcurr<<endl;
		if (WB.id == -1 && MEM.id != -1)
		{
			cout<<"In WB"<<endl;
			WB.id = MEM.id;
			assign2(MEM_prev,MEM);
			assign( WB,  MEM);
			eval[WB.id]=cycle;
			WB.id=-1;
			if (PCcurr==n-1 && MEM.id==-1 && ID.id==-1 && EX.id==-1 && IF.id==-1){PCcurr++; cycle++;}
		}
		if (MEM.id == -1 && EX.id != -1)
		{
			cout<<"In MEM"<<endl;
			assign2(EX_prev,EX);
            assign(MEM,EX);
			if (EX_prev.sig=="NA"){ 
				cout<<"NA "<<EX_prev.ins<< EX_prev.rd<<endl;
				if (EX_prev.ins=="beq"){beq(EX_prev.rs,EX_prev.rt,EX_prev.rd);}
				else if (EX_prev.ins=="bne"){bne(EX_prev.rs,EX_prev.rt,EX_prev.rd);}
				else {j(EX_prev.rd);}
				// cout<<beq(EX_prev.rs,EX_prev.rt,EX_prev.rd)<<" dfsd"<<endl;
				// exit_code ret = (exit_code) instructions[EX_prev.ins](*this, EX_prev.rd, EX_prev.rs,EX_prev.rt);
				// cout<<"ret "<<ret<<endl;
				branch=false;
				PCcurr=PCnext-1;
				cout<<PCnext-1<<" a PCcurr "<<PCcurr<<endl;
			}
		}
		if (EX.id == -1 && ID.id != -1) 
		{  
			if (detect_EX_hazard(MEM_prev,ID)==0 && detect_EX_hazard(EX_prev,ID)==0){
				cout<<"IN EX1"<<endl;
			assign2(ID_prev,ID);
            assign(EX,ID);
			if (EX.sig!="NA"){
			exit_code ret = (exit_code) instructions[EX.ins](*this, EX.rd, EX.rs,EX.rt);
			cout<<"ret "<<ret<<endl;}
			++commandCount[PCcurr];
			}
			else if (detect_EX_hazard(MEM_prev,ID)!=0 && detect_EX_hazard(EX_prev,ID)!=0 && eval[ID.id-1]<=cycle-1 && eval[ID.id-2]<=cycle-2 && eval[ID.id-1]>0 && eval[ID.id-2]>0  ){
				cout<<"eval[ID.id]"<<eval[ID.id-1]<<endl;
				stall=false;
				cout<<"IN EX2"<<endl;
			assign2(ID_prev,ID);
            assign(EX,ID);
			if (EX.sig!="NA"){
			exit_code ret = (exit_code) instructions[EX.ins](*this, EX.rd, EX.rs,EX.rt);
			cout<<"ret "<<ret<<endl;}
			++commandCount[PCcurr];}
			else if (detect_EX_hazard(EX_prev,ID)!=0 && detect_EX_hazard(MEM_prev,ID)==0 && eval[ID.id-1]<=cycle-1  && eval[ID.id-1]>0 ){stall=false;
			cout<<"IN EX3"<<endl;
			assign2(ID_prev,ID);
            assign(EX,ID);
			if (EX.sig!="NA"){
			exit_code ret = (exit_code) instructions[EX.ins](*this, EX.rd, EX.rs,EX.rt);
			cout<<"ret "<<ret<<endl;}
			++commandCount[PCcurr];}
			else if (detect_EX_hazard(MEM_prev,ID)!=0  && detect_EX_hazard(EX_prev,ID)==0 && eval[ID.id-2]<=cycle-2  && eval[ID.id-2]>0){stall=false;
			cout<<"IN EX4"<<endl;
			assign2(ID_prev,ID);
            assign(EX,ID);
			if (EX.sig!="NA"){
			exit_code ret = (exit_code) instructions[EX.ins](*this, EX.rd, EX.rs,EX.rt);
			cout<<"ret "<<ret<<endl;}
			++commandCount[PCcurr];}
			else {stall=true;}
			cout<<" after stall "<<stall<<endl; 
		}
		if (ID.id == -1 && IF.id != -1)
		{
			cout<<"In ID"<<endl;
			assign2(IF_prev,IF);
           assign(ID,IF);
		}
		if (IF.id == -1 && PCcurr<n-1 && branch==false)
		{   
			// cout<<"HI"<<endl;
			cout<<"In IF"<<endl;
			PCcurr++;
			vector<string> command = commands[PCcurr];
			IF.id=PCcurr;
			IF.sig=signal(command[0]);
			IF.ins=command[0];
			if (IF.ins=="bne" || IF.ins=="beq"){
				branch=true;
				IF.rd=command[3];
				IF.rs=command[1];
				IF.rt=command[2];
			}
			else if (IF.ins=="j"){
				branch=true;
				IF.rd=command[1];
				IF.rs=command[2];
				IF.rt=command[3];
			}
			else if (IF.ins=="lw" || IF.ins=="sw"){
				IF.rs=command[1];
				IF.rt=command[2];
			}
			else{
            IF.rd=command[1];
			IF.rs=command[2];
			IF.rt=command[3]; }
			// PCcurr=PCnext;
			// if (PCbranch == -1)
			// {
			// 	PCcurr++;
			// }
			// else
			// {
			// 	PCcurr = PCbranch;
			// 	PCbranch = -1;
			// }
		}
		// printRegisters(cycle);
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