#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "Token.h"
#include "errors.h"

void showTokenInfo(Token tok)
{
    int t = tok.getType();
    switch(t)
    {
        case NONE:
            cout << "NONE : " << tok.getText() <<endl;
            break;
        case ID:
            cout << "IDENTIFICADOR : " << tok.getText() <<endl;
            break;
        case ENDL:
            cout << "EOL : \\n"<<endl;  // EOL significa End Of Line o Fin De Linea
            break;
        case NUM:
            cout << "NUM_LITERAL : " << tok.getText() <<endl;
            break;
        case FLOAT:
            cout << "FLOAT_LITERAL : " << tok.getText() <<endl;
            break;
        case PLUS:
            cout << "PLUS : " << tok.getText() <<endl;
            break;
        case ALGORITMO:
            cout << "ALGORITMO : "<< tok.getText() <<endl;
            break;
        default:
            debugErrorUnkType(t);
            break;
    }
}


#endif // DEBUG_H_INCLUDED
