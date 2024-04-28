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

        void setScope(int s)
        {
            this->scope = s;
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

    void checkBinOp(AstNode* astRoot)
    {
        int op1, op2;
        op1 = this->check(astRoot->getLeft());
        op2 = this->check(astRoot->getRight());
        switch(astRoot->getToken().getType())
        {

            case ASTER:
            case PLUS:
            case MINUS:
            case RBAR:
                if(!(op1 == ast_number || op1 == ast_flnumber))
                {
                    this->errors++;
                    semanticErrorInvalidType(astRoot->getToken().getLine(), astRoot->getLeft()->getToken().getText(), astRoot->getToken().getType());
                }
                else if(!(op2 == ast_number || op2 == ast_flnumber))
                {
                    this->errors++;
                    semanticErrorInvalidType(astRoot->getToken().getLine(), astRoot->getRight()->getToken().getText(), astRoot->getToken().getType());
                }
                break;
            default:
                break;
        }
    }

    void varDecl(AstNode* vardecl)
    {
        for(auto i: vardecl->getSentences())
        {
            if(i != nullptr)
            {
                this->vars[i->getToken().getText()] = Variable(i->getToken().getType());
            }

        }
    }

    void varDeclBlock(AstNode* varblock)
    {
        for(auto i : varblock->getSentences())
        {
            if(i != nullptr)
            {
                this->check(i);
            }
        }
    }


    AstNodeType check(AstNode* astRoot)
    {
        list<AstNode*> sentences;
        map<string, Variable>::iterator a;

        switch(astRoot->getType())
        {
            case ast_binop:

                this->checkBinOp(astRoot);

                break;
            case ast_sentences:

                sentences = astRoot->getSentences();

                for(auto i : sentences)
                {
                    if(i != nullptr)
                    {
                       check(i);
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
                        check(astRoot->getLeft());
                    }

                }
                else
                {
                    semanticErrorVarNotDec(astRoot->getToken().getLine(), astRoot->getToken().getText());
                    this->errors++;
                }

                break;

            case ast_vardeclblock:
                this->varDeclBlock(astRoot);
                break;
            case ast_vardecl:
                this->varDecl(astRoot);
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
                        check(part);
                    }
                }

                break;
            case ast_bool:
                return ast_bool;
                break;
            default:
                break;
        }

        return ast_none;
    }
};


#endif // SEMANTIC_H_INCLUDED
