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
#define MUL 9
#define EQUAL 10
#define REFUN 11
#define IFEXP 12
#define OROP 13
#define NEWBOX 14
#define SETBOX 15
#define OPENBOX 16
#define SEQN 17
#define SETVAR 18
#define REFCLOSV 19
#define BOXV 20
#define EXPRV 21

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
    AST *test_expr;
    AST *then_expr;
    AST *else_expr;
    AST *boxname;
    AST *value;

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
    void createTestexpr(AST *);
    void createThenexpr(AST *);
    void createElseexpr(AST *);
    void createBoxname(AST *);
    void createValue(AST *);

    string getASTCode();
};

class DS;

class VAL
{
public:
    int type;
    string num = "0";
    string param ="";
    string address;    //for boxV
    AST *fbody;
    AST *expr;  //for exprV
    DS *dsClosure;
    VAL *value = NULL; //for exprV

    string getVal();
};

class DS
{
public:
    int is_empty = 1;
    string name = "";
    string address = "";
    DS* subDs;

    string showDs();
};

class ST
{
public:
    int is_empty = 1;
    string address = "";
    VAL *value;
    ST *subSt;

    string showSt();
};

class VS
{
public:
    VAL *value;
    ST *store;
    void setValue(VAL *);
    void setStore(ST *);
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

void AST::createTestexpr(AST *asttest)
{
    test_expr = asttest;
}
void AST::createThenexpr(AST *astthen)
{
    then_expr = astthen;
}
void AST::createElseexpr(AST *astelse)
{
    else_expr = astelse;
}
void AST::createBoxname(AST *astboxname)
{
    boxname = astboxname;
}
void AST::createValue(AST *astvalue)
{
    value = astvalue;
}

string AST::getASTCode()
{
    string astCode = "";
    string numCode = "(num ";
    string addCode = "(add ";
    string subCode = "(sub ";
    string mulCode = "(mul ";
    string eqCode = "(eqop ";
    string idCode = "(id ";
    string funCode = "(fun ";
    string refunCode = "(refun ";
    string ifCode = "(ifexp ";
    string orCode = "(orop ";
    string newboxCode = "(newbox ";
    string setboxCode = "(setbox ";
    string openboxCode = "(openbox ";
    string seqnCode = "(seqn ";
    string setvarCode = "(setvar ";
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
    else if (type == MUL)
    {
        astCode += mulCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == EQUAL)
    {
        astCode += eqCode;
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
    else if (type == REFUN)
    {
        astCode += refunCode;
        astCode += param;
        astCode += " ";
        astCode += fbody->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == IFEXP)
    {
        astCode += ifCode;
        astCode += test_expr->getASTCode();
        astCode += " ";
        astCode += then_expr->getASTCode();
        astCode += " ";
        astCode += else_expr->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == OROP)
    {
        astCode += orCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == NEWBOX)
    {
        astCode += newboxCode;
        astCode += value->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == SETBOX)
    {
        astCode += setboxCode;
        astCode += boxname->getASTCode();
        astCode += " ";
        astCode += value->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == OPENBOX)
    {
        astCode += openboxCode;
        astCode += value->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == SEQN)
    {
        astCode += seqnCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }
    else if (type == SETVAR)
    {
        astCode += setvarCode;
        astCode += id;
        astCode += " ";
        astCode += value->getASTCode();
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
    string refclosVal = "(refclosV ";
    string boxVal = "(boxV ";
    string exprVal = "(exprV ";
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
        result += " ";
        result += dsClosure->showDs();
        result += ')';
        return result;
    }
    else if(type == REFCLOSV){
        result += refclosVal;
        result += param;
        result += " ";
        result += fbody->getASTCode();
        result += " ";
        result += dsClosure->showDs();
        result += ')';
        return result;
    }
    else if(type == BOXV){
        result += boxVal;
        result += address;
        result += ')';
        return result;
    }
    else if(type == EXPRV){
        result += boxVal;
        result += expr->getASTCode();
        result += " ";
        result += dsClosure->showDs();
        if(value == NULL){
            result += " (box #f)";
        }
        else{
            result += " (box ";
            result += value->getVal();
        }
        result += ')';
        return result;
    }
}

string DS::showDs()
{
    string ds = "";
    if(is_empty == 1){
        ds += "(mtSub)";
    }
    else{
        ds += "(aSub ";
        ds += name;
        ds += " ";
        ds += address;
        ds += " ";
        ds += subDs->showDs();
        ds += ")";
    }
    return ds;
}

string ST::showSt()
{
    string st = "";
    if(is_empty == 1){
        st += "(mtSto)";
    }
    else{
        st += "(aSto ";
        st += address;
        st += " ";
        st += value->getVal();
        st += " ";
        st += subSt->showSt();
        st += ")";
    }
    return st;
}

void VS::setValue(VAL *val){
    value = val;
}
void VS::setStore(ST *st){
    store = st;
}
#endif