#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include <iostream>
#include <map>
#include "semantic.h"

using namespace std;



class Interpreter
{
    private:
        Ast ast;
        map<string, Variable> vars;

    public:

        Interpreter()
        {

        }


        AstElemUnion evalBinOp(AstNode* binop)
        {
            AstElemUnion res, op1, op2;

            switch(binop->getToken().getType())
            {
                case ASTER:
                    op1 = this->eval(binop->getLeft());
                    op2 = this->eval(binop->getRight());

                    if(op1)

                    break;
            }

        }

        AstElemUnion eval(AstNode* sentence)
        {
            switch(sentence->getType())
            {
                case ast_binop:
                    return evalBinOp(sentence);
                    break;
                case ast_unop:
                    break;
                case ast_algorithm_block:
                    break;
                case ast_vardeclblock:
                    break;
                case ast_vardecl:
                    break;
                case ast_number:
                case ast_flnumber:
                case ast_bool:
                    return sentence->getElem();
                    break;
                case ast_callwrite:
                    break;


            }

        }

};

#endif // INTERPRETER_H_INCLUDED
