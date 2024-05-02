#ifndef VM_COMPILER_H_INCLUDED
#define VM_COMPILER_H_INCLUDED

#include "vm_opcodes.h"
#include "vm.h"
#include "semantic.h"


class MadaVmCompiler
{
    private:
        Ast ast;

        vector<ByteCode> byteCodes;
        map<string, Variable> vars;
        map<string, int> inMemVars;
        int curVar=0;
        vector<int> stackOffsets; // local vars

    public:


        MadaVmCompiler()
        {

        }

        MadaVmCompiler(Ast ast, map<string, Variable> vars)
        {
            this->ast = ast;
            this->vars = vars;
        }

        void algorithm_block(AstNode* astRoot)
        {
            for(auto i: astRoot->getSentences())
            {
                if(i)
                {
                    compile(i);
                }
            }

            this->byteCodes.push_back(ByteCode(ADDI, R1, ZERO, EXIT, false)); // addi $r1, $zero, EXIT
            this->byteCodes.push_back(ByteCode(VMINT, R1));                     // vmint $r1
        }

        AstNodeType compileBinOp(AstNode* astRoot)
        {
            AstNodeType res=ast_none, op1, op2;

            op1 = compile(astRoot->getLeft());
            op2 = compile(astRoot->getRight());

            switch(astRoot->getToken().getType())
            {
                case ASTER:
                    if(op1==ast_number && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));
                        this->byteCodes.push_back(ByteCode(POP, R3));
                        this->byteCodes.push_back(ByteCode(MUL, R1, R2, R3));
                        this->byteCodes.push_back(ByteCode(PUSH, R1));


                        res = ast_number;
                    }else if(op1==ast_flnumber && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));
                        this->byteCodes.push_back(ByteCode(POP, R3));
                        this->byteCodes.push_back(ByteCode(MUL, F1, F2, R3));
                        this->byteCodes.push_back(ByteCode(PUSH, F1));

                        res = ast_flnumber;
                    }
                    else if(op1==ast_number && op2 ==ast_flnumber)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(MULF, F1, R2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));

                        res = ast_flnumber;
                    }
                    else
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(MULF, F1, F2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    break;
                case PLUS:
                    if(op1==ast_number && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));
                        this->byteCodes.push_back(ByteCode(POP, R3));
                        this->byteCodes.push_back(ByteCode(ADD, R1, R2, R3));
                        this->byteCodes.push_back(ByteCode(PUSH, R1));
                        res = ast_number;



                    }else if(op1==ast_flnumber && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, R3));


                        this->byteCodes.push_back(ByteCode(ADDF, F1, F2, R3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    else if(op1==ast_number && op2 ==ast_flnumber)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(ADDF, F1, R2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    else
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(ADDF, F1, F2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    break;
                case MINUS:

                    if(op1==ast_number && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, R3));


                        this->byteCodes.push_back(ByteCode(SUB, R1, R2, R3));


                        this->byteCodes.push_back(ByteCode(PUSH, R1));
                        res = ast_number;



                    }else if(op1==ast_flnumber && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, R3));


                        this->byteCodes.push_back(ByteCode(SUBF, F1, F2, R3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    else if(op1==ast_number && op2 ==ast_flnumber)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(SUBF, F1, R2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    else
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(SUBF, F1, F2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    break;
                case RBAR:
                    if(op1==ast_number && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, R3));



                        this->byteCodes.push_back(ByteCode(DIVF, F1, R2, R3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;



                    }else if(op1==ast_flnumber && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, R3));


                        this->byteCodes.push_back(ByteCode(DIVF, F1, F2, R3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    else if(op1==ast_number && op2 == ast_flnumber)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(DIVF, F1, R2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;

                    }
                    else
                    {
                        this->byteCodes.push_back(ByteCode(POP, F2));


                        this->byteCodes.push_back(ByteCode(POP, F3));


                        this->byteCodes.push_back(ByteCode(DIVF, F1, F2, F3));


                        this->byteCodes.push_back(ByteCode(PUSH, F1));
                        res = ast_flnumber;


                    }
                    break;
                case DIV:
                    if(op1==ast_number && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));


                        this->byteCodes.push_back(ByteCode(POP, R3));


                        this->byteCodes.push_back(ByteCode(DV, R1, R2, R3));


                        this->byteCodes.push_back(ByteCode(PUSH, R1));

                        res = ast_number;

                    }
                    break;
                case MOD:
                    if(op1==ast_number && op2 ==ast_number)
                    {
                        this->byteCodes.push_back(ByteCode(POP, R2));
                        this->byteCodes.push_back(ByteCode(POP, R3));
                        this->byteCodes.push_back(ByteCode(MD, R1, R2, R3));
                        this->byteCodes.push_back(ByteCode(PUSH, R1));
                        res = ast_number;
                    }
                    break;
            }

            return res;
        }

        AstNodeType compile(AstNode* astRoot)
        {
            AstNodeType res=ast_none;

            switch(astRoot->getType())
            {
                case ast_algorithm_block:
                    algorithm_block(astRoot);
                    break;
                case ast_vardeclblock:

                    for(auto i: astRoot->getSentences())
                    {
                        if(i)
                        {
                            res = compile(i);
                        }
                    }

                    break;
                case ast_vardecl:

                    for(auto i: astRoot->getSentences())
                    {
                        if(i)
                        {
                            this->inMemVars[i->getToken().getText()] = this->curVar;
                            this->curVar++;
                        }
                    }



                    break;
                case ast_forloop:
                    /*
                    this->byteCodes.push_back(ByteCode(ADD, R1, ZERO, PC));
                    this->byteCodes.push_back(ByteCode(ADDI, R1, R1, 3, false));
                    this->byteCodes.push_back(ByteCode(PUSH, R1));*/


                    res = compile(astRoot->getLeft()); //valor de inicio

                    this->byteCodes.push_back(ByteCode(POP, R4)); // valor de inicio var = R4

                    //this->byteCodes.push_back(ByteCode(PUSH, R4));

                    this->byteCodes.push_back(ByteCode(ADD, R7, ZERO, PC));
                    this->byteCodes.push_back(ByteCode(ADDI, R7, R7, 2, false));

                    res = compile(astRoot->getRight()); // Limite

                    this->byteCodes.push_back(ByteCode(POP, R3)); // Limite = R3


                    this->byteCodes.push_back(ByteCode(LOAD, R5, R4));
                    this->byteCodes.push_back(ByteCode(ADD, R1, ZERO, R5));
                    //this->byteCodes.push_back(ByteCode(PUSH, R1));

                    this->byteCodes.push_back(ByteCode(PUSH, R4));
                    this->byteCodes.push_back(ByteCode(PUSH, R7));
                    this->byteCodes.push_back(ByteCode(PUSH, R6));
                    this->byteCodes.push_back(ByteCode(PUSH, R3));
                    for(auto i: astRoot->getSentences())
                    {
                        if(i)
                        {
                            res = compile(i);
                        }
                    }

                    this->byteCodes.push_back(ByteCode(POP, R3));
                    this->byteCodes.push_back(ByteCode(POP, R6));
                    this->byteCodes.push_back(ByteCode(POP, R7));
                    this->byteCodes.push_back(ByteCode(POP, R4));

                    //this->byteCodes.push_back(ByteCode(LOAD, R2, R4));
                    this->byteCodes.push_back(ByteCode(ADDI, R6, R5, 1, false));
                    this->byteCodes.push_back(ByteCode(STOR, R6, R4));
                    this->byteCodes.push_back(ByteCode(JL, R7, R5, R3));

                    //this->byteCodes.push_back(ByteCode(POP, R4));

                    break;
                case ast_varassign:

                    res = compile(astRoot->getLeft());

                    this->byteCodes.push_back(ByteCode(POP, R1));
                    this->byteCodes.push_back(ByteCode(ADDI, R2, ZERO, this->inMemVars[astRoot->getToken().getText()], false));
                    this->byteCodes.push_back(ByteCode(STOR, R1, R2));

                    this->byteCodes.push_back(ByteCode(PUSH, R2));

                    break;
                case ast_callwrite:
                    for(auto i: astRoot->getSentences())
                    {
                        if(i)
                        {
                            res = compile(i);
                            if(res == ast_number)
                            {
                                this->byteCodes.push_back(ByteCode(POP, R1));
                                this->byteCodes.push_back(ByteCode(ADDI, R2, ZERO, PRINTINT, false));
                                this->byteCodes.push_back(ByteCode(VMINT, R2));

                            }
                            else if( res== ast_flnumber)
                            {
                                this->byteCodes.push_back(ByteCode(POP, F1));
                                this->byteCodes.push_back(ByteCode(ADDI, R2, ZERO, PRINTFLT, false));
                                this->byteCodes.push_back(ByteCode(VMINT, R2));

                            }
                        }
                    }
                    break;
                case ast_sentences:
                    for(auto i: astRoot->getSentences())
                    {
                        if(i)
                        {
                            res = compile(i);
                        }
                    }
                    break;
                case ast_binop:

                    res = compileBinOp(astRoot);

                    break;
                case ast_unop:
                    break;
                case ast_var:
                    this->byteCodes.push_back(ByteCode(ADDI, R2, ZERO, this->inMemVars[astRoot->getToken().getText()], false));
                    this->byteCodes.push_back(ByteCode(LOAD, R1, R2 ) );
                    this->byteCodes.push_back(ByteCode(PUSH, R1));

                    res = (AstNodeType)this->vars[astRoot->getToken().getText()].getElemType();

                    break;
                case ast_number:
                    this->byteCodes.push_back(ByteCode(ADDI, R1, ZERO, astRoot->getInt(), false));//set R1 to value
                    this->byteCodes.push_back(ByteCode(PUSH, R1));


                    res = ast_number;

                    break;
                case ast_flnumber:
                    this->byteCodes.push_back(ByteCode(ADDIF, F1, ZERO, formatIEEE(astRoot->getFloat()), true));//set R1 to value



                    this->byteCodes.push_back(ByteCode(PUSH, F1));


                    res = ast_flnumber;
                    break;
                case ast_bool:

                    break;

            }
            return res;
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

        vector<ByteCode> getByteCodes()
        {
            return this->byteCodes;
        }

        void showBytecodes()
        {
            for(int i=0; i<this->byteCodes.size(); i++)
            {
                switch(this->byteCodes[i].getType())
                {
                    case bcode_reg:
                        showBcodeRType(this->byteCodes[i]);
                        break;
                    case bcode_stack:
                        showBcodeSType(this->byteCodes[i]);
                        break;
                    case bcode_mem:
                        showBcodeMType(this->byteCodes[i]);
                        break;
                }

            }

        }
};

#endif // VM_COMPILER_H_INCLUDED
