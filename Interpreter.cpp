#include <iostream>
#include <regex>
#include <string>
#include "RBMRLFAE.h"

class Interpreter
{
public:
  vector<string> splitExpressionAsSubExpressions(string absCode);
  vector<string> getSubExpressions(string absCode);
  VAL *addVnum(VAL *lhs, VAL *rhs);
  VAL *subVnum(VAL *lhs, VAL *rhs);
  VAL *mulVnum(VAL *lhs, VAL *rhs);
  string lookup(string name, DS *ds);
  VS *storelookup(string address, ST *st, ST *originSt);
  VS *strict(VAL *value, ST *st);
  string mall(ST *st);
  int max_address(ST *st);
  VS *interprete(AST *absCode, DS *defSub, ST *store);
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

VAL * Interpreter::mulVnum(VAL *lhs, VAL *rhs)
{
  VAL *mul = new VAL();
  if(lhs->type == NUMV && rhs->type == NUMV){
    mul->type = NUMV;
    mul->num = to_string(stoi(lhs->num) * stoi(rhs->num));
  }
  return mul;
}

string Interpreter::lookup(string name, DS *ds)
{
  if(ds->is_empty == 1){
    fprintf(stderr, "no binding for identifier %s", &name);
		exit(EXIT_FAILURE);
  }
  else{
    if(name.compare(ds->name)==0){
      return ds->address;
    }
    else{
      return lookup(name, ds->subDs);
    }
  }
}

VS * Interpreter::storelookup(string address, ST *st, ST *originSt)
{
  if(st->is_empty == 1){
    fprintf(stderr, "no value at address");
		exit(EXIT_FAILURE);
  }
  else{
    if(address.compare(st->address) == 0){
      return strict(st->value, originSt);
    }
    else{
      return storelookup(address,st->subSt,originSt);
    }
  }
}

VS * Interpreter::strict(VAL *value, ST *st)
{
  VS* vs = new VS();
  if(value->type == EXPRV){
    if(value->value == NULL){
      VS *temp1;
      VS *temp2 = new VS();
      temp1 = interprete(value->expr, value->dsClosure, st);
      temp2 = strict(temp1->value,temp1->store);
      // delete temp1;
      value->value = temp2->value;
      // vs->setValue(temp2->value);
      // vs->setStore(temp2->store);
      vs = temp2;
    }
    else{
      vs->setValue(value->value);
      vs->setStore(st);
    }
  }
  else{
    vs->setValue(value);
    vs->setStore(st);
  }
  return vs;
}

string Interpreter::mall(ST *st)
{
  string new_address = to_string(max_address(st) + 1);
  return new_address;
}

int Interpreter::max_address(ST *st)
{
  if(st->is_empty == 1){
    return 0;
  }
  else{
    return max(stoi(st->address),max_address(st->subSt));
  }
}

VS * Interpreter::interprete(AST *absCode, DS *defSub, ST *store){
  VS *valueStore = new VS();
  VAL *val = new VAL();
  if(absCode->getType() == NUM){
    val->type = NUMV;
    val->num = absCode->num;
    valueStore->setValue(val);
    valueStore->setStore(store);
    return valueStore;
  }
  else if(absCode->getType() == ADD){
    VS *tempLhs = new VS();
    VS *tempRhs = new VS();
    tempLhs = interprete(absCode->getLhs(),defSub,store);
    tempRhs = interprete(absCode->getRhs(),defSub,tempLhs->store);
    val = addVnum(tempLhs->value, tempRhs->value);
    valueStore->setValue(val);
    valueStore->setStore(tempRhs->store);
    return valueStore;
  }
  else if(absCode->getType() == SUB){
    VS *tempLhs = new VS();
    VS *tempRhs = new VS();
    tempLhs = interprete(absCode->getLhs(),defSub,store);
    tempRhs = interprete(absCode->getRhs(),defSub,tempLhs->store);
    val = subVnum(tempLhs->value, tempRhs->value);
    valueStore->setValue(val);
    valueStore->setStore(tempRhs->store);
    return valueStore;
  }
  else if(absCode->getType() == MUL){
    VS *tempLhs = new VS();
    VS *tempRhs = new VS();
    tempLhs = interprete(absCode->getLhs(),defSub,store);
    tempRhs = interprete(absCode->getRhs(),defSub,tempLhs->store);
    val = mulVnum(tempLhs->value, tempRhs->value);
    valueStore->setValue(val);
    valueStore->setStore(tempRhs->store);
    return valueStore;
  }
  else if(absCode->getType()==EQUAL){
    VS *tempLhs = new VS();
    VS *tempRhs = new VS();
    tempLhs = interprete(absCode->getLhs(),defSub,store);
    tempRhs = interprete(absCode->getRhs(),defSub,tempLhs->store);
    if(to_string(stoi(tempLhs->value->num)) == to_string(stoi(tempRhs->value->num))){
      val->num = "1";
    }
    else{
      val->num = "0";
    }
    valueStore->setValue(val);
    valueStore->setStore(tempRhs->store);
    return valueStore;
  }
  else if(absCode->getType() == ID){
    string address = lookup(absCode->id, defSub);
    valueStore = storelookup(address,store,store);
    return valueStore;
  }
  else if(absCode->getType() == FUN){
    val->type = CLOSUREV;
    val->param = absCode->param;
    val->fbody = absCode->fbody;
    val->dsClosure = defSub;
    valueStore->setValue(val);
    valueStore->setStore(store);
    return valueStore;
  }
  else if(absCode->getType() == REFUN){
    val->type = REFCLOSV;
    val->param = absCode->param;
    val->fbody = absCode->fbody;
    val->dsClosure = defSub;
    valueStore->setValue(val);
    valueStore->setStore(store);
    return valueStore;
  }
  else if(absCode->getType() == APP){
    VS *temp1 = interprete(absCode->fun_expr, defSub, store);
    if(temp1->value->type == CLOSUREV){
      VAL *arg_val = new VAL();
      arg_val->type = EXPRV;
      arg_val->expr = absCode->arg_expr;
      arg_val->dsClosure = defSub;
      string new_address = mall(temp1->store);
      DS *aSub = new DS();
      aSub->is_empty = 0;
      if(temp1->value->param.front() == '{'){
        aSub->name = temp1->value->param.substr(1, temp1->value->param.length() - 2);
      }
      else{
        aSub->name = temp1->value->param;
      }
      aSub->address = new_address;
      aSub->subDs = temp1->value->dsClosure;
      ST *aSto = new ST();
      aSto->is_empty = 0;
      aSto->address = new_address;
      aSto->value = arg_val;
      aSto->subSt = temp1->store;
      valueStore = interprete(temp1->value->fbody,aSub,aSto);
    }
    else if(temp1->value->type == REFCLOSV){
      string address = lookup(absCode->arg_expr->id,defSub);
      DS *aSub = new DS();
      aSub->is_empty = 0;
      if(temp1->value->param.front() == '{'){
        aSub->name = temp1->value->param.substr(1, temp1->value->param.length() - 2);
      }
      else{
        aSub->name = temp1->value->param;
      }
      aSub->address = address;
      aSub->subDs = temp1->value->dsClosure;
      valueStore = interprete(temp1->value->fbody,aSub,temp1->store);
    }
    else{
      fprintf(stderr,"trying to apply a number\n");
      exit(EXIT_FAILURE);
    }
    return valueStore;
  }
  else if(absCode->getType() == IFEXP){
    VS *temp = interprete(absCode->test_expr,defSub,store);
    if(stoi(temp->value->num) == 1){
      valueStore = interprete(absCode->then_expr,defSub,temp->store);
    }
    else{
      valueStore = interprete(absCode->else_expr,defSub,temp->store);
    }
    return valueStore;
  }
  else if(absCode->getType() == OROP){
    VS *temp1 = interprete(absCode->lhs,defSub,store);
    VS *temp2 = interprete(absCode->rhs,defSub,temp1->store);
    if((stoi(temp1->value->num) == 1) || (stoi(temp2->value->num) == 1)){
      val->type = NUMV;
      val->num = "1";
      valueStore->setValue(val);
      valueStore->setStore(temp2->store);
    }
    else{
      val->type = NUMV;
      val->num = "0";
      valueStore->setValue(val);
      valueStore->setStore(temp2->store);
    }
    return valueStore;
  }
  else if(absCode->getType() == NEWBOX){
    VS *temp;
    temp = interprete(absCode->value,defSub,store);
    string a = mall(temp->store);
    val->type = BOXV;
    val->address = a;
    ST *aSto = new ST();
    aSto->is_empty = 0;
    aSto->address = a;
    aSto->value = temp->value;
    aSto->subSt = temp->store;
    valueStore->setValue(val);
    valueStore->setStore(aSto);
    return valueStore;
  }
  else if(absCode->getType() == OPENBOX){
    VS *temp;
    temp = interprete(absCode->value,defSub,store);
    valueStore = storelookup(temp->value->address,temp->store,temp->store);
    return valueStore;
  }
  else if(absCode->getType() == SETBOX){
    VS *temp1, *temp2;
    temp1 = interprete(absCode->boxname,defSub,store);
    temp2 = interprete(absCode->value,defSub,temp1->store);
    ST *aSto = new ST();
    aSto->is_empty = 0;
    aSto->address = temp1->value->address;
    aSto->value = temp2->value;
    aSto->subSt = temp2->store;
    valueStore->setValue(temp2->value);
    valueStore->setStore(aSto);
    return valueStore;
  }
  else if(absCode->getType() == SEQN){
    VS *temp;
    temp = interprete(absCode->lhs,defSub,store);
    valueStore = interprete(absCode->rhs,defSub,temp->store);
    return valueStore;
  }
  else if(absCode->getType() == SETVAR){
    string a = lookup(absCode->id,defSub);
    VS *temp = interprete(absCode->value,defSub,store);
    ST *aSto = new ST();
    aSto->is_empty = 0;
    aSto->address = a;
    aSto->value = temp->value;
    aSto->subSt = temp->store;
    valueStore->setValue(temp->value);
    valueStore->setStore(aSto);
    return valueStore;
  }
}