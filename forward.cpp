#include <bits/stdc++.h>
using namespace std;
vector<int> valid(33, -1); // Note: This is now unused with forwarding, but harmless.

// Helper function for correct sign extension
int sign_extend(int value, int bits) {
    int shift = 32 - bits;
    // Pushes the sign bit to the far left, then shifts back, preserving the sign
    return (value << shift) >> shift; 
}

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

// *** CORRECTED immgen CLASS ***
class immgen
{
public:
    int imm;
    string ins;
    void ig(string ins)
    {
        this->ins = ins;
        string opcode = ins.substr(25, 7);

        if (opcode == "0110011") // R-type
        {
            imm = 0;
        }
        else if (opcode == "0010011" || // I-type (ALU)
                 opcode == "0000011" || // I-type (Load)
                 opcode == "1100111")   // I-type (jalr)
        {
            string imm_str = ins.substr(0, 12);
            imm = stoi(imm_str, nullptr, 2);
            imm = sign_extend(imm, 12);
        }
        else if (opcode == "0100011") // S-type (Store)
        {
            string imm_11_5 = ins.substr(0, 7);
            string imm_4_0 = ins.substr(20, 5);
            string imm_str = imm_11_5 + imm_4_0;
            imm = stoi(imm_str, nullptr, 2);
            imm = sign_extend(imm, 12);
        }
        else if (opcode == "1100011") // B-type (Branch)
        {
            // imm[12] [10:5] [4:1] [11]
            string imm_12   = ins.substr(0, 1);  // ins[0]
            string imm_10_5 = ins.substr(1, 6);  // ins[1:6]
            string imm_4_1  = ins.substr(20, 4); // ins[20:23]
            string imm_11   = ins.substr(24, 1); // ins[24]
            
            // Reassemble in correct order and add imm[0] = '0'
            string imm_str = imm_12 + imm_11 + imm_10_5 + imm_4_1 + "0";
            imm = stoi(imm_str, nullptr, 2);
            imm = sign_extend(imm, 13);
        }
        else if (opcode == "1101111") // J-type (jal)
        {
            // imm[20] [10:1] [11] [19:12]
            string imm_20    = ins.substr(0, 1);  // ins[0]
            string imm_10_1  = ins.substr(1, 10); // ins[1:10]
            string imm_11    = ins.substr(11, 1); // ins[11]
            string imm_19_12 = ins.substr(12, 8); // ins[12:19]

            // Reassemble in correct order and add imm[0] = '0'
            string imm_str = imm_20 + imm_19_12 + imm_11 + imm_10_1 + "0";
            imm = stoi(imm_str, nullptr, 2);
            imm = sign_extend(imm, 21);
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
        s = 0;
        v = 1;
    }
};
class ifid
{
public:
    int dpc;
    int npc;
    int s;
    int v;
    string ir;
    ifid()
    {
        s = 0;
        v = 0;
    }
};

// *** UPDATED idex CLASS ***
class idex
{
public:
    int dpc;
    int jpc;
    int imm1;
    int rs1;
    int rs2;
    int imm2;
    int rsl1; // <-- ADDED
    int rsl2; // <-- ADDED

    int alusrc;
    int aluop;
    int branch;
    int memread;
    int memwrite;
    int jump;
    int memtoreg;
    int regwrite;
    int s;
    int v;
    string ir;
    int rdl;
    idex()
    {
        s = 0;
        v = 0;
    }
};
class exmo
{
public:
    int dpc;
    int bpc;
    int jpc;
    int s;
    int v;
    int branch;
    int memread;
    int memwrite;
    int jump;
    int memtoreg;
    int regwrite;

    int aluout;
    int rs2;
    int rdl;
    exmo()
    {
        s = 0;
        v = 0;
    }
};

// *** UPDATED mowb CLASS ***
class mowb
{
public:
    int dpc;
    int bpc;
    int jpc;
    int s;
    int v;
    int branch;
    int jump;
    int memtoreg;
    int regwrite;
    int memread; // <-- ADDED

    int ldout;
    int aluout;
    int rdl;
    mowb()
    {
        s = 0;
        v = 0;
    }
};

void insfetch(ipc &obj1, ifid &obj2, insMem &obj3, vector<string> machinecode)
{
    // Added safety check for pc < 0
    if (obj1.pc >= machinecode.size() || obj1.pc < 0) 
        return;
    if (obj2.s) // Stall check
        return;

    obj2.ir = obj3.fetch(obj1.pc);
    obj2.npc = obj1.pc + 1;
    obj2.dpc = obj1.pc;
    obj1.pc++;
    obj2.v = 1;
    obj1.v = 0;
}

// *** UPDATED insdecode FUNCTION ***
void insdecode(ifid &obj2, idex &obj3, immgen &gen, controlPath &cp, regFile &rf, exmo &obj4) // Added obj4
{
    if (obj3.s || (!obj2.v))
        return;

    // --- NEW: Load-Use Hazard Detection ---
    int rsl1_stall = stoi(obj2.ir.substr(12, 5), nullptr, 2);
    int rsl2_stall = stoi(obj2.ir.substr(7, 5), nullptr, 2);

    // Check if the instruction in the EX stage (obj4) is a load
    // and if its destination matches our current sources.
    if (obj4.v && obj4.memread && obj4.rdl != 0 && (obj4.rdl == rsl1_stall || obj4.rdl == rsl2_stall))
    {
        obj2.s = 1; // Stall the ifid register
        return;     // Return without setting obj3.v=1, inserting a bubble
    }
    obj2.s = 0; // No stall, so make sure fetch is not stalled
    // --- END NEW LOGIC ---

    obj3.dpc = obj2.dpc;
    gen.ig(obj2.ir);
    int im = gen.imm;
    string op = obj2.ir.substr(25, 7);
    obj3.jpc = obj2.dpc + im; // Note: This is wrong for jumps, but matches your original.
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
    
    int rsl1 = stoi(obj2.ir.substr(12, 5), nullptr, 2);
    int rsl2 = stoi(obj2.ir.substr(7, 5), nullptr, 2);
    obj3.rsl1 = rsl1; // <-- ADDED: Pass register number
    obj3.rsl2 = rsl2; // <-- ADDED: Pass register number

    // --- DELETED OLD STALL LOGIC ---
    rf.reg_fillr1(obj2.ir, cp.regread, cp.regwrite);
    rf.reg_fillr2(obj2.ir, cp.regread, cp.regwrite);
    
    int rdl = stoi(obj2.ir.substr(20, 5), nullptr, 2);
    // --- DELETED SCOREBOARD LOGIC ---
    
    obj3.rs1 = rf.rs1;
    obj3.rs2 = rf.rs2;
    obj3.rdl = rdl;
    obj3.ir = obj2.ir;
    // obj2.s = 0; // This is set above now
    obj3.v = 1;
    obj2.v = 0;
}

// *** UPDATED execute FUNCTION ***
void execute(ipc &obj1, ifid &obj2, idex &obj3, exmo &obj4, ALU &alu, ALUcontrol &ac, mowb &obj5) // Added obj5
{
    if (obj4.s || (!obj3.v))
        return;

    obj4.dpc = obj3.dpc;
    obj4.bpc = obj3.dpc + obj3.imm1; // B-type target
    obj4.jpc = obj3.jpc; // J-type target (using obj3.jpc from decode)
    obj4.rdl = obj3.rdl;
    int alusrc = obj3.alusrc;

    // --- NEW: Operand Forwarding Logic ---
    int alu_in1 = obj3.rs1;     // Default value for ALU input 1 (from regfile)
    int alu_in2_reg = obj3.rs2; // Default value for ALU input 2 (from regfile)

    // Check MEM Stage (obj4) for forwarding
    // This handles ALU results (add, sub, etc.)
    if (obj4.v && obj4.regwrite && !obj4.memread && obj4.rdl != 0 && obj4.rdl == obj3.rsl1)
    {
        alu_in1 = obj4.aluout;
    }
    if (obj4.v && obj4.regwrite && !obj4.memread && obj4.rdl != 0 && obj4.rdl == obj3.rsl2)
    {
        alu_in2_reg = obj4.aluout;
    }

    // Check WB Stage (obj5) for forwarding
    // This handles Load results (lw) OR older ALU results
    // We also check that the MEM stage isn't *already* forwarding the same register
    if (obj5.v && obj5.regwrite && obj5.rdl != 0 && obj5.rdl == obj3.rsl1 &&
        !(obj4.v && obj4.regwrite && !obj4.memread && obj4.rdl != 0 && obj4.rdl == obj3.rsl1))
    {
        alu_in1 = (obj5.memread) ? obj5.ldout : obj5.aluout; // MUX: Pick load data or ALU data
    }
    if (obj5.v && obj5.regwrite && obj5.rdl != 0 && obj5.rdl == obj3.rsl2 &&
        !(obj4.v && obj4.regwrite && !obj4.memread && obj4.rdl != 0 && obj4.rdl == obj3.rsl2))
    {
        alu_in2_reg = (obj5.memread) ? obj5.ldout : obj5.aluout; // MUX: Pick load data or ALU data
    }
    // --- END Forwarding Logic ---

    // This is the final value for the ALU's second input
    int alusrc2;
    // This value is also needed by 'sw' in the MEM stage, so we pass it along
    obj4.rs2 = alu_in2_reg; // <-- MODIFIED: Use forwarded value

    if (alusrc == 1)
    {
        alusrc2 = obj3.imm2;
    }
    else
    {
        alusrc2 = alu_in2_reg;
    }

    ac.alucontrolgen(obj3.ir, obj3.aluop);
    int aluselect = ac.aluselect;
    
    // Compute using the (potentially) forwarded values
    alu.compute(alu_in1, alusrc2, ac.aluselect); // <-- MODIFIED
    
    obj4.aluout = alu.alures;
    obj4.branch = (obj3.branch & alu.zeroflag);
    obj4.jump = obj3.jump;
    obj4.memread = obj3.memread;
    obj4.memtoreg = obj3.memtoreg;
    obj4.memwrite = obj3.memwrite;
    obj4.regwrite = obj3.regwrite;
    int tpc;
    if (obj3.jump == 1)
    {
        obj2.v = 0; // Flush IF/ID
        obj3.v = 0; // Flush ID/EX
        
        // JALR logic: target = rs1 + imm
        if(obj3.ir.substr(25,7) == "1100111") {
            obj1.pc = alu_in1 + obj3.imm1; // Use forwarded rs1 + imm
        } else { // JAL logic
            obj1.pc = obj3.jpc;
        }
    }
    else if (obj4.branch == 1)
    {
        obj2.v = 0; // Flush IF/ID
        obj3.v = 0; // Flush ID/EX
        obj1.pc = obj4.bpc;
    }

    obj3.s = 0;
    obj4.v = 1;
    obj3.v = 0;
}

// *** UPDATED memop FUNCTION ***
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
    obj5.memread = obj4.memread; // <-- ADDED
    mm.memunit(obj4.aluout, obj4.rs2, obj4.memread, obj4.memwrite);
    obj5.ldout = mm.ldres;
    obj4.s = 0;
    obj5.v = 1;
    obj4.v = 0;
}

// *** UPDATED writeback FUNCTION ***
void writeback(mowb &obj5, ipc &obj1, regFile &rf)
{
    if (!(obj5.v))
        return;

    int tpc;

    int wdata;
    if (obj5.jump == 1)
    {
        wdata = obj5.dpc + 1; // JAL/JALR write pc+4 (or pc+1 in your case)
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
    }
    rf.regs[0] = 0; // x0 is always 0
    
    // --- DELETED SCOREBOARD LOGIC ---

    obj5.s = 0;
    obj5.v = 0;
}

// *** UPDATED main FUNCTION ***
int main()
{
    fstream f("output1.txt", ios::in | ios::out);
    vector<string> machinecode;
    string line;
    while (getline(f, line))
    {
        machinecode.push_back(line);
    }

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
    int cycle = 0;

    // --- NEW RE-ORDERED LOOP ---
    while (1)
    {
        cycle++;

        // Check for exit condition *before* writeback clears the valid bit
        // The last instruction is at index size() - 1
        if (obj5.v && obj5.dpc == InsMem.instructions.size() - 1)
            break;

        // 1. "Read" stages run first
        //    execute() reads obj4/obj5 for forwarding
        //    insdecode() reads obj4 for load-use stall
        execute(obj1, obj2, obj3, obj4, alu, ALUControl, obj5);
        insdecode(obj2, obj3, ImmGen, ControlPath, RegFile, obj4);
        writeback(obj5, obj1, RegFile);
        // 2. "Write" stages run next
        memop(obj4, obj5, MainMem);


        // 3. "Fetch" stage runs last to prepare for next cycle
        if (obj1.pc >= 0 && obj1.pc < machinecode.size())
        {
            insfetch(obj1, obj2, InsMem, machinecode);
        }
    }
    // --- END NEW LOOP ---

    cout << "cycles:" << cycle << endl;

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
