#ifndef VM_OPCODES_H_INCLUDED
#define VM_OPCODES_H_INCLUDED

enum vm_opcodes
{
    NOP=0,
    ADD,
    SUB,
    MUL,
    DV,
    MD,
    SHL,
    SHR,
    MOV,
    JMP,
    JE,
    JNE,
    JLE,
    JL,
    JBE,
    JB,
    JZ,
    JNZ,
    SLT,
    SBT,
    SLE,
    SBE,
    SZE, // Set 1 if REG equal ZERO
    CALL,
    RET,
    LOOP,
    PUSH,
    POP,
    ADDS,
    SUBS,
    MULS,
    MULI,
    DIVS,
    ADDF,
    ADDIF, // Add Immediate Float
    SUBF,
    SUBI,
    MULF,
    DIVF,
    LOAD,
    STOR,
    ADDI,
    VMINT,
};

enum VmInts
{
    PRINTINT,
    PRINTFLT,
    PRINTSTR,
    PRINTCHR,
    EXIT,
};


#endif // VM_OPCODES_H_INCLUDED
