#include<bits/stdc++.h>
using namespace std;


    map<string,vector<string>> mp={
        {"add",{"R","000","0000000"}},
        {"mul",{"R","001","0100000"}},
           {"div",{"R","010","0100000"}},
              {"rem",{"R","011","0100000"}},
        {"sub",{"R","000","0100000"}},
        {"xor",{"R","100","0000000"}},
        {"or",{"R","110","0000000"}},
        {"and",{"R","111","0000000"}},
        {"sll",{"R","001","0000000"}},
        {"slr",{"R","101","0000000"}},
        {"sra",{"R","101","0100000"}},
        {"slt",{"R","010","0000000"}},
        {"sltu",{"R","011","0000000"}},
        {"addi",{"IA","000","0000000"}},
        {"slti",{"IA","001","0000000"}},
        {"sltiu",{"IA","010","0000000"}},
        {"andi",{"IA","000","0100000"}},
        {"ori",{"IA","001","0100000"}},
        {"xori",{"IA","010","0100000"}},
        {"slli",{"IS","011","0000000"}},
       {"srli",{"IS","100","0000000"}},
       {"srai",{"IS","011","0100000"}},
       {"lw",{"L","000","0000000"}},
       {"ld",{"L","001","0000000"}},
       {"lh",{"L","010","0000000"}},
       {"lb",{"L","011","0000000"}},
       {"lwu",{"L","100","0000000"}},
       {"lhu",{"L","101","0000000"}},
       {"lbu",{"L","110","0000000"}},
       {"sd",{"S","000","0000000"}},
       {"sw",{"S","001","0000000"}},
       {"shw",{"S","010","0000000"}},
       {"sb",{"S","011","0000000"}},
       {"beq",{"B","000","0000000"}},
       {"bne",{"B","001","0000000"}},
       {"blt",{"B","010","0000000"}},
       {"bltu",{"B","011","0000000"}},
       {"bge",{"B","100","0000000"}},
       {"bgeu",{"B","101","0000000"}},
       {"jr",{"Jalr","000","0000000"}},
        {"j",{"Jal"}}
    };
    
    map<string,int> reg={
        {"Zero",0},
        {"ra",1},
        {"sp",2},
        {"gp",3},
        {"tp",4},
        {"t0",5},
        {"t1",6},
        {"t2",7},
        {"s0",8},
        {"fp",8},
        {"s1",9},
        {"a0",10},
        {"a1",11},
        {"a2",12},
        {"a3",13},
        {"a4",14},
        {"a5",15},
        {"a6",16},
        {"a7",17},
        {"s2",18},
        {"s3",19},
        {"s4",20},
        {"s5",21},
        {"s6",22},
        {"s7",23},
        {"s8",24},
        {"s9",25},
        {"s10",26},
        {"s11",27},
        {"t3",28},
        {"t4",29},
        {"t5",30},
        {"t6",31},
        
        
        
        
        
    };
    


class R_type{
  public:
  string f7;
  int r2;
  int r1;
  string f3;
  int rd;
  string opcode="0110011";
  
  
    
};
class I_type_A{
  public:
  int im1;
  int r1;
  string f3;
  int rd;
  string opcode="0010011";
    
};
class I_type_S{
  public:
  string im1;
  int im2;
  int r1;
  string f3;
  int rd;
  string opcode="0010011";
    
};
class L_type{
  public:
  int im1;
  int r1;
  string f3;
  int rd;
  string opcode="0000011";
    
};
class S_type{
  public:
  int im1;
  int r2;
  int r1;
  string f3;
  int im2;
  string opcode="0100011";
    
};
class B_type{
  public:
  int im1;
  int r2;
  int r1;
  string f3;
  int im2;
  string opcode="1100011";
    
};
class Jalr_type{
  public:
  int im1;

  int r1;
  string f3;
  int rd;
  string opcode="1100111";
    
};

class Jal_type{
  public:
  int im1;
  int rd;
  string opcode="1101111";
    
};

string tob(int a,int n){
   
   return bitset<32>(a).to_string().substr(32-n);
}

void format(string type,vector<int>&values){
if(mp[type][0]=="R"){
    R_type ins;
    ins.r1=values[1];
    ins.r2=values[2];
    ins.rd=values[0];
    ins.f7=mp[type][2];
    ins.f3=mp[type][1];
    cout<<ins.f7<<tob(ins.r2,5)<<tob(ins.r1,5)<<ins.f3<<tob(ins.rd,5)<<ins.opcode<<endl;
    
}
else if(mp[type][0]=="IA"){
    I_type_A ins;
    ins.r1=values[1];
    ins.rd=values[0];
  ins.im1=values[2];
  
    ins.f3=mp[type][1];
        cout<<tob(ins.im1,12)<<tob(ins.r1,5)<<ins.f3<<tob(ins.rd,5)<<ins.opcode<<endl;
    
}
else if(mp[type][0]=="IS"){
    I_type_S ins;
    ins.r1=values[1];
    ins.im2=values[2];
    ins.rd=values[0];
    ins.im1=mp[type][2];
    ins.f3=mp[type][1];
        cout<<ins.im1<<tob(ins.im2,5)<<tob(ins.r1,5)<<ins.f3<<tob(ins.rd,5)<<ins.opcode<<endl;
    
}
else if(mp[type][0]=="L"){
    L_type ins;
    ins.r1=values[2];
    ins.rd=values[0];
    ins.im1=values[1];
    ins.f3=mp[type][1];
        cout<<tob(ins.im1,12)<<tob(ins.r1,5)<<ins.f3<<tob(ins.rd,5)<<ins.opcode<<endl;
}
else if(mp[type][0]=="S"){
    S_type ins;
    ins.r1=values[2];
    ins.im2=values[1];
    ins.im1=values[1];
    ins.r2=values[0];
    ins.f3=mp[type][1];
   string temp=tob(ins.im1,12);
    string i1="";
    for(int i=0;i<7;i++){
        i1+=temp[i];
    }
    string i2="";
    for(int i=7;i<12;i++){
        i2+=temp[i];
    }
        cout<<i1<<tob(ins.r2,5)<<tob(ins.r1,5)<<ins.f3<<i2<<ins.opcode<<endl;
}
else if(mp[type][0]=="B"){
    B_type ins;
    ins.r1=values[0];
    ins.r2=values[1];
ins.im2=values[2];
    ins.im1=values[2];
    ins.f3=mp[type][1];
    string temp=tob(ins.im1,12);
    string i1="";
    for(int i=0;i<7;i++){
        i1+=temp[i];
    }
    string i2="";
    for(int i=7;i<12;i++){
        i2+=temp[i];
    }
        cout<<i1<<tob(ins.r2,5)<<tob(ins.r1,5)<<ins.f3<<i2<<ins.opcode<<endl;
}
else if(mp[type][0]=="Jal"){
    Jal_type ins;
    ins.rd=0;
    ins.im1=values[0];
        cout<<tob(ins.im1,20)<<tob(ins.rd,5)<<ins.opcode<<endl;
}
else if(mp[type][0]=="Jalr"){
    Jalr_type ins;
    ins.rd=values[0];
    ins.r1=0;
    ins.im1=0;
    ins.f3=mp[type][1];
        cout<<tob(ins.im1,12)<<tob(ins.r1,5)<<ins.f3<<tob(ins.rd,5)<<ins.opcode<<endl;
}


 
}



pair<string,vector<int>> tokens(string line){
    string type="";
    vector<int>values;
    int i=0;
    while(line[i]==' ') i++;
    while(i<line.length()&&line[i]!= ' '){
        type+=line[i++];
    }

    while(i<line.size()){
        while(i<line.size() && (line[i]==' ' || line[i]==',' ||line[i]=='(' || line[i]==')')){

            i++;
        }
        string temp="";
        while(i<line.length() && line[i]!=' ' && line[i]!=',' && line[i]!='(' && line[i]!=')')
        {
    temp+=line[i++];        
            }
            
          if(!temp.empty()){
              if(reg.find(temp)!=reg.end()){
                  values.push_back(reg[temp]);
              }
              else {
                  values.push_back(stoi(temp));
              }
          }
    }
    return {type,values};
}










int main(){
fstream f("input1.txt",ios::in| ios::out);
string str;

while(getline(f,str)){
pair<string,vector<int>> seg=tokens(str);
 format(seg.first,seg.second);
}




return 0;
}
