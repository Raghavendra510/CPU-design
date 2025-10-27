#include<bits/stdc++.h>
using namespace std;



class insMem{
    public:
  vector<string>instructions;
  
  insMem(vector<string>machinecode){
      instructions=machinecode;
  }
  
  string fetch(int pc){
      return instructions[pc];
  }
    
    
    
    
};

class regFile{
     public:
    vector<int>regs(33);
    int rsl1;
    int rsl2;
    int rdl;
    int w_data=0;
    int rs1;
    int rs2;

    
    
    
    
 void  reg_fill1(string ins,bool regread,bool regwrite){
        string temp=ins.substr(7,5);
         rsl2=stoi(temp,nullptr,2);
        temp=ins.substr(19,5);
        rsl1=stoi(temp,nullptr,2);
        temp=ins.substr(20,5);
        rdl=stoi(temp,nullpt,2);
        if(regread){
        rs1=regs[rsl1];
        rs2=regs[rsl2];
        }
      
    }
 void  reg_fill2(string ins,bool regread,bool regwrite){
        string temp=ins.substr(20,5);
        rdl=stoi(temp,nullpt,2);
      
        if(regwrite){
            regs[rdl]=w_data;
         }
    }
    
};


class immgen
{
  public :
  int imm;
  string ins;
 void ig(string ins){
      this->ins=ins;
      string opcode=ins.substr(25.7);
     if(opcode=="0110011"){
      imm=0;
  
      }
      else if(opcode=="0010011"){
             imm=stoi(ins.substr(0,12),nullptr,2);
             if(ins.substr(0,12)[0]=='1'){
                 imm-=(1<<12);
             }
      }
      else if(opcode=="0000011"){
      imm=stoi(ins.substr(0,12),nullptr,2);
       if(ins.substr(0,12)[0]=='1'){
                 imm-=(1<<12);
             }
      }
      
      
      else if(opcode=="0100011"){
          string temp1=ins.substr(0,7);
          string temp2=ins.substr(20,5);
          temp1=temp1+temp2;
          imm=stoi(temp1,nullptr,2);
           if(temp1[0]=='1'){
                 imm-=(1<<12);
             }
          
      }
      
      
      else if(opcode=="1100011"){
           string temp1=ins.substr(0,7);
          string temp2=ins.substr(20,5);
          temp1=temp1+temp2;
          imm=stoi(temp1,nullptr,2);
          if(temp1[0]=='1'){
                 imm-=(1<<12);
             }
      }
      else if(opcode=="1100111"){//jalr
           imm=stoi(ins.substr(0,12),nullptr,2);
             if(temp1[0]=='1'){
                 imm-=(1<<12);
             }
         
      }
      else if(opcode=="1101111"){//jal
      imm=stoi(ins.substr(0,20),nullpt,2);
        if(temp1[0]=='1'){
                 imm-=(1<<20);
             }
      
      }
      
      
      
      
  }
  
  
  
  
    
    
};

class ALUcontrol{
    
    public:
    string ins;
    string f3;
    string f7;
    int aluop;
    int aluselect;
 void   alucontrolgen(string ins,int aluop){
        this->ins=ins;
        this->aluop=aluop;
        f3=ins.substr(17,3);
        f7=ins.substr(0,7);
        if(aluop==0){
            if(f7=="0000000"){
                if(f3=="000"){
                    aluselect=0;
                }
                else if(f3=="001"){
                    aluselect=7;
                }
                else if(f3=="010"){
                    aluselect=9;
                }
                else if(f3=="011"){
                    aluselect=9;
                }
                else if(f3=="100"){
                    aluselect=11;
                }
                else if(f3=="101"){
                    aluselect=8;
                }
                else if(f3=="110"){
                    aluselect=5;
                }
                
                else if(f3=="111"){
                    aluselect=4;
                }
                
            }
            else if(f7=="0100000"){
                if(f3=="000"){
                    aluselect=1;
                }
                else if(f3=="001"){
                    aluselect=2;
                }
                else if(f3=="010"){
                    aluselect=6;
                }
                else if(f3=="011"){
                    aluselect=3;
                }
                else if(f3=="100"){
                    aluselect=6;
                }
                else if(f3=="101"){
                    aluselect=8;
                }
            }
                
                
            
            }
            
        else if(aluop==1) {
             if(f7=="0000000"){
                if(f3=="000"){
                    aluselect=0;
                }
                else if(f3=="001"){
                    aluselect=9;
                }
                else if(f3=="010"){
                    aluselect=9;
                }
                else if(f3=="011"){
                    aluselect=7;
                }
                else if(f3=="100"){
                    aluselect=8;
                }
                else if(f3=="101"){
                    aluselect=8;
                }
        
                
            }
            else if(f7=="0100000"){
                if(f3=="000"){
                    aluselect=4;
                }
                else if(f3=="001"){
                    aluselect=5;
                }
                else if(f3=="010"){
                    aluselect=11;
                }
                else if(f3=="011"){
                    aluselect=8;
                }
              
            }
                
                
            
            
            
        }
        else if(aluop==2) {
            
             aluselect=0;
    
            
            
        }
        else if(aluop==3){
            aluselect=0;
        }
        else if(aluop==4){
            if(f3=="000"){
                
            }
        }
     
            
            
            
            
        }
        
        
        
    }
    
    
    
    
};

class ALU{
  public:
  
  int alusrc1;
  int alusrc2;
  int aluselect;
  bool zeroflag;
  int alures;
  
  void  compute(int a1,int a2,int as){
        alusrc1=a1;
        alusrc2=a2;
        aluselect=as;
        
        if(aluselect==0){
            alures=alusrc1+alusrc2;
            
        }
        else if(aluselect==1){
            alures=alusrc1-alusrc2;
        }
        else if(aluselect==2){
            alures=alusrc1*alusrc2;
        }
        else if(aluselect==3){
            alures=alusrc1%alusrc2;
        }
        else if(aluselect==4){
            alures=alusrc1&alusrc2;
        }
        else if(aluselect==5){
            alures=alusrc1|alusrc2;
        }
        else if(aluselect==6){
            alures=alusrc1/alusrc2;
        }
        else if(aluselect==7){
            alures=alusrc1<<alusrc2;
        }
        
        else if(aluselect==8){
            alures=alusrc1>>alusrc2;
        }
        else if(aluselect==9){
            alures=(alusrc1<alusrc2);
        }
        else if(aluselect==10){
            alures=(alusrc1>=alusrc2);
        }
        
        
        
        
        zeroflag=(alusrc1==alusrc2);
        
        
    }
    
    
    
    
    
    
};


class controlPath{
  public:
  bool regread;
  bool regwrite;
  bool Alusrc;
  bool Aluop;
  bool branch;
  bool memread;
  bool memwrite;
  bool memtoreg;
  bool jump;
  
  
  void cw(string opcode){
      if(opcode=="0110011"){
              Alusrc=0;
             memtoreg=0;
             regread=1;
             regwrite=1;
             memread=0;
             memwrite=0;
             branch=0;
             jump=0;
             Aluop=0;

      }
      else if(opcode=="0010011"){
              Alusrc=1;
             memtoreg=0;
             regread=1;
             regwrite=1;
             memread=0;
             memwrite=0;
             branch=0;
             jump=0;
             Aluop=1;
      }
      else if(opcode=="0000011"){
            Alusrc=1;
             memtoreg=1;
             regread=1;
             regwrite=1;
             memread=1;
             memwrite=0;
             branch=0;
             jump=0;
             Aluop=2;
      }
      
      
      else if(opcode=="0100011"){
            Alusrc=1;
             memtoreg=0;
             regread=1;
             regwrite=0;
             memread=0;
             memwrite=1;
             branch=0;
             jump=0;
             Aluop=3;
      }
      
      
      else if(opcode=="1100011"){
            Alusrc=0;
             memtoreg=0;
             regread=1;
             regwrite=0;
             memread=0;
             memwrite=0;
             branch=1;
             jump=0;
             Aluop=4;
      }
      else if(opcode=="1100111"){
            Alusrc=0;
             memtoreg=0;
             regread=0;
             regwrite=1;
             memread=0;
             memwrite=0;
             branch=0;
             jump=1;
             Aluop=5;
      }
      else if(opcode=="1101111"){
            Alusrc=0;
             memtoreg=0;
             regread=0;
             regwrite=1;
             memread=0;
             memwrite=0;
             branch=0;
             jump=1;
             Aluop=5;
      }
  }
  
  
  
    
};


class mainmem{
  vector<int>mem(1e9);
  bool memwrite;
  bool memread;
  int EA;
  int writedata;
  int ldres;
  
  
  void memunit(int ea,int wd,bool mr,bool mw){
      memwrite=mw;
      memread=mr;
      writedata=wd;
      EA=ea;
      
      if(mr==1){
          ldres=mem[EA];
          
      }
      if(mw==1){
          mem[EA]=writedata;
      }
      
      
  }
    
};













 int main(){
     fstream f("output.txt",ios::in|ios::out);
    vector<string>machinecode;
    string line;
    while(getline(f,line)){
       machinecode.push_back(line);
        
     }
    
    int pc=0;
    insMem InsMem(machinecode);
    regFile RegFile;
    immgen ImmGen;
    ALUcontrol ALUControl;
    ALU alu;
    controlPath ControlPath;
    mainmem MainMem;
    
    //cpu 
    while(pc<machinecode.size()){
        
        string cur_ins=InsMem.fetch(pc);
       cw(cur_ins.substr(25,7));
        RegFile.reg_fill1(cur_ins,ControlPath.regread,ControlPath.regwrite);
        ImmGen.ig(cur_ins);
        int  a=RegFile.rs1;
        int b;
        if(ControlPath.Alusrc==1){
            b=ImmGen.imm
        }
        else {
            b=RegFile.rs2;
        }
        ALUControl.alucontrolgen(cur_ins,ControlPath.Aluop);
        alu.compute(a,b,ALUControl.aluselect);
        MainMem.memunit(alu.alures,RegFile.rs2,ControlPath.memread,ControlPath.memwrite);
        int npc=pc+1;
        int jpc=pc+ImmGen.imm;
        int bpc=pc+ImmGen.imm;
        int tpc;
       
        int m2;
        if(ControlPath.branch&&alu.zeroflag){
            m2=bpc;
        }
        else {
            m2=npc;
        }
        if(ControlPath.jump){
            tpc=jpc;
        }
        else {
            tpc=m2;
        }
        
       int erenjeager; 
        if(ControlPath.jump==1){
            erenjeager=npc;
        }
        else if(ControlPath.memtoreg==1){
            erenjeager=MainMem.ldres;
        }
        else if(ControlPath.memtoreg==0){
            erenjeager=alu.alures;
        }
        RegFile.reg_fill2(cur_ins,ControlPath.regread,ControlPath.regwrite);
        
        pc=tpc;
        
        
        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
 }

