#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED

#include <iostream>
#include <map>
#include "ast.h"
using namespace std;

class Variable
{
    private:
        AstElemUnion value;
        AstNodeType elemType;
        int scope=0;
        int type;
    public:

        Variable()
        {

        }

        Variable(int type)
        {
            this->type = type;
        }


        Variable(int i, int type)
        {
            this->value.integer = i;
            this->type = type;
        }

        Variable(char c, int type)
        {
            this->value.chr = c;
            this->type = type;
        }

        Variable(bool b, int type)
        {
            this->value.boolean = b;
            this->type = type;
        }

        Variable(double d, int type)
        {
            this->value.flt = d;
            this->type = type;
        }

        int getType()
        {
            return this->type;
        }

        int getInt()
        {
            return this->value.integer;
        }

        bool getBool()
        {
            return this->value.boolean;
        }

        char getChr()
        {
            return this->value.chr;
        }

        double getFloat()
        {
            return this->value.flt;
        }

        int getScope()
        {
            return this->scope;
        }

        AstNodeType getElemType()
        {
            return this->elemType;
        }

        void setScope(int s)
        {
            this->scope = s;
        }

        void setElemType(AstNodeType e)
        {
            this->elemType = e;
        }

};

class Semantic
{
    private:
        Ast ast;
        map<string, Variable> vars;

        int errors;
    public:

    Semantic()
    {
        this->errors = 0;
    }

    Semantic(Ast ast)
    {
        this->ast = ast;
        this->errors = 0;
    }

    int getErrors()
    {
        return this->errors;
    }

    map<string, Variable> getVars()
    {
        return this->vars;
    }

    AstNodeType checkBinOp(AstNode* astRoot)
    {
        int op1, op2;
        AstElemUnion e;
        op1 = this->check(astRoot->getLeft());
        op2 = this->check(astRoot->getRight());

        switch(astRoot->getToken().getType())
        {

            case ASTER:
            case PLUS:
            case MINUS:
            case RBAR:
            case LESS:
            case LEQUAL:
            case BIGGER:
            case BEQUAL:
            case EQUAL:

                switch(op1+op2)
                {
                    case ast_number+ast_number:

                        return ast_number;
                        break;
                    case ast_number+ast_flnumber:

                        return ast_flnumber;
                    case ast_flnumber+ast_flnumber:

                        return ast_flnumber;
                        break;
                    default:
                        this->errors++;
                        semanticErrorInvalidType(astRoot->getToken().getLine(), astRoot->getRight()->getToken().getText(), astRoot->getToken().getType());

                        return ast_none;
                        break;
                }
                /*
                if(!(op1 == ast_number || op1 == ast_flnumber))
                {
                    this->errors++;
                    semanticErrorInvalidType(astRoot->getToken().getLine(), astRoot->getLeft()->getToken().getText(), astRoot->getToken().getType());
                    break;
                }
                else if(!(op2 == ast_number || op2 == ast_flnumber))
                {
                    this->errors++;
                    semanticErrorInvalidType(astRoot->getToken().getLine(), astRoot->getRight()->getToken().getText(), astRoot->getToken().getType());
                    break;
                }

                return ast_number;*/
                break;
            //booleanos
            default:
                break;
        }

        return ast_none;
    }

    AstNodeType checkUnOp(AstNode* a)
    {
        AstNodeType res = ast_none;
        int op1 = check(a->getLeft());

        switch(a->getToken().getType())
        {
            case MINUS:


                switch(op1)
                {
                    case ast_number:
                    case ast_flnumber:
                        res = (AstNodeType)op1;
                        break;
                    default:
                        this->errors++;
                        semanticErrorInvalidType(a->getToken().getLine(), a->getLeft()->getToken().getText(), a->getToken().getType());
                        break;
                }
                break;
        }

        return res;
    }

    AstNodeType varDecl(AstNode* vardecl)
    {
        for(auto i: vardecl->getSentences())
        {
            if(i != nullptr)
            {
                TokenType t = vardecl->getToken().getType();
                Variable v = Variable( t);

                switch(t)
                {
                    case ENTERO:
                        v.setElemType(ast_number);
                        break;
                    case REAL:
                        v.setElemType(ast_flnumber);
                        break;
                    default:
                        break;
                }
                this->vars[i->getToken().getText()] =  v;
            }

        }

        return ast_vardecl;
    }

    AstNodeType varDeclBlock(AstNode* varblock)
    {
        for(auto i : varblock->getSentences())
        {
            if(i != nullptr)
            {
                this->check(i);
            }
        }

        return ast_vardeclblock;
    }


    AstNodeType check(AstNode* astRoot)
    {
        list<AstNode*> sentences;
        AstNodeType res = ast_none;
        map<string, Variable>::iterator a;

        switch(astRoot->getType())
        {
            case ast_unop:
                res = this->checkUnOp(astRoot);
                astRoot->setResType(res);
                break;
            case ast_binop:

                res = this->checkBinOp(astRoot);
                astRoot->setResType(res);

                break;
            case ast_sentences:

                sentences = astRoot->getSentences();

                for(auto i : sentences)
                {
                    if(i != nullptr)
                    {
                       res = check(i);
                    }

                }

                break;
            case ast_varassign:

                a = this->vars.find(astRoot->getToken().getText());

                if(a != this->vars.end())
                {
                    //a = this->vars.at(astRoot->getToken().getText());
                    if(a->second.getScope() != astRoot->getScope())
                    {
                        semanticErrorVarWrongScope(astRoot->getToken().getLine(), astRoot->getToken().getText());
                        this->errors++;
                    }
                    else
                    {
                        res = check(astRoot->getLeft());
                        if(a->second.getElemType() == ast_flnumber && !(res == ast_number || res == ast_flnumber))
                        {
                            cout << "error tipos no coinciden" <<endl;
                        }
                        else if(a->second.getElemType() == ast_number && res!=ast_number)
                        {
                            cout << "error tipos no coinciden" <<endl;
                        }
                        //a->second.getType()
                    }

                }
                else
                {
                    semanticErrorVarNotDec(astRoot->getToken().getLine(), astRoot->getToken().getText());
                    this->errors++;
                }

                break;
            case ast_var:

                a = this->vars.find(astRoot->getToken().getText());

                if(a != this->vars.end())
                {
                    //a = this->vars.at(astRoot->getToken().getText());
                    if(a->second.getScope() != astRoot->getScope())
                    {
                        semanticErrorVarWrongScope(astRoot->getToken().getLine(), astRoot->getToken().getText());
                        this->errors++;
                    }
                    else
                    {
                        res = a->second.getElemType();
                        //a->second.getType()
                    }

                }
                else
                {
                    semanticErrorVarNotDec(astRoot->getToken().getLine(), astRoot->getToken().getText());
                    this->errors++;
                }

                break;
            case ast_vardeclblock:
                res = this->varDeclBlock(astRoot);
                break;
            case ast_vardecl:
                res = this->varDecl(astRoot);
                break;

            case ast_flnumber:
                return ast_flnumber;
                break;
            case ast_number:
                return ast_number;
                break;
            case ast_algorithm_block:

                for(auto part: astRoot->getSentences())
                {
                    if(part != nullptr)
                    {
                        res = check(part);
                    }
                }

                break;
            case ast_bool:
                return ast_bool;
                break;
            default:
                return ast_none;
                break;
        }

        return res;


    }

    void showVars()
    {
        for(auto& i: this->vars)
        {
            cout << i.first << setw(5) << " " << i.second.getType() << endl;
        }
    }
};


#endif // SEMANTIC_H_INCLUDED
