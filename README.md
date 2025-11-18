# ⚙️ RISC-V 5-Stage Pipelined CPU Simulator

This project is a C++ simulation of a 5-stage pipelined RISC-V processor. It models the flow of instructions through the classic five stages—Instruction Fetch (IF), Instruction Decode (ID), Execute (EX), Memory (MEM), and Write-Back (WB)—and includes logic for handling data and control hazards.

## 🚀 Core Features

* **5-Stage Pipeline:** Implements the IF, ID, EX, MEM, and WB stages using dedicated pipeline registers (`ifid`, `idex`, `exmo`, `mowb`).
* **RISC-V ISA Subset:** Supports a variety of instruction types, including:
    * **R-type** (e.g., `add`, `sub`, `sll`, `slt`, `xor`, `srl`, `or`, `and`)
    * **I-type** (e.g., `addi`, `lw`, `jalr`)
    * **S-type** (e.g., `sw`)
    * **B-type** (e.g., `beq`, `bne`, `blt`, `bge`)
    * **J-type** (e.g., `jal`)
* **Hazard Handling:**
    * **Data Hazards:** Implements a **forwarding unit** to send results from the EX and MEM stages back to the EX stage.
    * **Load-Use Hazards:** Implements **stalling** (pipeline bubbling) when a load instruction is immediately followed by an instruction that uses the loaded data.
    * **Control Hazards:** Resolves branches and jumps in the EX stage. On a taken branch or jump, the IF and ID stages are **flushed** to prevent incorrect instructions from executing.

## 🏛️ Project Structure

The simulator is built using several C++ classes, each modeling a key component of the datapath or control logic:

* **`insMem`**: Simulates the Instruction Memory. Fetches one instruction at a time based on the PC.
* **`regFile`**: Simulates the 32-register Register File. Handles reading from `rs1`, `rs2` and writing to `rd`.
* **`mainmem`**: Simulates the Main Data Memory. Handles `lw` (loads) and `sw` (stores).
* **`controlPath`**: The main Control Unit. Generates all control signals (e.g., `RegWrite`, `MemRead`, `MemWrite`, `ALUSrc`, `Branch`, `Jump`) based on the instruction's opcode.
* **`ALU`**: The Arithmetic Logic Unit. Performs all arithmetic and logical operations, as well as comparisons for branches.
* **`ALUcontrol`**: Generates the specific 4-bit operation code for the ALU based on `funct3`, `funct7`, and the `AluOp` signal.
* **`immgen`**: The Immediate Generator. Parses the 32-bit instruction to extract and sign-extend the correct immediate value based on the instruction format (I, S, B, J).
* **Pipeline Registers**:
    * `ifid`: Stores instruction and PC between IF and ID.
    * `idex`: Stores control signals, register values, and immediate between ID and EX.
    * `exmo`: Stores ALU results, data, and control signals between EX and MEM.
    * `mowb`: Stores ALU results or loaded data between MEM and WB.

## 💡 How to Use

1.  **Prepare Machine Code:**
    * The simulator reads 32-bit binary machine code instructions from a text file.
    * Ensure your RISC-V machine code (one 32-bit binary string per line) is in a file named `output.txt` (for Code 1) or `output1.txt` (for Code 2) in the same directory.

2.  **Compile the Code:**
    * Use a C++ compiler like g++.
    ```bash
    g++ your_simulator_file.cpp -o simulator
    ```

3.  **Run the Simulation:**
    ```bash
    ./simulator
    ```

4.  **Check the Output:**
    * The program will execute the pipeline until completion.
    * It will then print the total number of **cycles** taken.
    * Finally, it will display the final values held in all 32 **registers** (`x0` through `x31`) and dump the contents of the first 100 addresses of the **data memory**.
