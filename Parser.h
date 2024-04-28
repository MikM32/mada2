#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED


#include "lexer.h"
#include "ast.h"

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


                default:
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
                    res->setToken(this->curTok);
                    eat();
                    res = new AstNode();
                    res->setLeft( this->primary() );
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

            eat();
            res = this->unary();
            res->setToken(this->curTok);

            eat();
            switch(this->curTok.getType())
            {
                case ASTER:
                    res_binop = new AstNode();
                    res_binop->setToken(this->curTok);
                    eat();

                    res_binop->setLeft(res);
                    res_binop->setRight( this->unary() );
                    res_binop->setType(ast_binop);
                    res_binop->setOpType(ASTER);


                    return res_binop;
                    break;
                default:
                    break;

            }

            return res;
        }

        // -------------- Sentencias ------------------

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
                            res->setType(ast_varassign);
                            res->setToken(this->curTok);

                            eat();

                            res->setLeft(this->factor());

                            if(this->nextTok.getType() != ENDL)
                            {
                                parseErrorExpected(this->curTok.getLine(), "salto de linea", this->nextTok.getText());
                                this->errors++;
                                return nullptr;
                            }

                            eat();
                            //eat();

                            break;
                        default:
                            //error
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


                    while(this->curTok.getType() != COMMA && this->curTok.getType() != CLOS_PAREN)
                    {

                        res->addArg(this->factor());
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
                res->addArg(this->sentence());

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





};


#endif // PARSER_H_INCLUDED
