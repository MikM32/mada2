#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <iostream>
#include "token_types.h"
#include "errors.h"

using namespace std;



class Token{

    private:
        TokenType type = NONE;
        int line;
        string text;

    public:


        Token();
        Token(TokenType type, string text, int line);

        void setType(TokenType type)
        {
            this->type = type;
        };

        void setText(string text)
        {
            this->text = text;
        };

        TokenType getType()
        {
            return this->type;
        };

        string getText()
        {
            return this->text;
        };

        int getLine()
        {
            return this->line;
        }

        void showTokenInfo()
        {
            switch(this->type)
            {
                case NONE:
                    cout << "NONE : " << this->text <<endl;
                    break;
                case ID:
                    cout << "IDENTIFICADOR : " << this->text <<endl;
                    break;
                case ENDL:
                    cout << "EOL : \\n"<<endl;  // EOL significa End Of Line o Fin De Linea
                    break;
                case NUM:
                    cout << "NUM_LITERAL : " << this->text <<endl;
                    break;
                case FLOAT:
                    cout << "FLOAT_LITERAL : " << this->text <<endl;
                    break;
                case PLUS:
                    cout << "PLUS : " << this->text <<endl;
                    break;
                case ALGORITMO:
                    cout << "ALGORITMO : " << this->text <<endl;
                    break;
                case DDOT:
                    cout << "DDOT : :"<<endl;
                    break;
                case CADENA:
                    cout << "CADENA : "<< this->text << endl;
                    break;
                case ASIGN:
                    cout << "ASIGNACION : <-" << endl;
                    break;
                case WRITE:
                    cout << "ESCRIBIR" << endl;
                    break;
                case OPEN_PAREN:
                    cout << "OPEN_PARENT : ( " <<endl;
                    break;
                case CLOS_PAREN:
                    cout << "CLOSED_PARENT : )" << endl;
                    break;
                case COMMA:
                    cout << "COMMA : ," << endl;
                    break;
                case ASTER:
                    cout << "ASTERISC : *" << endl;
                    break;
                case VERDADERO:
                    cout << "TRUE : verdadero" <<endl;
                    break;
                case FALSO:
                    cout << "FALSE : falso" << endl;
                    break;
                default:
                    debugErrorUnkType(this->type);
                    break;
            }
        }


};

Token::Token()
{

}

Token::Token(TokenType type, string text, int line)
{
    this->type = type;
    this->text = text;
    this->line = line;
}

#endif // TOKEN_H_INCLUDED
