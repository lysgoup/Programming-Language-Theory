#include <iostream>
#include <regex>
#include "RBMRLFAE.h"

class Parser
{
public:
    // Parser();
    vector<string> splitExpressionAsSubExpressions(string exampleCode);
    vector<string> getSubExpressions(string exampleCode);
    AST *parse(string exampleCode);
    bool isNumeric(string str);
    string desugar(string exampleCode);
    bool isRecursion(string exampleCode);
};

vector<string> Parser::splitExpressionAsSubExpressions(string exampleCode)
{
    if ((exampleCode.front() == '{' && exampleCode.back() != '}') || (exampleCode.front() != '{' && exampleCode.back() == '}'))
        exit(0);

    if (exampleCode.front() == '{')
        exampleCode = exampleCode.substr(1, exampleCode.length() - 1);

    return getSubExpressions(exampleCode);
}

vector<string> Parser::getSubExpressions(string exampleCode)
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
            if(exampleCode.at(i) == '\n'){
                continue;
            }
            else if (exampleCode.at(i) == '{' && openingParenthesisCount == 0)
            {
                openingParenthesisCount++;
                // Ready to start a new buffer
                strBuffer = "";
                strBuffer += exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == '{')
            {
                openingParenthesisCount++;
                strBuffer = strBuffer + exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == '}' && openingParenthesisCount > 0)
            {
                openingParenthesisCount--;
                strBuffer = strBuffer + exampleCode.at(i);
                if(openingParenthesisCount == 0){
                    sexpressions.push_back(strBuffer);
                    strBuffer = "";
                }
                continue;
            }
            else if (exampleCode.at(i) == '}')
            {
                // buffer is ready to be a subexpression
                //sexpressions.push_back(strBuffer);
                continue;
            }
        }
        strBuffer = strBuffer + exampleCode.at(i);
    }
    if(strBuffer.size()>0){
        sexpressions.push_back(strBuffer);
    }
    return sexpressions;
}

AST *Parser::parse(string exampleCode)
{
    vector<string> subExpressions = splitExpressionAsSubExpressions(exampleCode);
    // for(int i=0; i<subExpressions.size(); i++){
    //     cout << subExpressions.at(i) << "\n";
    // }
    if (subExpressions.size() == 1 && isNumeric(subExpressions.front()))
    {
        // AST* num = new Num();
        AST *num = new AST();
        num->type = NUM;
        //cout << subExpressions.front();
        num->createNum(subExpressions.front());
        return num;
    }
    else if (subExpressions.size() == 1 && !isNumeric(subExpressions.front()))
    {
        // AST* num = new Num();
        AST *id = new AST();
        id->type = ID;
        //cout << subExpressions.front();
        id->createId(subExpressions.front());
        return id;
    }

    if (subExpressions.front() == "+")
    {
        // AST* num = new Num();
        AST *add = new AST();
        add->type = ADD;
        add->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return add;
    }
    else if (subExpressions.front() == "-")
    {
        // AST* num = new Num();
        AST *sub = new AST();
        sub->type = SUB;
        sub->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return sub;
    }
    else if (subExpressions.front() == "*")
    {
        // AST* num = new Num();
        AST *mul = new AST();
        mul->type = MUL;
        mul->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return mul;
    }
    else if (subExpressions.front() == "=")
    {
        // AST* num = new Num();
        AST *eq = new AST();
        eq->type = EQUAL;
        eq->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return eq;
    }
    else if (subExpressions.front() == "fun")
    {
        // cout << "here: fun\n";
        // AST* num = new Num();
        AST *fun = new AST();
        fun->type = FUN;
        string param = subExpressions.at(1);
        param = param.substr(1, param.size()-2);
        fun->createParam(param);
        fun->createFunction(parse(subExpressions.at(2)));
        return fun;
    }
    else if (subExpressions.front() == "refun")
    {
        // AST* num = new Num();
        AST *refun = new AST();
        refun->type = REFUN;
        refun->createParam(subExpressions.at(1));
        refun->createFunction(parse(subExpressions.at(2)));
        return refun;
    }
    else if (subExpressions.front() == "with")
    {
        // cout << "here: with\n";
        if(subExpressions.at(2) == ""){
            fprintf(stderr, "There is no body expression");
		    exit(EXIT_FAILURE);
        }
        AST *app = new AST();
        vector<string> temp = splitExpressionAsSubExpressions(subExpressions.at(1));
        string function = "{fun {";
        function += temp.at(0);
        function += "} ";
        function += subExpressions.at(2);
        function += "}";
        app->type = APP;
        app->createFunexpr(parse(function));
        app->createArgexpr(parse(temp.at(1)));
        return app;
    }
    else if(subExpressions.front() == "if")
    {
        AST *ifexp = new AST();
        ifexp->type = IFEXP;
        ifexp->createTestexpr(parse(subExpressions.at(1)));
        ifexp->createThenexpr(parse(subExpressions.at(2)));
        ifexp->createElseexpr(parse(subExpressions.at(3)));
        return ifexp;
    }
    else if(subExpressions.front() == "or")
    {
        AST *orop = new AST();
        orop->type = OROP;
        orop->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return orop;
    }
    else if(subExpressions.front() == "newbox")
    {
        AST *newbox = new AST();
        newbox->type = NEWBOX;
        newbox->createValue(parse(subExpressions.at(1)));
        return newbox;
    }
    else if(subExpressions.front() == "setbox")
    {
        AST *setbox = new AST();
        setbox->type = SETBOX;
        setbox->createBoxname(parse(subExpressions.at(1)));
        setbox->createValue(parse(subExpressions.at(2)));
        return setbox;
    }
    else if(subExpressions.front() == "openbox")
    {
        AST *openbox = new AST();
        openbox->type = OPENBOX;
        openbox->createValue(parse(subExpressions.at(1)));
        return openbox;
    }
    else if(subExpressions.front() == "seqn")
    {
        AST *seqn = new AST();
        seqn->type = SEQN;
        seqn->createAdd(parse(subExpressions.at(1)),parse(subExpressions.at(2)));
        return seqn;
    }
    else if(subExpressions.front() == "setvar")
    {
        AST *setvar = new AST();
        setvar->type = SETVAR;
        setvar->createId(subExpressions.at(1));
        setvar->createValue(parse(subExpressions.at(2)));
        return setvar;
    }
    //parse function application
    else if (subExpressions.size() == 2){
        // AST* num = new Num();
        AST *app = new AST();
        app->type = APP;
        app->createFunexpr(parse(subExpressions.at(0)));
        app->createArgexpr(parse(subExpressions.at(1)));
        return app;
    }
}

bool Parser::isNumeric(string str)
{
    regex pattern("-?\\d+(\\.\\d+)?");
    smatch match;
    return regex_match(str, match, pattern);
}

string Parser::desugar(string exampleCode){
    string desugared = "";
    vector<string> SubExpressions = splitExpressionAsSubExpressions(exampleCode);
    vector<string> SubsubExpressions;
    if(SubExpressions.at(0) == "with"){
        SubsubExpressions = splitExpressionAsSubExpressions(SubExpressions.at(1));
    }
    desugared += "{with {mk-rec {fun {body-proc} {with {fX {fun {fY} {with {f {fun {x} {{fY fY} x}}} {body-proc f}}}}{fX fX}}}} {with {";
    desugared += SubsubExpressions.at(0);
    desugared += " {mk-rec {fun {";
    desugared += SubsubExpressions.at(0);
    desugared += "} ";
    desugared += SubsubExpressions.at(1);
    desugared += "}}} ";
    desugared += SubExpressions.at(2);
    desugared += "}}";
    return desugared;
}

bool Parser::isRecursion(string exampleCode)
{
    vector<string> sub = splitExpressionAsSubExpressions(exampleCode);
    if(sub.at(0) == "with"){
        vector<string> sub1 = splitExpressionAsSubExpressions(sub.at(1));
        // for(int i=0; i<sub1.size(); i++){
        //     cout << sub1.at(i) << "\n";
        // }
        vector<string> sub2 = splitExpressionAsSubExpressions(sub1.at(1));
        // for(int i=0; i<sub2.size(); i++){
        //     cout << sub2.at(i) << "\n";
        // }
        if((sub2.size() > 2) && (sub2.at(2).find(sub1.at(0)) != string::npos)){
            return true;
        }
    }

    return false;
}