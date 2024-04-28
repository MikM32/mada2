#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include <iostream>
#include "token_types.h"

using namespace std;

#define DEBUG_ERROR_UNKNOWN_TYPE_MSG "Debug Error: Tipo de token desconocido "


#define LEX_ERROR_INVALID_FLOAT " escritura invalida de numeros de punto flotante"
#define LEX_ERROR_INVALID_IDENT " identificador invalido "
#define LEX_ERROR_INVALID_CHAR " Caracter invalido "
#define PARSE_ERROR_EXPECTED_OPEN_PAREN " Se esperaba un parentesis abierto en"

enum ErrType
{
    dbgErrUnkwT = 1,

    lexErrFloat,
};

// Errores de Debugging

void debugErrorUnkType(int t)
{
    cout << DEBUG_ERROR_UNKNOWN_TYPE_MSG << t << endl;
    exit(dbgErrUnkwT);
}

// Errores del lexer

void lexErrorInvalidFloat(int line, string id)
{
    cout << "Error lexico en la linea: " << line << ":" << LEX_ERROR_INVALID_FLOAT << " \"" << id << "\""<< endl;
    //exit(lexErrFloat);
}

void lexErrorInvalidId(int line, string id)
{
    cout << "Error lexico en la linea: " << line << ":" << LEX_ERROR_INVALID_IDENT << " \"" << id << "\"" << endl;
}

void lexErrorInvalidChar(int line, char chr)
{
    cout << "Error lexico en la linea: " << line << ":" << LEX_ERROR_INVALID_CHAR << " \"" << chr << "\" " << endl;
}

// Errores del parser

void parseErrorExpected(int line, string expected ,string id)
{
    cout << "Error sintactico en la linea: " << line << ":" << " Se esperaba \""<< expected << "\" en lugar de " << " \"" << id << "\" "<<endl;
}

// Errores semanticos

void semanticErrorDifTypes(int line, string id1, string id2)
{
    cout << "Error semantico en la linea: " << line << ":" << " Los tipos de las expresiones " << id1 << " y " << id2 << " no coinciden" <<endl;
}

void semanticErrorInvalidType(int line, string id, int op)
{
    switch(op)
    {
        case PLUS:
        case MINUS:
        case RBAR:
        case ASTER:
            cout << "Error semantico en la linea: " << line << ":" << " Las operaciones aritmeticas solo operan con numeros, no con valores logicos como \"" << id << "\" "<<endl;
            break;
    }
}

void semanticErrorVarNotDec(int line, string id)
{
    cout << "Error semantico en la linea: " << line << ":" << " La variable \""<< id <<"\" no ha sido declarada" <<endl;
}

void semanticErrorVarWrongScope(int line, string id)
{
    cout << "Error semantico en la linea: " << line << ":" << " La variable \""<< id <<"\" no ha sido declarada en este contexto" <<endl;
}

// Otros Errores

#endif // ERRORS_H_INCLUDED
