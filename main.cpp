#include <iostream>
#include <list>
#include <fstream>

#include "lexer.h"
#include "Parser.h"
#include "semantic.h"
#include "vm_compiler.h"

using namespace std;

int main()
{
    Lexer lex;
    vector<Token> tokens;

    ifstream t("test.mc");
    t.seekg(0, ios::end);
    size_t size = t.tellg();
    string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    lex.scan(buffer);
    tokens = lex.getTokens();

    /*
    for(int i=0; i<tokens.size(); i++)
    {
        tokens[i].showTokenInfo();
    }*/




    Parser par;
    Semantic sem;
    MadaVmCompiler comp;
    Vm mvm;

    //lex.showTokens();
    par.parse(tokens);

    Ast a = par.getAst();

    par.showAst(a.getRoot(), 1);


    if(!par.getErrors())
    {
        sem.check(a.getRoot());
    }

    cout << "Codigo lexeado con " << lex.getErrors() << " errores "<<endl;
    cout << "Codigo parseado con "<< par.getErrors() << " errores "<<endl;
    cout << "Codigo semantizado con "<< sem.getErrors() << " errores" <<endl;

    if(!sem.getErrors())
    {
        comp= MadaVmCompiler(a, sem.getVars());
        comp.compile(a.getRoot());
        comp.showBytecodes();
        mvm = Vm(comp.getByteCodes());

        cout << endl << "Ejecutando..." << endl;
        mvm.eval(false);
        cout << endl << "Ejecucion finalizada."<<endl;

    }


    return 0;
}
