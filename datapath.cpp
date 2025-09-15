#include<bits/stdc++.h>
using namespace std;


vector<int>reg(32,0);
vector<vector<bool>>mem(1e6,vector<bool>(32,0));

map<string,string> find_type={

{"0110011","R_type"},
{"0010011","I_type"},
{"0000011","L_type"},
{"0100011","S_type"},
{"1100011","B_type"},
{"1100111","Jalr"},
{"1101111","Jal_type"}


};




int main(){
    fstream f("output.txt",ios::in|ios::out);
    vector<string>machinecode;
    string line;
    while(getline(f,line)){
        machinecode.push_back(line);
        
    }
    
    int pc=0;
    for(auto& ins:machinecode){
        string op=ins.substr(25,7);
        
    }
  
    
    
    
    
    
    
}

