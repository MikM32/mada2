#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <iostream>
#include <stack>
#include <vector>
#include <iomanip>
#include "vm_opcodes.h"

#define MAX_REGS 14
#define MAX_STACK 1024
#define MEM_MAX 2048

using namespace std;

enum byteCodeType
{
    bcode_reg,  // Operar con registros
    bcode_jmp,  // Operar saltos
    bcode_mem,  // Operar lecturas y escrituras en memoria
    bcode_stack // Operar con la pila (reservar y liberar espacio en la pila)
};

enum Registers
{
    ZERO=0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    F1,
    F2,
    F3,
    SP,
    BP,
    PC,

};

union aux{
        int res;
        float flt;
};

int formatIEEE(float flt)
{
    union aux unres;

    unres.flt = flt;

    return unres.res;
}

float deformatIEEE(int fint)
{
    union aux unres;

    unres.res = fint;
    return unres.flt;
}

class ByteCode
{
    private:
        vm_opcodes opcode=NOP;
        byteCodeType type;
        bool isFloatImm;

        int exceptions=0;
        int dir=-1;
        int reg1=-1, reg2=-1, reg3=-1;
        int imm=-1;
    public:

        ByteCode()
        {

        }

        ByteCode(vm_opcodes opcode, int reg1) // stack type
        {

            this->opcode = opcode;
            this->type = bcode_stack;
            this->reg1 = reg1;
        }

        ByteCode(vm_opcodes opcode, int reg1, int reg2, int reg3)
        {
            this->opcode = opcode;
            this->type = bcode_reg;
            this->reg1 = reg1;
            this->reg2 = reg2;
            this->reg3 = reg3;
        }

        ByteCode(vm_opcodes opcode, int reg1, int reg2, int imm, bool isFlt)
        {
            this->opcode = opcode;
            this->type = bcode_reg;
            this->reg1 = reg1;
            this->reg2 = reg2;
            this->imm = imm;
            this->isFloatImm = isFlt;
        }

        /*
        ByteCode(vm_opcodes opcode, int reg1, int imm)
        {
            this->opcode = opcode;
            this->type = bcode_mem;
            this->reg1 = reg1;
            this->imm = imm;
        }*/

        ByteCode(vm_opcodes opcode, int reg1, int reg2)
        {
            this->opcode = opcode;
            this->type = bcode_mem;
            this->reg1 = reg1;
            this->reg2 = reg2;
        }

        void setOpcode(vm_opcodes opcode)
        {
            this->opcode = opcode;
        }

        void setType(byteCodeType type)
        {
            this->type = type;
        }

        void setDir(int dir)
        {
            this->dir = dir;
        }

        void setReg1(int reg)
        {
            this->reg1 = reg;
        }

        void setReg2(int reg)
        {
            this->reg2 = reg;
        }

        void setImm(int imm, bool isFlt)
        {
            this->isFloatImm = isFlt;
            this->imm = imm;
        }

        vm_opcodes getOpcode()
        {
            return this->opcode;
        }

        byteCodeType getType()
        {
            return this->type;
        }

        int getReg1()
        {
            return this->reg1;
        }

        int getReg2()
        {
            return this->reg2;
        }

        int getReg3()
        {
            return this->reg3;
        }

        int getImm()
        {
            return this->imm;
        }

        bool getIsFlt()
        {
            return this->isFloatImm;
        }

        void showBcodeRType(ByteCode bcode)
        {
            switch(bcode.getOpcode())
            {
                    case ADD:
                        cout << setw(3) <<"ADD" << setw(3)<< bcode.getReg1() << setw(3)<< bcode.getReg2()<< setw(3) << bcode.getReg3() <<endl;
                        break;
                    case ADDI:
                        cout << setw(3) <<"ADDI" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << setw(3) << bcode.getImm() <<endl;
                        break;
                    case ADDIF:
                        cout << setw(3) <<"ADDIF" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << setw(3) << bcode.getImm() << endl;
                        break;
                    case DIVF:
                        cout << setw(3) <<"DIVF" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << setw(3) << bcode.getReg3() << endl;
                        break;
                    case JLE:
                        cout << setw(3) <<"JLE" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << setw(3) << bcode.getReg3() << endl;
                        break;
                    case JL:
                        cout << setw(3) <<"JL" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << setw(3) << bcode.getReg3() << endl;
                        break;
            }
        }

        void showBcodeMType(ByteCode bcode)
        {
            switch(bcode.getOpcode())
            {
                case STOR:
                    cout << setw(3) <<"STOR" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << endl;
                    break;
                case LOAD:
                    cout << setw(3) <<"LOAD" << setw(3) << bcode.getReg1() << setw(3) << bcode.getReg2() << endl;
                    break;
            }
        }

        void showBcodeSType(ByteCode bcode)
        {
            switch(bcode.getOpcode())
            {
                    case PUSH:
                        cout << setw(3) <<"PUSH" << setw(3) << bcode.getReg1() << endl;
                        break;
                    case POP:
                        cout << setw(3) << "POP" << setw(3) << bcode.getReg1() << endl;
                        break;
                    case VMINT:
                        cout << setw(3) << "VMINT" << setw(3) << bcode.getReg1() << endl;
                        break;
            }
        }

        void showBytecode(ByteCode bcode)
        {

                switch(bcode.getType())
                {
                    case bcode_reg:
                        showBcodeRType(bcode);
                        break;
                    case bcode_stack:
                        showBcodeSType(bcode);
                        break;
                    case bcode_mem:
                        showBcodeMType(bcode);
                        break;
                }

        }


};

class Vm
{
    private:
        int regs[MAX_REGS]={0};
        int vstack[MAX_STACK]={0,0,0,0,0,0,0};
        bool isRunning = true;
        int errors=0;
        int mem[MEM_MAX];
        vector<ByteCode> byteCodes;

    public:

        Vm()
        {

        }

        Vm(vector<ByteCode> byteCodes)
        {
            this->byteCodes = byteCodes;
        }

        void vm_interrupt(VmInts intcode)
        {
            switch(intcode)
            {
                case PRINTINT:
                    cout << this->regs[R1];
                    break;
                case PRINTFLT:
                    cout << deformatIEEE(this->regs[F1]);
                    break;
                case EXIT:
                    this->isRunning = false;
                    break;
            }
        }

        void evalBcode(ByteCode bcode)
        {
            float aux=0, aux1=0, aux2=0;
            switch(bcode.getOpcode())
            {
                case NOP:
                    // No Operation
                    break;
                case ADD:
                    this->regs[bcode.getReg1()] = this->regs[bcode.getReg2()] + this->regs[bcode.getReg3()];

                    this->regs[PC]++;
                    break;
                case ADDI:
                    this->regs[bcode.getReg1()] = this->regs[bcode.getReg2()] + bcode.getImm();

                    this->regs[PC]++;
                    break;
                case ADDIF:
                    if(bcode.getReg2()>= F1 && bcode.getReg2()<=F3)
                    {
                        this->regs[bcode.getReg1()] = formatIEEE(deformatIEEE(this->regs[bcode.getReg2()]) + deformatIEEE(bcode.getImm()));
                    }
                    else
                    {
                        this->regs[bcode.getReg1()] = formatIEEE(this->regs[bcode.getReg2()] + deformatIEEE(bcode.getImm()));
                    }

                    this->regs[PC]++;
                    break;
                case ADDF:

                    if(bcode.getReg3()>= F1 && bcode.getReg3()<=F3)
                    {
                        aux += deformatIEEE(this->regs[bcode.getReg3()]);
                    }
                    else
                    {
                        aux +=this->regs[bcode.getReg3()];
                    }
                    if(bcode.getReg2()>= F1 && bcode.getReg2()<=F3)
                    {
                        aux += deformatIEEE(this->regs[bcode.getReg2()]);
                    }
                    else
                    {
                        aux +=this->regs[bcode.getReg2()];
                    }

                    this->regs[bcode.getReg1()] = formatIEEE(aux);

                    this->regs[PC]++;
                    break;
                case SUB:
                    this->regs[bcode.getReg1()] = this->regs[bcode.getReg3()] - this->regs[bcode.getReg2()];

                    this->regs[PC]++;
                    break;
                case SUBI:

                    this->regs[bcode.getReg1()] = this->regs[bcode.getReg3()] - this->regs[bcode.getImm()];

                    this->regs[PC]++;
                    break;
                case DIVF:

                    if(bcode.getReg3()>= F1 && bcode.getReg3()<=F3)
                    {
                        aux1 = deformatIEEE(this->regs[bcode.getReg3()]);
                    }
                    else
                    {
                        aux1 =this->regs[bcode.getReg3()];
                    }
                    if(bcode.getReg2()>= F1 && bcode.getReg2()<=F3)
                    {
                        aux2 = deformatIEEE(this->regs[bcode.getReg2()]);
                    }
                    else
                    {
                        aux2 =this->regs[bcode.getReg2()];
                    }

                    this->regs[bcode.getReg1()] = formatIEEE(aux1/aux2);

                    this->regs[PC]++;
                    break;
                case MUL:
                    this->regs[bcode.getReg1()] = this->regs[bcode.getReg3()] * this->regs[bcode.getReg2()];

                    this->regs[PC]++;
                    break;
                case PUSH:
                    this->regs[SP]++;
                    this->vstack[this->regs[SP]] = this->regs[bcode.getReg1()];

                    this->regs[PC]++;
                    break;
                case POP:
                    this->regs[bcode.getReg1()] = this->vstack[this->regs[SP]];
                    this->regs[SP]--;

                    this->regs[PC]++;
                    break;
                case VMINT:
                    vm_interrupt((VmInts)this->regs[bcode.getReg1()]);
                    this->regs[PC]++;
                    break;
                case JMP:

                    if(this->regs[bcode.getReg1()] < 0 || this->regs[bcode.getReg1()] >= this->byteCodes.size() )
                    {
                        this->isRunning=false;
                        errorPcOverflow();
                        this->errors++;
                    }
                    else
                    {
                        this->regs[PC] = this->regs[bcode.getReg1()];
                    }

                    break;
                case JNZ:

                    if(this->regs[bcode.getReg1()] < 0 || this->regs[bcode.getReg1()] >= this->byteCodes.size() )
                    {
                        this->isRunning=false;
                        errorPcOverflow();
                        this->errors++;
                    }
                    else
                    {
                        if(!this->regs[bcode.getReg2()])
                        {
                            this->regs[PC] = this->regs[bcode.getReg1()];
                        }
                        else
                        {
                            this->regs[PC]++;
                        }

                    }
                case JLE:
                    if(this->regs[bcode.getReg1()] < 0 || this->regs[bcode.getReg1()] >= this->byteCodes.size() )
                    {
                        this->isRunning=false;
                        errorPcOverflow();
                        this->errors++;
                    }
                    else
                    {
                        if(this->regs[bcode.getReg2()] <= this->regs[bcode.getReg3()])
                        {
                            this->regs[PC] = this->regs[bcode.getReg1()];
                        }
                        else
                        {
                            this->regs[PC]++;
                        }

                    }
                    break;
                case JL:
                    if(this->regs[bcode.getReg1()] < 0 || this->regs[bcode.getReg1()] >= this->byteCodes.size() )
                    {
                        this->isRunning=false;
                        errorPcOverflow();
                        this->errors++;
                    }
                    else
                    {
                        if(this->regs[bcode.getReg2()] < this->regs[bcode.getReg3()])
                        {
                            this->regs[PC] = this->regs[bcode.getReg1()];
                        }
                        else
                        {
                            this->regs[PC]++;
                        }

                    }
                    break;
                case STOR:
                    this->mem[this->regs[bcode.getReg2()]] = this->regs[bcode.getReg1()];
                    this->regs[PC]++;

                    break;
                case LOAD:
                    this->regs[bcode.getReg1()] = this->mem[this->regs[bcode.getReg2()]];
                    this->regs[PC]++;
                    break;

            }
        }

        void eval(bool debug)
        {
            while(this->isRunning)
            {
                if(debug)
                {
                    this->byteCodes[this->regs[PC]].showBytecode(this->byteCodes[this->regs[PC]]);
                    showRegs();
                }
                evalBcode(this->byteCodes[this->regs[PC]]);

            }
        }

        void showRegs()
        {
            cout << endl << "Registers: " << endl;
            for(auto i: this->regs)
            {
                cout << i <<endl;
            }
        }

        void errorPcOverflow()
        {
            cout << "Mada VM: Error: Direccion de PC fuera de los limites." <<endl;
        }

};

#endif // VM_H_INCLUDED
