#ifndef TOKEN_TYPES_H_INCLUDED
#define TOKEN_TYPES_H_INCLUDED


enum TokenType
{
    NONE = 1,
    ID,
    ENDL,
    NUM,
    FLOAT,
    STRING,
    PLUS,   //  Mas
    MINUS,  //  Menos
    ASTER,  //  Multiplicacion
    RBAR,   //  Division
    DIV,    //  Division entera
    MOD,    //  Modulo
    ALGORITMO,
    VERDADERO,
    FALSO,
    VAR,
    INICIO,
    FIN,
    DDOT,
    ENTERO,
    REAL,
    LOGICO,
    CARACTER,
    CADENA,
    ASIGN,
    LESS,
    BIGGER,
    LEQUAL,
    BEQUAL,
    EQUAL,
    DIFF,
    OPEN_PAREN,
    CLOS_PAREN,
    COMMA,
    WRITE,
    READ,
    PARA,
    HASTA,
    HACER,
    FPARA

};

#endif // TOKEN_TYPES_H_INCLUDED
