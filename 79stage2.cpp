#include <bits/stdc++.h>
#include "MIPS_Processor.hpp"
using namespace std;

struct instruct{
	string rs;
	string rt;
	string rd;
	string type;
	vector<int> time;
	vector<int> value;
};

int dep(int a,int b, instruct(ins)[]){
	bool x=(ins[a].rs==ins[b].rd);
	bool y=(ins[a].rt.find(ins[b].rd)!=string::npos);
	bool z=(ins[a].rt.find("$")!=string::npos);
	if ( (x || (y  && z)) && ins[b].rd!="") {
			return 1;
		}
	else{
		return 0;
	}
}

int hazard(int a,int b, instruct ( ins)[]){ // if rt of sw depends on i-1th instruction 
    int x=7; int y=7;
	if(ins[a].type=="sw" || ins[a].type=="lw"){x=9;}
	if(ins[b].type=="sw" || ins[b].type=="lw"){y=9;}
	int z= dep(a,b,ins);
	if(x==7 && y==7){
		if(z){return 2;}
		else{return 0;}
	}
	else if(x==7 && y==9){
		if(z){return 4;}
		else{return 0;}
	}
	else if(x==9 && y==7){
		if(z){return 2;}
		else{return 0;}
	}
	else{
		if(z && ins[b].type!="sw"){return 4;}
		else{return 0;}
	}
}

void update(int a, int b, instruct (ins)[], map<string,int> mp, vector<vector<int>> &v, int i, vector<string> seven, vector<string> nine, int &last){
	if(ins[i].type=="lw" || ins[i].type=="sw"){
		for(int j=a;j<b;j++){
			ins[i].time[j]=ins[i].time[j-1]+1; int x=mp[nine[j]];
			while(v[ins[i].time[j]][x]>1){
				ins[i].time[j]++;
			}
			if(j==8 && ins[i].time[8]<=last){ins[i].time[8]=last+1; last++;}
			else if(j==8){last=ins[i].time[8];}
			v[ins[i].time[j]][x]++;
		}
	}
	else{
		for(int j=a;j<min(b,7);j++){
			ins[i].time[j]=ins[i].time[j-1]+1; int x=mp[seven[j]];
			while(v[ins[i].time[j]][x]>1){
				ins[i].time[j]++;
			}
			if(j==6 && ins[i].time[6]<=last){ins[i].time[6]=last+1; last++;}
			else if(j==6){last=ins[i].time[6];}
			v[ins[i].time[j]][x]++;
		}
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
		for (int j=0; j<9; j++){
			(ins[i].time).push_back(-1);
		}
	}
    vector<string> nine{"IF1","IF2","DE1","DE2","RR","ALU","DM1","DM2","RW"};
    vector<string> seven{"IF1","IF2","DE1","DE2","RR","ALU","RW"};
    map<string,int> mp={{ "IF1",0},{"IF2",1},{"DE1",2},{"DE2",3},{"RR",4},{"ALU",5},{"DM1",6},{"DM2",7},{"RW",8}};
    vector<vector<int>> v(1000,vector<int>(9,0));
    ins[0].time={1,2,3,4,5,6,7,8,9}; int last;
	if(ins[0].type=="lw" || "sw"){last=9;}
	else{last=7;}

	for(int i=1;i<m;i++){ 
		// remember to handle branch hazard
		if(ins[i-1].type=="j"){
            
            ins[i].time[0]=ins[i-1].time[3]+1;
			update(1,9,ins,mp,v,i,seven,nine,last);

		}
		else if(ins[i-1].type=="bne" || ins[i-1].type=="beq"){
			ins[i].time[0]=ins[i-1].time[5]+1;
			update(1,9,ins,mp,v,i,seven,nine,last);
		}
		else {
			int x= hazard(i,i-1,ins); int z=ins[i-1].time[4];
			if(ins[i-1].type=="sw" || ins[i-1].type=="lw"){
				if(x){z=ins[i-1].time[8];}
				if(i>1){
					int y=hazard(i,i-2,ins);
					if(y){
						if(ins[i-2].type=="sw" || ins[i-2].type=="lw"){
							z=max(z,ins[i-2].time[8]);
						}
						else{
							z=max(z,ins[i-2].time[6]);
						}
					}
				}
			}
			else{
				if(x){z=ins[i-1].time[6];}
				if(i>1){
					int y=hazard(i,i-2,ins);
					if(y){
						if(ins[i-2].type=="sw" || ins[i-2].type=="lw"){
							z=max(z,ins[i-2].time[8]);
						}
						else{
							z=max(z,ins[i-2].time[6]);
						}
					}
				}
			}
			ins[i].time[0]=ins[i-1].time[0]+1;
			update(1,4,ins,mp,v,i,seven,nine,last);            
			ins[i].time[4]=max(z,ins[i].time[3]+1);
			update(5,9,ins,mp,v,i,seven,nine,last);		
		}
		for(int k=0;k<9;k++){
			cout<<ins[i].time[k]<<" ";
		}
		cout<<endl;
	}
	int s=ins[m-1].time[6];
	if(ins[m-1].type=="lw" || ins[m-1].type=="sw"){s=ins[m-1].time[8];}
	if(m>1 && ins[m-2].type=="sw" || ins[m-2].type=="lw"){s=max(s,ins[m-2].time[8]);}
	cout<<s<<endl;
	string t;
	t.append(1+s,'.');
	vector<string> pipe(m,t);
	for(int i=0;i<m;i++){
		if(ins[i].type=="lw" || ins[i].type=="sw"){
			for(int j=0;j<9;j++){
			pipe[i][ins[i].time[j]]='|';
			}
			continue;
		}
		for(int j=0;j<7;j++){
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