#include <iostream>
#include "FAE.h"
#include "Parser.cpp"
#include "Interpreter.cpp"
using namespace std;

int main(int argc, char *argv[]){
    string pOption, ConcreteCode;
    AST* ast;
    VAL* val;
    DS* ds = new DS();
    Parser parser;
    Interpreter interp;

    if(argc >= 3){
        if(strcmp(argv[1], "-p") == 0){
            pOption = argv[1];
            ConcreteCode = argv[2];
        }
        else    
            return 0;
    }
    else if(argc == 2){
        pOption = "Interprete";
        ConcreteCode = argv[1];
    }

    if(pOption == "-p"){
        ast = parser.parse(ConcreteCode);
        cout << ast->getASTCode();
        
        return 0;   
    }
    else if(pOption == "Interprete"){
        ast = parser.parse(ConcreteCode);
        val = interp.interprete(ast, ds);
        cout << val->getVal();
        return 0;
    }
    else
        cout << "Syntax Error." ; 

    return 0;
}