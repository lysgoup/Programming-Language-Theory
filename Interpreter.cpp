#include <iostream>
#include <regex>
#include <string>
#include "FAE.h"

class Interpreter
{
public:
  vector<string> splitExpressionAsSubExpressions(string absCode);
  vector<string> getSubExpressions(string absCode);
  VAL *addVnum(VAL *lhs, VAL *rhs);
  VAL *subVnum(VAL *lhs, VAL *rhs);
  VAL *lookup(string name, DS *ds);
  VAL *interprete(AST *absCode, DS *defSub);
};

vector<string> Interpreter::splitExpressionAsSubExpressions(string absCode)
{
    if ((absCode.front() == '(' && absCode.back() != ')') || (absCode.front() != '(' && absCode.back() == ')'))
        exit(0);

    if (absCode.front() == '(')
        absCode = absCode.substr(1, absCode.length() - 1);

    return getSubExpressions(absCode);
}

vector<string> Interpreter::getSubExpressions(string exampleCode)
{
    vector<string> sexpressions = vector<string>();
    int openingParenthesisCount = 0;
    string strBuffer = "";
    for (int i = 0; i < exampleCode.length(); i++)
    {
        if (exampleCode.at(i) == ' ' && openingParenthesisCount == 0)
        {
            // buffer is ready to be a subexpression
            if (strBuffer.size() > 0)
            {
                sexpressions.push_back(strBuffer);
                strBuffer = ""; // Ready to start a new buffer
            }
            continue;
        }
        else
        {
            if (exampleCode.at(i) == '(' && openingParenthesisCount == 0)
            {
                openingParenthesisCount++;
                // Ready to start a new buffer
                strBuffer = "";
                strBuffer += exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == '(')
            {
                openingParenthesisCount++;
                strBuffer = strBuffer + exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == ')' && openingParenthesisCount > 0)
            {
                openingParenthesisCount--;
                strBuffer = strBuffer + exampleCode.at(i);
                if(openingParenthesisCount == 0){
                    sexpressions.push_back(strBuffer);
                    strBuffer = "";
                }
                continue;
            }
            else if (exampleCode.at(i) == ')')
            {
                // buffer is ready to be a subexpression
                //sexpressions.push_back(strBuffer);
                continue;
            }
        }
        strBuffer = strBuffer + exampleCode.at(i);
    }
    //sexpressions.push_back(strBuffer);
    return sexpressions;
}

VAL * Interpreter::addVnum(VAL *lhs, VAL *rhs)
{
  VAL *add = new VAL();
  if(lhs->type == NUMV && rhs->type == NUMV){
    add->type = NUMV;
    add->num = to_string(stoi(lhs->num) + stoi(rhs->num));
  }
  return add;
}

VAL * Interpreter::subVnum(VAL *lhs, VAL *rhs)
{
  VAL *sub = new VAL();
  if(lhs->type == NUMV && rhs->type == NUMV){
    sub->type = NUMV;
    sub->num = to_string(stoi(lhs->num) - stoi(rhs->num));
  }
  return sub;
}

VAL * Interpreter::lookup(string name, DS *ds)
{
  if(ds->is_empty == 1){
    fprintf(stderr, "no binding for identifier %s", &name);
		exit(EXIT_FAILURE);
  }
  else{
    if(name.compare(ds->name)==0){
      return ds->value;
    }
    else{
      return lookup(name, ds->subDs);
    }
  }
}

VAL * Interpreter::interprete(AST *absCode, DS *defSub){
  VAL *val = new VAL();
  if(absCode->getType() == NUM){
    val->type = NUMV;
    val->num = absCode->num;
    return val;
  }
  else if(absCode->getType() == ADD){
    val = addVnum(interprete(absCode->getLhs(), defSub), interprete(absCode->getRhs(), defSub));
    return val;
  }
  else if(absCode->getType() == SUB){
    val = subVnum(interprete(absCode->getLhs(), defSub), interprete(absCode->getRhs(), defSub));
    return val;
  }
  else if(absCode->getType() == ID){
    val = lookup(absCode->id, defSub);
    return val;
  }
  else if(absCode->getType() == FUN){
    val->type = CLOSUREV;
    val->param = absCode->param;
    val->fbody = absCode->fbody;
    val->dsClosure = defSub;
    return val;
  }
  else if(absCode->getType() == APP){
    VAL *funVal;
    VAL *argVal;
    funVal = interprete(absCode->fun_expr, defSub);
    argVal = interprete(absCode->arg_expr, defSub);
    DS *ds = new DS();
    ds->is_empty = 0;
    ds->subDs = defSub;
    if(funVal->param.front() == '{'){
      ds->name = funVal->param.substr(1, funVal->param.length() - 2);
    }
    else{
      ds->name = funVal->param;
    }
    ds->value = argVal;
    val = interprete(funVal->fbody, ds);
    return val;
  }
}