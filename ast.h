#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED


#include <iostream>
#include <list>

using namespace std;

typedef union
{
    int integer;
    double flt;
    char chr;
    bool boolean;
    string* str;

}AstElemUnion;

enum AstNodeType
{
    ast_none=0,
    ast_number=1,
    ast_bool,
    ast_binop,
    ast_unop,
    ast_str,
    ast_flnumber,
    ast_expr,
    ast_var,
    ast_callproc,
    ast_callwrite,
    ast_forloop,
    ast_sentences,
    ast_varassign,
    ast_vardecl,
    ast_vardeclblock,
    ast_algorithm_block
};

class AstElemObj
{
    private:
        AstElemUnion elem;
    public:

        AstElemObj()
        {

        }

        AstElemObj(int i)
        {
            this->elem.integer = i;
        }

        AstElemObj(double f)
        {
            this->elem.flt = f;
        }

        AstElemObj(char c)
        {
            this->elem.chr = c;
        }

        AstElemObj(bool b)
        {
            this->elem.boolean = b;
        }

        char getChar()
        {
            return this->elem.chr;
        }

        float getFloat()
        {
            return this->elem.flt;
        }

        int getInt()
        {
            return this->elem.integer;
        }

        bool getBoolean()
        {
            return this->elem.boolean;
        }

        AstElemUnion getElem()
        {
            return this->elem;
        }

        void showElemInfo(AstNodeType t)
        {
            switch(t)
            {
                case ast_number:
                    cout << this->elem.integer << endl;
                    break;
                case ast_flnumber:
                    cout << this->elem.flt << endl;
                    break;
                default:
                    break;

            }
        }

};

class AstNode
{
    private:
        AstElemUnion elem;
        AstNode* right;
        AstNode* left;
        list<AstNode*> sentences;
        string id;
        int opType;
        Token asocToken;
        int scope=0;

        AstNodeType type;
        AstNodeType resType=ast_none;

    public:

        AstNode()
        {
            this->right = nullptr;
            this->left = nullptr;
        }

        AstNode(int i)
        {
            this->elem.integer = i;
            this->right = nullptr;
            this->left = nullptr;
        }

        AstNode(double f)
        {
            this->elem.flt = f;
            this->right = nullptr;
            this->left = nullptr;
        }

        AstNode(char c)
        {
            this->elem.chr = c;
            this->right = nullptr;
            this->left = nullptr;
        }

        void setElem(AstElemUnion e)
        {
            this->elem = e;
        }

        int getScope()
        {
            return this->scope;
        }

        void setScope(int s)
        {
            this->scope = s;
        }

        char getChar()
        {
            return this->elem.chr;
        }

        float getFloat()
        {
            return this->elem.flt;
        }

        int getInt()
        {
            return this->elem.integer;
        }

        Token getToken()
        {
            return this->asocToken;
        }

        int getOpType()
        {
            return this->opType;
        }

        /*
        AstElemObj getInfo()
        {
            return this->info;
        }*/

        AstNodeType getType()
        {
            return this->type;
        }

        AstNodeType getResType()
        {
            return this->resType;
        }

        AstNode* getRight()
        {
            return this->right;
        }

        AstNode* getLeft()
        {
            return this->left;
        }

        string getId()
        {
            return this->id;
        }

        list<AstNode*> getSentences()
        {
            return this->sentences;
        }

        /*
        void setInfo(AstElemObj i)
        {
            this->info = i;
        }*/
        void setId(string id)
        {
            this->id = id;
        }
        void setOpType(int op)
        {
            this->opType = op;
        }
        void setRight(AstNode* right)
        {
            this->right = right;
        }

        void setLeft(AstNode* left)
        {
            this->left = left;
        }

        void setType(AstNodeType type)
        {
            this->type = type;
        }

        void setResType(AstNodeType resT)
        {
            this->resType = resT;
        }

        void setToken(Token t)
        {
            this->asocToken = t;
        }

        void addArg(AstNode* arg)
        {
            this->sentences.push_front(arg);
        }

        void addArgEnd(AstNode* arg)
        {
            this->sentences.push_back(arg);
        }




};


class Ast
{
    private:
        AstNode* root;
    public:

        Ast()
        {

        }

        Ast(AstNode* root)
        {
            this->root = root;
        }

        AstNode* getRoot()
        {
            return this->root;
        }
};


#endif // AST_H_INCLUDED
