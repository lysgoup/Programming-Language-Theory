#include <iostream>
#include "RBMRLFAE.h"
#include "Parser.cpp"
#include "Interpreter.cpp"
using namespace std;

int main(int argc, char *argv[]){
    string pOption, ConcreteCode, dOption;
    AST* ast;
    VS* vs;
    DS* ds = new DS();
    ST* st = new ST();
    Parser parser;
    Interpreter interp;

    if(argc >= 4){
        if(strcmp(argv[1], "-p") == 0){
            pOption = argv[1];
            ConcreteCode = argv[3];
        }
        else if(strcmp(argv[1], "-d") == 0){
            dOption = argv[1];
            ConcreteCode = argv[3];
        }
        else    
            return 0;
        
        if(strcmp(argv[2], "-p") == 0){
            pOption = argv[2];
            ConcreteCode = argv[3];
        }
        else if(strcmp(argv[2], "-d") == 0){
            dOption = argv[2];
            ConcreteCode = argv[3];
        }
        else    
            return 0;
    }
    else if(argc >= 3){
        if(strcmp(argv[1], "-p") == 0){
            pOption = argv[1];
            ConcreteCode = argv[2];
        }
        else if(strcmp(argv[1], "-d") == 0){
            dOption = argv[1];
            pOption = "Interprete";
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
        if(dOption == "-d"){
            ConcreteCode = parser.desugar(ConcreteCode);
        }
        ast = parser.parse(ConcreteCode);
        cout << ast->getASTCode();
        
        return 0;   
    }
    else if(pOption == "Interprete"){
        if(parser.isRecursion(ConcreteCode)){
            cout << "Recursion!!\n";
            ConcreteCode = parser.desugar(ConcreteCode);
        }
        else{
            cout << "not Recursion!!\n";
        }
        ast = parser.parse(ConcreteCode);
        vs = interp.interprete(ast, ds, st);
        cout << vs->getVs();
        return 0;
    }
    else
        cout << "Syntax Error." ; 

    return 0;
}