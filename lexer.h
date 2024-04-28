#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include "Token.h"
#include "reservadas.h"
#include<vector>

#define COMMENT_BLOCK_OPEN '{'
#define COMMENT_BLOCK_CLOSED '}'

bool isAlfa(char chr)
{
    if((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
    {
        return true;
    }

    return false;
}

bool isNum(char chr)
{
    if(chr >= '0' && chr <= '9')
    {
        return true;
    }

    return false;
}

int reservedWord(string word) // Funcion que devuelve el tipo de palabra reservada si el parametro es igual a la palabra mencionada y si no es entonces devuelve 0
{
    if(word == R_ALG)
    {
        return ALGORITMO;
    }
    else if(word == R_VAR)
    {
        return VAR;
    }
    else if(word == R_INI)
    {
        return INICIO;
    }
    else if(word == R_END)
    {
        return FIN;
    }
    else if(word == R_INT)
    {
        return ENTERO;
    }
    else if(word == R_DIV)
    {
        return DIV;
    }
    else if(word == R_MOD)
    {
        return MOD;
    }
    else if(word == R_TRUE)
    {
        return VERDADERO;
    }
    else if(word == R_FALSE)
    {
        return FALSO;
    }
    else if(word == R_WRITE)
    {
        return WRITE;
    }
    else if(word == R_BOOL)
    {
        return LOGICO;
    }
    else if(word == R_FLOAT)
    {
        return REAL;
    }
    else if(word == R_CHAR)
    {
        return CARACTER;
    }
    else
    {
        return 0;
    }
}

class Lexer{

    private:
        bool isPrevEndLine = false;
        bool isCurEndLine = false;
        bool isFirstChar = true;
        bool isAtEnd = false;

        int errors = 0;
        int curLine = 1;
        int curCharIndex = 0;

        vector<Token> tokens;

    public:

        Lexer();
        void scan(string raw_code);


        void advanceChar()
        {
            this->curCharIndex++;
            if(this->isFirstChar)
            {
                this->isFirstChar = false;
            }
        }

        vector<Token> getTokens()
        {
            return this->tokens;
        }

        int getErrors()
        {
            return this->errors;
        }

};

Lexer::Lexer()
{

}

void Lexer::scan(string raw_code)
{
    string buffer;
    int tt;

    while(raw_code[this->curCharIndex] != '\0') //bucle del escaneo, si llega al final de la cadena se detiene
    {
        if(raw_code[this->curCharIndex] == COMMENT_BLOCK_OPEN)  // Si es un comentario omite todo lo que este alli
        {
            this->advanceChar();
            while(raw_code[this->curCharIndex] != COMMENT_BLOCK_CLOSED)
            {
                if(raw_code[this->curCharIndex] == '\n')
                {
                    this->curLine++;
                }
                this->advanceChar();
            }
            this->advanceChar();
        }
        else if( isAlfa( raw_code[this->curCharIndex]) || raw_code[this->curCharIndex] == '_') // si es un identificador
        {
            buffer.push_back(raw_code[this->curCharIndex]);
            this->advanceChar();

            while(isAlfa( raw_code[this->curCharIndex]) || isNum(raw_code[this->curCharIndex]) || raw_code[this->curCharIndex] == '_')
            {
                buffer.push_back(raw_code[this->curCharIndex]);
                this->advanceChar();
            }

            tt = reservedWord(buffer);
            if(tt)
            {
                this->tokens.push_back(Token((TokenType)tt, buffer, this->curLine));
            }
            else
            {
                this->tokens.push_back(Token(ID, buffer, this->curLine));
            }
            //cout << buffer << endl;
            buffer = "";
            this->isPrevEndLine = false;


        }
        else if(isNum(raw_code[this->curCharIndex]))    // Si es un numero
        {
            bool /*isFloat = false, */ isFerror = false;

            buffer.push_back(raw_code[this->curCharIndex]);
            this->advanceChar();

            while(isNum(raw_code[this->curCharIndex]))
            {
                buffer.push_back(raw_code[this->curCharIndex]);
                this->advanceChar();
            }

            if(raw_code[this->curCharIndex] == '.') // Si hay un punto despues del numero entonces esperara un numero adelante del punto
            {
                //isFloat = true;
                buffer.push_back(raw_code[this->curCharIndex]);
                this->advanceChar();
                while(raw_code[this->curCharIndex] != ' ' && raw_code[this->curCharIndex] != '\n')
                {
                    if(!isNum(raw_code[this->curCharIndex]))
                    {
                        isFerror = true;

                    }
                    buffer.push_back(raw_code[this->curCharIndex]);
                    this->advanceChar();

                }

                if(!isFerror)
                {
                    this->tokens.push_back(Token(FLOAT, buffer, this->curLine));
                    this->isPrevEndLine = false;
                }
                else
                {
                    this->errors++;
                    lexErrorInvalidFloat(this->curLine, buffer);
                }


            }else if(isAlfa(raw_code[this->curCharIndex]))  //Si despues del numero hay una letra entonces el lexer supone que es un identificador invalido
            {
                buffer.push_back(raw_code[this->curCharIndex]);
                this->advanceChar();

                while(raw_code[this->curCharIndex] != ' ' && raw_code[this->curCharIndex] != '\n')
                {
                    buffer.push_back(raw_code[this->curCharIndex]);
                    this->advanceChar();
                }

                lexErrorInvalidId(this->curLine, buffer);
                this->errors++;
                //this->advanceChar();

            }
            else
            {
                this->tokens.push_back(Token(NUM, buffer, this->curLine));
                this->isPrevEndLine = false;
            }


            //cout << buffer << endl;
            buffer="";



        }
        else if(raw_code[this->curCharIndex] == '+')
        {
            this->tokens.push_back(Token(PLUS, "+", this->curLine));
            this->advanceChar();
            this->isPrevEndLine = false;
        }
        else if(raw_code[this->curCharIndex] == '-')
        {
            this->tokens.push_back(Token(MINUS, "-", this->curLine));
            this->advanceChar();
            this->isPrevEndLine = false;
        }
        else if(raw_code[this->curCharIndex] == '*')
        {
            this->tokens.push_back(Token(ASTER, "*", this->curLine));
            this->advanceChar();
            this->isPrevEndLine = false;
        }
        else if(raw_code[this->curCharIndex] == '/')
        {
            this->tokens.push_back(Token(RBAR, "/", this->curLine));
            this->advanceChar();
            this->isPrevEndLine = false;
        }
        else if(raw_code[this->curCharIndex] == '\n')
        {
            this->curLine++;
            if(!this->isPrevEndLine && !this->isFirstChar)
            {
                this->tokens.push_back(Token(ENDL, "\\n", this->curLine));

            }
            this->isPrevEndLine = true;
            this->advanceChar();

        }
        else if(raw_code[this->curCharIndex] == ':')
        {
            this->tokens.push_back(Token(DDOT, ":", this->curLine));
            this->advanceChar();
        }
        else if(raw_code[this->curCharIndex] == '"')
        {
            this->advanceChar();
            while(raw_code[this->curCharIndex] != '"')
            {
                if(raw_code[this->curCharIndex] == '\\')
                {
                    this->advanceChar();
                    switch(raw_code[this->curCharIndex])
                    {
                    case 'n':
                        buffer.push_back('\n');
                        break;
                    case 't':
                        buffer.push_back('\t');
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    buffer.push_back(raw_code[this->curCharIndex]);
                }

                this->advanceChar();
            }

            this->tokens.push_back(Token(CADENA, buffer, this->curLine));
            buffer = "";
            this->advanceChar();
        }
        else if(raw_code[this->curCharIndex] == '<')
        {
            this->advanceChar();
            if(raw_code[this->curCharIndex] == '-')
            {
                this->tokens.push_back(Token(ASIGN, "<-", this->curLine));
                this->advanceChar();
            }
            else
            {
                this->tokens.push_back(Token(LESS, "<", this->curLine));
            }
        }
        else if (raw_code[this->curCharIndex] == ' ' || raw_code[this->curCharIndex] == '\t')
        {
            this->advanceChar();
        }
        else if( raw_code[this->curCharIndex] == '(')
        {
            this->tokens.push_back(Token(OPEN_PAREN, "(", this->curLine));
            this->advanceChar();
        }
        else if( raw_code[this->curCharIndex] == ')')
        {
            this->tokens.push_back(Token(CLOS_PAREN, ")", this->curLine));
            this->advanceChar();
        }
        else if( raw_code[this->curCharIndex] == ',')
        {
            this->tokens.push_back(Token(COMMA, ",", this->curLine));
            this->advanceChar();
        }
        else
        {
            lexErrorInvalidChar(this->curLine, raw_code[this->curCharIndex]);
            this->errors++;
            this->advanceChar();
        }
    }

}


#endif // LEXER_H_INCLUDED
