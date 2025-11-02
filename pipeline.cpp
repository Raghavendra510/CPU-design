#include <bits/stdc++.h>
using namespace std;

class insMem
{
public:
    vector<string> instructions;

    insMem(vector<string> machinecode)
    {
        instructions = machinecode;
    }

    string fetch(int pc)
    {
        return instructions[pc];
    }
};

class regFile
{
public:
    vector<int> regs;
    vector<int> valid;
    int rsl1;
    int rsl2;
    int rdl;
    int w_data = 0;
    int rs1;
    int rs2;

    regFile()
    {
        vector<int> temp(33);
        regs = temp;
        regs[2] = 100;
        vector<int> temp2(32, 1); // 1 = ready
        temp2[0] = 1;             // x0 is ready (always zero)
        valid = temp2;
    }

    void reg_fill1(string ins, bool regread, bool regwrite)
    {
        string temp = ins.substr(7, 5);
        rsl2 = stoi(temp, nullptr, 2);
        temp = ins.substr(12, 5);
        rsl1 = stoi(temp, nullptr, 2);
        temp = ins.substr(20, 5);
        rdl = stoi(temp, nullptr, 2);
        if (regread)
        {
            rs1 = regs[rsl1];
            rs2 = regs[rsl2];
        }
    }
    void reg_fillr1(string ins, bool regread, bool regwrite)
    {
        string temp = ins.substr(7, 5);
        rsl2 = stoi(temp, nullptr, 2);
        temp = ins.substr(12, 5);
        rsl1 = stoi(temp, nullptr, 2);
        temp = ins.substr(20, 5);
        rdl = stoi(temp, nullptr, 2);
        if (regread)
        {
            rs1 = regs[rsl1];
        }
    }
    void reg_fillr2(string ins, bool regread, bool regwrite)
    {
        string temp = ins.substr(7, 5);
        rsl2 = stoi(temp, nullptr, 2);
        temp = ins.substr(12, 5);
        rsl1 = stoi(temp, nullptr, 2);
        temp = ins.substr(20, 5);
        rdl = stoi(temp, nullptr, 2);
        if (regread)
        {
            rs2 = regs[rsl2];
        }
    }
    void reg_fill2(string ins, int ej, bool regread, bool regwrite)
    {
        string temp = ins.substr(20, 5);
        rdl = stoi(temp, nullptr, 2);
        w_data = ej;
        if (regwrite)
        {
            if (rdl != 0)
                regs[rdl] = w_data;
        }
    }
};

class immgen
{
public:
    int imm;
    string ins;
    void ig(string ins)
    {
        this->ins = ins;
        string opcode = ins.substr(25, 7);
        if (opcode == "0110011")
        {
            imm = 0;
        }
        else if (opcode == "0010011")
        {
            imm = stoi(ins.substr(0, 12), nullptr, 2);
            if (ins.substr(0, 12)[0] == '1')
            {
                imm -= (1 << 12);
            }
        }
        else if (opcode == "0000011")
        {
            imm = stoi(ins.substr(0, 12), nullptr, 2);
            if (ins.substr(0, 12)[0] == '1')
            {
                imm -= (1 << 12);
            }
        }

        else if (opcode == "0100011")
        {
            string temp1 = ins.substr(0, 7);
            string temp2 = ins.substr(20, 5);
            temp1 = temp1 + temp2;
            imm = stoi(temp1, nullptr, 2);
            if (temp1[0] == '1')
            {
                imm -= (1 << 12);
            }
        }

        else if (opcode == "1100011")
        {
            string temp1 = ins.substr(0, 7);
            string temp2 = ins.substr(20, 5);
            temp1 = temp1 + temp2;
            imm = stoi(temp1, nullptr, 2);
            if (temp1[0] == '1')
            {
                imm -= (1 << 12);
            }
        }
        else if (opcode == "1100111")
        { // jalr
            string temp1 = ins.substr(0, 12);
            imm = stoi(ins.substr(0, 12), nullptr, 2);
            if (temp1[0] == '1')
            {
                imm -= (1 << 12);
            }
        }
        else if (opcode == "1101111")
        { // jal
            string temp1 = ins.substr(0, 12);
            imm = stoi(ins.substr(0, 20), nullptr, 2);
            if (temp1[0] == '1')
            {
                imm -= (1 << 20);
            }
        }
    }
};

class ALUcontrol
{

public:
    string ins;
    string f3;
    string f7;
    int aluop;
    int aluselect;
    void alucontrolgen(string ins, int aluop)
    {
        this->ins = ins;
        this->aluop = aluop;
        f3 = ins.substr(17, 3);
        f7 = ins.substr(0, 7);
        if (aluop == 0)
        {
            if (f7 == "0000000")
            {
                if (f3 == "000")
                {
                    aluselect = 0;
                }
                else if (f3 == "001")
                {
                    aluselect = 7;
                }
                else if (f3 == "010")
                {
                    aluselect = 9;
                }
                else if (f3 == "011")
                {
                    aluselect = 9;
                }
                else if (f3 == "100")
                {
                    aluselect = 11;
                }
                else if (f3 == "101")
                {
                    aluselect = 8;
                }
                else if (f3 == "110")
                {
                    aluselect = 5;
                }

                else if (f3 == "111")
                {
                    aluselect = 4;
                }
            }
            else if (f7 == "0100000")
            {
                if (f3 == "000")
                {
                    aluselect = 1;
                }
                else if (f3 == "001")
                {
                    aluselect = 2;
                }
                else if (f3 == "010")
                {
                    aluselect = 6;
                }
                else if (f3 == "011")
                {
                    aluselect = 3;
                }
                else if (f3 == "100")
                {
                    aluselect = 6;
                }
                else if (f3 == "101")
                {
                    aluselect = 8;
                }
            }
        }

        else if (aluop == 1)
        {

            if (f3 == "000")
            {
                aluselect = 0;
            }
            else if (f3 == "001")
            {
                aluselect = 9;
            }
            else if (f3 == "010")
            {
                aluselect = 9;
            }
            else if (f3 == "011")
            {
                aluselect = 4;
            }
            else if (f3 == "100")
            {
                aluselect = 5;
            }
            else if (f3 == "101")
            {
                aluselect = 11;
            }

            else if (f3 == "110")
            {
                aluselect = 7;
            }

            else if (f3 == "111")
            {
                aluselect = 8;
            }
        }
        else if (aluop == 2)
        {

            aluselect = 0;
        }
        else if (aluop == 3)
        {
            aluselect = 0;
        }
        else if (aluop == 4)
        {
            if (f3 == "000")
            {
                aluselect = 12;
            }
            else if (f3 == "001")
            {
                aluselect = 13;
            }
            else if (f3 == "010")
            {
                aluselect = 14;
            }
            else if (f3 == "011")
            {
                aluselect = 15;
            }
            else if (f3 == "100")
            {
                aluselect = 16;
            }
            else if (f3 == "101")
            {
                aluselect = 17;
            }
        }
        else if (aluop == 5)
        {
            aluselect = 12;
        }
    }
};

class ALU
{
public:
    int alusrc1;
    int alusrc2;
    int aluselect;
    bool zeroflag = 0;
    int alures = 0;

    void compute(int a1, int a2, int as)
    {
        alusrc1 = a1;
        alusrc2 = a2;
        aluselect = as;

        if (aluselect == 0)
        {
            alures = alusrc1 + alusrc2;
        }
        else if (aluselect == 1)
        {
            alures = alusrc1 - alusrc2;
        }
        else if (aluselect == 2)
        {
            alures = alusrc1 * alusrc2;
        }
        else if (aluselect == 3)
        {
            if (alusrc2 != 0)
                alures = alusrc1 % alusrc2;
        }
        else if (aluselect == 4)
        {
            alures = alusrc1 & alusrc2;
        }
        else if (aluselect == 5)
        {
            alures = alusrc1 | alusrc2;
        }
        else if (aluselect == 6)
        {
            alures = alusrc1 / alusrc2;
        }
        else if (aluselect == 7)
        {
            alures = alusrc1 << alusrc2;
        }

        else if (aluselect == 8)
        {
            alures = alusrc1 >> alusrc2;
        }
        else if (aluselect == 9)
        {
            alures = (alusrc1 < alusrc2);
        }
        else if (aluselect == 10)
        {
            alures = (alusrc1 >= alusrc2);
        }
        else if (aluselect == 11)
        {
            alures = (alusrc1 ^ alusrc2);
        }
        else if (aluselect == 12)
        {
            zeroflag = (alusrc1 == alusrc2);
        }
        else if (aluselect == 13)
        {
            zeroflag = (alusrc1 != alusrc2);
        }
        else if (aluselect == 14)
        {
            zeroflag = (alusrc1 < alusrc2);
        }
        else if (aluselect == 15)
        {
            zeroflag = (alusrc1 < alusrc2);
        }
        else if (aluselect == 16)
        {
            zeroflag = (alusrc1 >= alusrc2);
        }
        else if (aluselect == 17)
        {
            zeroflag = (alusrc1 >= alusrc2);
        }
    }
};

class controlPath
{
public:
    bool regread;
    bool regwrite;
    int Alusrc;
    int Aluop;
    bool branch;
    bool memread;
    bool memwrite;
    bool memtoreg;
    bool jump;

    void cw(string opcode)
    {
        if (opcode == "0110011")
        {
            Alusrc = 0;
            memtoreg = 0;
            regread = 1;
            regwrite = 1;
            memread = 0;
            memwrite = 0;
            branch = 0;
            jump = 0;
            Aluop = 0;
        }
        else if (opcode == "0010011")
        {
            Alusrc = 1;
            memtoreg = 0;
            regread = 1;
            regwrite = 1;
            memread = 0;
            memwrite = 0;
            branch = 0;
            jump = 0;
            Aluop = 1;
        }
        else if (opcode == "0000011")
        {
            Alusrc = 1;
            memtoreg = 1;
            regread = 1;
            regwrite = 1;
            memread = 1;
            memwrite = 0;
            branch = 0;
            jump = 0;
            Aluop = 2;
        }

        else if (opcode == "0100011")
        {
            Alusrc = 1;
            memtoreg = 0;
            regread = 1;
            regwrite = 0;
            memread = 0;
            memwrite = 1;
            branch = 0;
            jump = 0;
            Aluop = 3;
        }

        else if (opcode == "1100011")
        { // brancvh
            Alusrc = 0;
            memtoreg = 0;
            regread = 1;
            regwrite = 0;
            memread = 0;
            memwrite = 0;
            branch = 1;
            jump = 0;
            Aluop = 4;
        }
        else if (opcode == "1100111")
        {
            Alusrc = 0;
            memtoreg = 0;
            regread = 0;
            regwrite = 1;
            memread = 0;
            memwrite = 0;
            branch = 0;
            jump = 1;
            Aluop = 5;
        }
        else if (opcode == "1101111")
        {
            Alusrc = 0;
            memtoreg = 0;
            regread = 0;
            regwrite = 1;
            memread = 0;
            memwrite = 0;
            branch = 0;
            jump = 1;
            Aluop = 5;
        }
    }
};

class mainmem
{
public:
    vector<int> mem;
    bool memwrite;
    bool memread;
    int EA;
    int writedata;
    int ldres;
    mainmem()
    {
        vector<int> temp(1e5, 0);
        mem = temp;
    }

    void memunit(int ea, int wd, bool mr, bool mw)
    {
        memwrite = mw;
        memread = mr;
        writedata = wd;
        EA = ea;

        if (mr == 1)
        {
            ldres = mem[EA];
        }
        if (mw == 1)
        {
            mem[EA] = writedata;
        }
    }
};

class ipc
{
public:
    int pc;
    int s;
    int v;
    ipc()
    {
        v = 1;
        s = 0;
    }
};
class ifid
{
public:
    int dpc;
    int npc;
    string ir;
    int s;
    int v;
    ifid()
    {
        v = 0;
        s = 0;
    }
};
class idex
{
public:
    int dpc;
    int jpc;
    int imm1;
    int rs1;
    int rs2;
    int imm2;

    int alusrc;
    int aluop;
    int branch;
    int memread;
    int memwrite;
    int jump;
    int memtoreg;
    int regwrite;

    string ir;
    int rdl;

    int s;
    int v;
    idex()
    {
        v = 0;
        s = 0;
    }
};
class exmo
{
public:
    int dpc;
    int bpc;
    int jpc;

    int branch;
    int memread;
    int memwrite;
    int jump;
    int memtoreg;
    int regwrite;

    int aluout;
    int rs2;
    int rdl;
    int s;
    int v;
    exmo()
    {
        v = 0;
        s = 0;
    }
};
class mowb
{
public:
    int dpc;
    int bpc;
    int jpc;

    int branch;
    int jump;
    int memtoreg;
    int regwrite;

    int ldout;
    int aluout;
    int rdl;
    int s;
    int v;
    mowb()
    {
        v = 0;
        s = 0;
    }
};

void insfetch(ipc &obj1, ifid &obj2, insMem &obj3)
{
    if (obj2.s || (!obj1.v))
        return;
    obj2.ir = obj3.fetch(obj1.pc);
    obj2.npc = obj1.pc + 1;
    obj2.dpc = obj1.pc;
    obj2.v = 1;
}

void insdecode(ifid &obj2, idex &obj3, immgen &gen, controlPath &cp, regFile &rf)
{
    if (obj3.s || (!obj2.v))
        return;

    obj3.dpc = obj2.dpc;
    gen.ig(obj2.ir);
    int im = gen.imm;
    string op = obj2.ir.substr(25, 7);
    obj3.jpc = obj2.dpc + im;
    obj3.imm1 = im;
    obj3.imm2 = im;
    cp.cw(op);
    obj3.aluop = cp.Aluop;
    obj3.alusrc = cp.Alusrc;
    obj3.branch = cp.branch;
    obj3.jump = cp.jump;
    obj3.memread = cp.memread;
    obj3.memtoreg = cp.memtoreg;
    obj3.memwrite = cp.memwrite;
    obj3.regwrite = cp.regwrite;
    int r1;
    int r2;
    r1 = stoi(obj2.ir.substr(12, 5));
    r2 = stoi(obj2.ir.substr(7, 5));
    if (rf.valid[r1] == 1)
    {
        rf.reg_fillr1(obj2.ir, cp.regread, cp.regwrite);
        obj3.rs1 = rf.rs1;
    }
    else
    {
        obj2.s = 1;
        return;
    }
    if (rf.valid[r2] == 1)
    {
        rf.reg_fillr2(obj2.ir, cp.regread, cp.regwrite);
        obj3.rs2 = rf.rs2;
    }
    else
    {
        obj2.s = 1;
        return;
    }

    obj3.rdl = rf.rdl;
    obj3.ir = obj2.ir;
    obj2.s = 0;
    obj3.v = 1;
    if (obj3.regwrite && obj3.rdl != 0)
    {
        rf.valid[obj3.rdl] = 0; // mark as not ready
    }
}

void execute(idex &obj3, exmo &obj4, ALU &alu, ALUcontrol &ac)
{
    if (obj4.s || (!obj3.v))
    {
        return;
    }

    obj4.dpc = obj3.dpc;
    obj4.bpc = obj3.dpc + obj3.imm1;
    obj4.jpc = obj3.jpc;
    obj4.rdl = obj3.rdl;
    int alusrc = obj3.alusrc;
    int alusrc2;
    obj4.rs2 = obj3.rs2;
    if (alusrc == 1)
    {
        alusrc2 = obj3.imm2;
    }
    else
    {
        alusrc2 = obj3.rs2;
    }
    ac.alucontrolgen(obj3.ir, obj3.aluop);
    int aluselect = ac.aluselect;
    alu.compute(obj3.rs1, alusrc2, ac.aluselect);
    obj4.aluout = alu.alures;

    obj4.branch = (obj3.branch & alu.zeroflag);
    obj4.jump = obj3.jump;
    obj4.memread = obj3.memread;
    obj4.memtoreg = obj3.memtoreg;
    obj4.memwrite = obj3.memwrite;
    obj4.regwrite = obj3.regwrite;
    obj3.s = 0;
    obj4.v = 1;
}

void memop(exmo &obj4, mowb &obj5, mainmem &mm)
{
    if (obj5.s || (!obj4.v))
        return;
    obj5.dpc = obj4.dpc;
    obj5.bpc = obj4.bpc;
    obj5.jpc = obj4.jpc;
    obj5.rdl = obj4.rdl;
    obj5.aluout = obj4.aluout;
    obj5.branch = obj4.branch;
    obj5.jump = obj4.jump;
    obj5.memtoreg = obj4.memtoreg;
    obj5.regwrite = obj4.regwrite;
    mm.memunit(obj4.aluout, obj4.rs2, obj4.memread, obj4.memwrite);
    obj5.ldout = mm.ldres;
    obj4.s = 0;
    obj5.v = 1;
}

void writeback(mowb &obj5, ipc &obj1, regFile &rf)
{
    if (!obj5.v)
        return;
    int tpc;
    if (obj5.jump == 1)
    {
        tpc = obj5.jpc;
    }
    else if (obj5.branch == 1)
    {
        tpc = obj5.bpc;
    }
    else
    {
        tpc = obj5.dpc + 1;
    }
    obj1.pc = tpc;
    int wdata;
    if (obj5.jump == 1)
    {
        wdata = obj5.dpc + 1;
    }
    else if (obj5.memtoreg == 1)
    {
        wdata = obj5.ldout;
    }
    else
    {
        wdata = obj5.aluout;
    }
    if (obj5.regwrite == 1 && obj5.rdl != 0)
    {
        rf.regs[obj5.rdl] = wdata;
        rf.valid[obj5.rdl] = 1;
    }
    rf.regs[0] = 0;

    if (obj5.rdl != 0)
        rf.valid[obj5.rdl] = 1;
    obj5.s = 0;
}

int main()
{
    fstream f("output1.txt", ios::in | ios::out);
    vector<string> machinecode;
    string line;
    while (getline(f, line))
    {
        machinecode.push_back(line);
    }

    // pipeline required

    int pc = 0;
    insMem InsMem(machinecode);
    regFile RegFile;
    immgen ImmGen;
    ALUcontrol ALUControl;
    ALU alu;
    controlPath ControlPath;
    mainmem MainMem;
    ipc obj1;
    ifid obj2;
    idex obj3;
    exmo obj4;
    mowb obj5;
    obj1.pc = 0;
    while (obj1.pc < (int)machinecode.size() || obj2.v || obj3.v || obj4.v || obj5.v)
    {
        if (obj1.pc == machinecode.size() - 1)
            break;
        writeback(obj5, obj1, RegFile);
        memop(obj4, obj5, MainMem);
        execute(obj3, obj4, alu, ALUControl);
        insdecode(obj2, obj3, ImmGen, ControlPath, RegFile);
        insfetch(obj1, obj2, InsMem);
        cout<<"stages :"<<endl;
        cout<<"obj1: "<<obj1.pc<<endl;
        cout<<"obj2: "<<obj2.dpc<<" "<<obj2.npc<<" "<<obj2.ir<<endl;
        cout<<"obj3: "<<obj3.dpc<<" "<<obj3.jpc<<" "<<obj3.imm1<<" "<<obj3.rs1<<" "<<obj3.rs2<<" "<<obj3.imm2<<" "<<obj3.ir<<" "<<obj3.rdl<<endl;
        cout<<"obj4: "<<obj4.dpc<<" "<<obj4.bpc<<" "<<obj4.jpc<<" "<<obj4.aluout<<" "<<obj4.rs2<<" "<<obj4.rdl<<endl;
        cout<<"obj5: "<<obj5.dpc<<" "<<obj5.bpc<<" "<<obj5.jpc<<" "<<obj5.ldout<<" "<<obj5.aluout<<" "<<obj5.rdl<<endl;
    }

    for (int i = 0; i < 33; i++)
    {
        cout << RegFile.regs[i] << endl;
    }

    cout << "main mem ea is " << MainMem.EA << endl;
    cout << "......................................." << endl;
    for (int i = 0; i <= 100; i++)
    {
        cout << MainMem.mem[i] << " ";
        if (i % 20 == 0)
        {
            cout << endl;
        }
    }
}
