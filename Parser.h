#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "ast.h"

using namespace std;

int strToInt(string str)
{
    int res=0, bas=1;

    for(int i=str.size()-1; i >= 0 ; i--)
    {
        res += (str[i] & 0xF) * bas;
        bas*=10;

    }

    return res;
}

double strToReal(string str)
{
    double res=0;
    int dotpos=0;
    int bas=1;

    for(int i=0; i<(int)str.size(); i++)
    {
        if(str[i] == '.')
        {
            break;
        }
        dotpos++;
    }

    for(int i=dotpos-1; i >= 0 ; i--)
    {

        res += (str[i] & 0xF) * bas;
        bas*=10;

    }

    bas=1;
    for(int i=dotpos+1; i < (int)str.size() ; i++)
    {
        res+= (double)(str[i] & 0xF) / (double)(10*bas);
        bas*=10;
    }

    return (double)res;

}

TokenType isBuiltinType(Token tok)
{
    TokenType res = tok.getType();
    switch(res)
    {
        case ENTERO:
        case REAL:
        case LOGICO:
        case CARACTER:
            break;
        default:
            res = NONE;
            break;
    }

    return res;
}

class Parser
{
    private:
        vector<Token> tokens;

        int errors=0;
        int tokIndex=0;

        bool isAtEnd=false;

        Token curTok, nextTok;
        Ast ast;

    public:

        Parser()
        {

        }

        void eat()
        {
            this->curTok = this->tokens[this->tokIndex++];
            if(this->tokIndex < this->tokens.size())
            {
                this->nextTok = this->tokens[this->tokIndex];
                this->isAtEnd = true;
            }
            else
            {
                this->nextTok = this->curTok;
            }

        }

        Ast getAst()
        {
            return this->ast;
        }

        int getErrors()
        {
            return this->errors;
        }

        // -------------- Expresiones ------------------

        AstNode* primary()  // Expresion primaria o primordial
        {
            AstNode* res = nullptr;
            double a;


            switch(this->curTok.getType())
            {
                case NUM:
                    res = new AstNode(strToInt(this->curTok.getText()));
                    res->setType(ast_number);
                    res->setToken(this->curTok);
                    break;
                case FLOAT:
                    a = strToReal(this->curTok.getText());
                    res = new AstNode(a);
                    res->setType(ast_flnumber);
                    res->setToken(this->curTok);
                    break;

                case VERDADERO:
                    res = new AstNode(true);
                    res->setType(ast_bool);
                    res->setToken(this->curTok);
                    break;
                case FALSO:
                    res = new AstNode(false);
                    res->setType(ast_bool);
                    res->setToken(this->curTok);
                    break;
                case ID:
                    res = new AstNode(false);
                    res->setType(ast_var);
                    res->setToken(this->curTok);
                    break;
                case OPEN_PAREN:    //Expresion compuesta
                    res = this->equality();
                    eat();
                    if(this->curTok.getType() != CLOS_PAREN)
                    {
                        parseErrorExpected(this->curTok.getLine(), ")", this->curTok.getText());
                        this->errors++;
                        res = nullptr;
                    }
                    break;


                default:
                    parseErrorExpected(this->curTok.getLine(), "expresion", this->curTok.getText());
                    this->errors++;
                    res = nullptr;
                    break;
            }

            return res;
        }

        AstNode* unary() // Expresion unaria o de operaciones unarias
        {
            AstNode* res = nullptr;




            switch(this->curTok.getType())
            {
                case MINUS:
                    //eat();
                    res = new AstNode();

                    res->setToken(this->curTok);
                    res->setOpType(this->curTok.getType());
                    eat();
                    //res = new AstNode();
                    res->setLeft( this->unary() );
                    res->setType(ast_unop);


                    break;
                default:

                    res = this->primary();
                    break;

            }




            return res;
        }

        AstNode* factor() // Expresion de factor o un factor (1*2) el cual es una operacion binaria
        {
            AstNode* res = nullptr, *res_binop = nullptr;

            //eat();
            res = this->unary();

            if(res)
            {
                if(res->getToken().getType() == NONE)
                {
                    res->getToken().showTokenInfo();

                    res->setToken(this->curTok);
                }
                //res->getToken().showTokenInfo();
            }



            switch(this->nextTok.getType())
            {
                case ASTER:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->factor() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(ASTER);


                    return res_binop;
                    break;
                case RBAR:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->factor() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(RBAR);


                    return res_binop;
                    break;
                case DIV:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->factor() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(DIV);


                    return res_binop;
                    break;
                case MOD:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->factor() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(MOD);


                    return res_binop;
                    break;
                default:
                    break;

            }


            return res;
        }

        AstNode* term()
        {
            AstNode* res = nullptr, *res_binop = nullptr;
            //eat();
            res = this->factor();
            if(res)
            {
                if(res->getToken().getType() == NONE)
                {
                    res->setToken(this->curTok);
                }

            }

            switch(this->nextTok.getType())
            {
                case PLUS:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->term() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(PLUS);


                    return res_binop;
                    break;
                case MINUS:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->term() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(MINUS);


                    return res_binop;
                    break;
                default:
                    break;

            }


            return res;

        }

        AstNode* comparation()
        {
            AstNode* res = nullptr, *res_binop = nullptr;

            res = this->term();
            if(res)
            {
                if(res->getToken().getType() == NONE)
                {
                    res->setToken(this->curTok);
                }

            }

            //eat();
            switch(this->nextTok.getType())
            {
                case LESS:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->comparation() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(LESS);


                    return res_binop;
                    break;
                case LEQUAL:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->comparation() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(LEQUAL);


                    return res_binop;
                    break;
                case BIGGER:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->comparation() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(BIGGER);


                    return res_binop;
                    break;
                case BEQUAL:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->comparation() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(BEQUAL);


                    return res_binop;
                    break;
                default:
                    break;

            }

            return res;

        }

        AstNode* equality()
        {
            AstNode* res = nullptr, *res_binop = nullptr;
            eat();
            res = this->comparation();

            if(res)
            {
                if(res->getToken().getType() == NONE)
                {
                    res->setToken(this->curTok);
                }

            }

            //eat();
            switch(this->nextTok.getType())
            {
                case EQUAL:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->equality() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(PLUS);


                    return res_binop;
                    break;
                case DIFF:
                    eat();
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->equality() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(DIFF);


                    return res_binop;
                    break;
                default:
                    break;

            }

            return res;

        }

        // -------------- Sentencias ------------------

        AstNode* for_loop()
        {
            AstNode* res = new AstNode();

            if(this->curTok.getType() != PARA)
            {
                parseErrorExpected(this->curTok.getLine(), "para",this->curTok.getText());
                this->errors++;
                delete res;
                return nullptr;
            }

            res->setToken(this->curTok);
            res->setType(ast_forloop);

            eat();
            res->setLeft(var_assign()); // una asignacion

            //eat();
            if(this->curTok.getType() != HASTA)
            {
                parseErrorExpected(this->curTok.getLine(), "hasta",this->curTok.getText());
                this->errors++;
                delete res;
                return nullptr;
            }

            //eat();
            res->setRight(equality()); // Una expresion


            eat();
            if(this->curTok.getType() != HACER)
            {
                parseErrorExpected(this->curTok.getLine(), "hacer ",this->curTok.getText());
                this->errors++;
                delete res;
                return nullptr;
            }
            eat();

            while(this->curTok.getType() != FPARA)
            {
                if(this->tokIndex >= this->tokens.size())
                {
                    parseErrorExpected(this->curTok.getLine(), "fpara",this->curTok.getText());
                    this->errors++;
                    delete res;
                    return nullptr;
                    break;
                }
                res->addArgEnd(sentence());
            }

            eat();

            return res;

        }

        AstNode* var_assign()
        {
            AstNode* res = new AstNode();

            if(this->curTok.getType() != ID)
            {
                parseErrorExpected(this->curTok.getLine(), "identificador de variable", this->curTok.getText());
                this->errors++;
                delete res;
                return nullptr;
            }

            res->setType(ast_varassign);
            res->setToken(this->curTok);

            eat();

            if(this->curTok.getType() != ASIGN)
            {
                parseErrorExpected(this->curTok.getLine(), "<-", this->curTok.getText());
                this->errors++;
                delete res;
                return nullptr;
            }


            res->setLeft(this->equality());



            eat();

            return res;
        }

        AstNode* sentence() // Evalua sentencias y las guarda de forma secuencial
        {
            AstNode* res = nullptr;
            eat();
            switch(this->curTok.getType())
            {
                case ID:
                    res = new AstNode();

                    switch(this->nextTok.getType())
                    {

                        case OPEN_PAREN: //Proc call

                            res->setType(ast_callproc);
                            res->setToken(this->curTok);

                            eat();
                            eat();
                            while(this->curTok.getType() != COMMA)
                            {
                                res->addArg(this->factor());
                            }

                            if(this->curTok.getType() != CLOS_PAREN)
                            {
                                parseErrorExpected(this->curTok.getLine(), ")",res->getId());
                                this->errors++;
                            }

                            res->setType(ast_callproc);
                            res->setToken(this->curTok);
                            break;
                        case ASIGN:
                            /*
                            res->setType(ast_varassign);
                            res->setToken(this->curTok);

                            eat();

                            res->setLeft(this->equality());

                            if(this->nextTok.getType() != ENDL && res->getLeft() != nullptr)
                            {
                                parseErrorExpected(this->curTok.getLine(), "salto de linea", this->nextTok.getText());
                                this->errors++;
                                return nullptr;
                            }

                            eat();
                            //eat();*/

                            res = var_assign();

                            //eat();
                            if(this->curTok.getType() != ENDL && res->getLeft() != nullptr)
                            {
                                parseErrorExpected(this->curTok.getLine(), "salto de linea", this->curTok.getText());
                                this->errors++;
                                delete res;
                                return nullptr;
                            }

                            break;
                        default:
                            parseErrorExpected(this->curTok.getLine(), "Una llamada o asignacion", this->curTok.getText());
                            this->errors++;
                            return nullptr;
                            break;
                    }





                    break;
                case WRITE:
                    res = new AstNode();
                    res->setId(this->curTok.getText());
                    eat();
                    if(this->curTok.getType() != OPEN_PAREN)
                    {
                        parseErrorExpected(this->curTok.getLine(), "(",res->getId());
                        this->errors++;
                    }

                    res->addArg(this->equality());
                    eat();

                    while(this->curTok.getType() == COMMA && this->curTok.getType() != CLOS_PAREN)
                    {

                        res->addArg(this->equality());
                        eat();
                    }

                    if(this->curTok.getType() != CLOS_PAREN)
                    {
                        parseErrorExpected(this->curTok.getLine(), ")",res->getId());
                        this->errors++;
                    }

                    res->setType(ast_callwrite);
                    res->setToken(this->curTok);
                    break;

                case PARA:

                    res = for_loop();
                    break;

            }

            return res;
        }

        AstNode* var_decl()
        {
            AstNode* res = nullptr;
            //eat();

            TokenType bType = isBuiltinType(this->curTok);

            if(this->curTok.getType() != ID && !bType)
            {
                parseErrorExpected(this->curTok.getLine(), "identificador de tipo", this->curTok.getText());
                this->errors++;
                return res;
            }


            res = new AstNode();
            res->setType(ast_vardecl);
            res->setToken(this->curTok);

            eat();

            if(this->curTok.getType() != DDOT)
            {
                parseErrorExpected(this->curTok.getLine(), "separador de tipo ( : )", this->curTok.getText());
                this->errors++;
                delete res;
                res = nullptr;
                return res;
            }

            eat();

            if(this->curTok.getType() == ENDL)
            {
                parseErrorExpected(this->curTok.getLine(), "identificador de la o las variables", this->curTok.getText());
                this->errors++;
                delete res;
                res = nullptr;
                return res;
            }



            while(this->curTok.getType() == ID)
            {
                AstNode* var = new AstNode();
                var->setType(ast_var);
                var->setToken(this->curTok);

                res->addArg(var);
                eat();
                if(this->curTok.getType() != COMMA)
                {
                    break;
                }
                else if(this->nextTok.getType() != ID)
                {
                    parseErrorExpected(this->curTok.getLine(), "identificador despues de la coma", this->curTok.getText());
                    this->errors++;
                    delete res;
                    res = nullptr;
                    return res;
                }
                else
                {
                    eat();
                    continue;
                }
            }

            return res;

        }

        AstNode* var_decl_block()
        {
            AstNode* res = nullptr;
            eat();

            if(this->curTok.getType() == VAR)
            {
                eat();
                if(this->curTok.getType() != ENDL)
                {
                    parseErrorExpected(this->curTok.getLine(), "salto de linea", this->curTok.getText());
                    this->errors++;
                    return res;
                }

                eat();
                res = new AstNode();
                res->setType(ast_vardeclblock);



                while(this->curTok.getType() != INICIO)
                {
                    if(this->tokIndex >= this->tokens.size())
                    {
                        parseErrorExpected(this->curTok.getLine(), "inicio", this->nextTok.getText());
                        //advertencia de que no se inicio el bloque de codigo principal o main del algoritmo

                        this->errors++;
                        delete res;
                        res = nullptr;
                        break;
                    }

                    res->addArg(this->var_decl());
                    if(this->curTok.getType() != ENDL)
                    {
                        parseErrorExpected(this->curTok.getLine(), "salto de linea", this->curTok.getText());
                        this->errors++;
                        delete res;
                        res = nullptr;
                        break;
                    }
                    eat();

                }
            }

            return res;
        }

        AstNode* code_block() // Bloque de codigo inicio-fin
        {
            AstNode* res = nullptr;


            if(this->curTok.getType() != INICIO)
            {
                parseErrorExpected(this->curTok.getLine(), "inicio", this->curTok.getText());
                this->errors++;
                return nullptr;
            }

            if(this->nextTok.getType() != ENDL)
            {
                parseErrorExpected(this->curTok.getLine(), "salto de linea", this->nextTok.getText());
                this->errors++;
                return nullptr;
            }

            res = new AstNode();
            res->setType(ast_sentences);
            while(this->curTok.getType() != FIN)
            {
                if(this->tokIndex >= this->tokens.size())
                {
                    parseErrorExpected(this->curTok.getLine(), "fin", this->nextTok.getText());
                    this->errors++;
                    res = nullptr;
                    break;
                }
                res->addArgEnd(this->sentence());

            }

            return res;

        }

        //Cuerpo principal del codigo
        AstNode* algorithm_block()  // Bloque algoritmo / tipos /proc / func / const / var / code_block
        {
            AstNode* res = nullptr;
            eat();
            switch(this->curTok.getType())
            {
                case ALGORITMO:
                    eat();
                    if(this->curTok.getType() != ID)
                    {
                        this->errors++;
                        parseErrorExpected(this->curTok.getLine(), "nombre del algoritmo", this->curTok.getText());
                        break;
                    }
                    res = new AstNode();
                    res->setToken(this->curTok);
                    res->setType(ast_algorithm_block);

                    eat();
                    if(this->curTok.getType() != ENDL)
                    {
                        this->errors++;
                        parseErrorExpected(this->curTok.getLine(), "salto de linea", this->curTok.getText());
                        break;
                    }

                    res->addArgEnd(this->var_decl_block());
                    res->addArgEnd(this->code_block());

                    break;
                default:
                    parseErrorExpected(this->curTok.getLine(), "algoritmo", this->curTok.getText());
                    this->errors++;
                    break;
            }

            return res;
        }

        void parse(vector<Token> tokens)
        {
            this->tokens = tokens;
            this->ast = Ast(this->algorithm_block());
        }

        void showAst(AstNode* a, int level)
        {

            if(a)
            {
                switch(a->getType())
                {
                case ast_algorithm_block:
                    cout << setw(level) << " " << "Algoritmo" << endl;
                    for(auto i: a->getSentences())
                    {
                        showAst(i, level+2);
                    }
                    break;
                case ast_vardeclblock:
                    cout << setw(level) << " " << "Declaracion de variables" << endl;
                    for(auto i: a->getSentences())
                    {
                        if(i)
                        {
                            showAst(i, level+2);
                        }
                    }
                    break;
                case ast_vardecl:
                    cout << setw(level) << " " << "Declaracion" << " <-> "<< a->getToken().getText()<< endl;
                    for(auto i: a->getSentences())
                    {
                        if(i)
                        {
                            showAst(i, level+2);
                        }
                    }
                    break;
                case ast_sentences:
                    cout << setw(level) << " " << "Codigo" << endl;
                    for(auto i: a->getSentences())
                    {
                        if(i)
                        {
                            showAst(i, level+2);
                        }

                    }
                    break;
                case ast_varassign:
                    cout << setw(level) << " " << "Asignacion de variable" << " <-> "<< a->getToken().getText()<< endl;
                    showAst(a->getLeft(), level+2);
                    break;
                case ast_forloop:
                    cout << setw(level) << " " << "Ciclo para" << endl;
                    for(auto i: a->getSentences())
                    {
                        if(i)
                        {
                            showAst(i, level+2);
                        }

                    }
                    break;
                case ast_callwrite:
                    cout << setw(level) << " " << "Escribir en pantalla" << endl;
                    for(auto i: a->getSentences())
                    {
                        if(i)
                        {
                            showAst(i, level+2);
                        }

                    }
                    break;
                case ast_binop:
                    cout << setw(level) << " " << "Operacion binaria" << " <-> "<< a->getToken().getText()<< endl;

                    showAst(a->getLeft(), level+2);
                    showAst(a->getRight(), level+2);
                    break;
                case ast_unop:
                    cout << setw(level) << " " << "Operacion unaria" << " <-> "<< a->getToken().getText()<< endl;

                    showAst(a->getLeft(), level+2);
                    break;
                case ast_number:
                    cout << setw(level) << " " << "numero entero" << " <-> "<< a->getToken().getText()<<endl;
                    break;
                case ast_flnumber:
                    cout << setw(level) << " " << "numero real" << " <-> "<< a->getToken().getText()<< endl;
                    break;
                case ast_bool:
                    cout << setw(level) << " " << "valor logico" << " <-> "<< a->getToken().getText()<< endl;
                    break;
                case ast_var:
                    cout << setw(level) << " " << "variable" << " <-> "<< a->getToken().getText()<< endl;
                }
            }

        }





};


#endif // PARSER_H_INCLUDED
