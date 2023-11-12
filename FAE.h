#pragma once

#ifndef AE_H
#define AE_H

#include <iostream>
#include <string>

using namespace std;
#define NUM 1
#define ADD 2
#define SUB 3
#define ID 4
#define FUN 5
#define APP 6
#define NUMV 7
#define CLOSUREV 8

class AST
{

public:
    int type;
    string num = "0";
    string id = "";
    string param = "";
    AST *lhs;
    AST *rhs;
    AST *fbody;
    AST *fun_expr;
    AST *arg_expr;

    void createNum(string);
    void createId(string);
    void createAdd(AST *, AST *);
    AST* getLhs();
    AST* getRhs();
    int getType();
    void createParam(string);
    void createFunction(AST *);
    void createFunexpr(AST *);
    void createArgexpr(AST *);

    string getASTCode();
};

class DS;

class VAL
{
public:
    int type;
    string num = "0";
    string param ="";
    AST *fbody;
    DS *dsClosure;

    string getVal();
};

class DS
{
public:
    int is_empty = 1;
    string name = "";
    VAL* value;
    DS* subDs;
};

void AST::createNum(string stringNum)
{
    num = stringNum;
}

void AST::createId(string stringId)
{
    id = stringId;
}

void AST::createAdd(AST *astlhs, AST *astrhs)
{
    lhs = astlhs;
    rhs = astrhs;
}

AST* AST::getLhs()
{
    return lhs;
}

AST* AST::getRhs()
{
    return rhs;
}

int AST::getType(){
    return type;
}

void AST::createParam(string stringParam)
{   
    param = stringParam;
}

void AST::createFunction(AST *astfbody)
{
    fbody = astfbody;
}

void AST::createFunexpr(AST *astfun)
{
    fun_expr = astfun;
}

void AST::createArgexpr(AST *astarg)
{
    arg_expr = astarg;
}

string AST::getASTCode()
{
    string astCode = "";
    string numCode = "(num ";
    string addCode = "(add ";
    string subCode = "(sub ";
    string idCode = "(id ";
    string funCode = "(fun ";
    string appCode = "(app ";

    if (type == NUM)
    {
        astCode += numCode;
        astCode += num;
        astCode += ")";
        return astCode;
    }
    else if (type == ADD)
    {
        astCode += addCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == SUB)
    {
        astCode += subCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == ID)
    {
        astCode += idCode;
        astCode += id;
        astCode += ")";
        return astCode;
    }
    else if (type == FUN)
    {
        astCode += funCode;
        astCode += param;
        astCode += " ";
        astCode += fbody->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == APP)
    {
        astCode += appCode;
        astCode += fun_expr->getASTCode();
        astCode += " ";
        astCode += arg_expr->getASTCode();
        astCode += ")";
        return astCode;
    }

    return astCode;
}

string VAL::getVal(){
    string result = "";
    string numVal = "(numV ";
    string closureVal = "(closureV ";
    if(type == NUMV){
        result += numVal;
        result += num;
        result += ')';
        return result;
    }
    else if(type == CLOSUREV){
        result += closureVal;
        result += param;
        result += " ";
        result += fbody->getASTCode();
        result += ')';
        return result;
    }
}

#endif