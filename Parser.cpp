#include <iostream>
#include <regex>
#include "FAE.h"

class Parser
{
public:
    // Parser();
    vector<string> splitExpressionAsSubExpressions(string exampleCode);
    vector<string> getSubExpressions(string exampleCode);
    AST *parse(string exampleCode);
    bool isNumeric(string str);
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
            if (exampleCode.at(i) == '{' && openingParenthesisCount == 0)
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
    //parse function application
    else if (subExpressions.size() == 2 && subExpressions.front()[0]=='{'){
        // AST* num = new Num();
        AST *app = new AST();
        app->type = APP;
        app->createFunexpr(parse(subExpressions.at(0)));
        app->createArgexpr(parse(subExpressions.at(1)));
        return app;
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
    else if (subExpressions.front() == "fun")
    {
        // AST* num = new Num();
        AST *fun = new AST();
        fun->type = FUN;
        fun->createParam(subExpressions.at(1));
        fun->createFunction(parse(subExpressions.at(2)));
        return fun;
    }
    else if (subExpressions.front() == "with")
    {
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
}

bool Parser::isNumeric(string str)
{
    regex pattern("-?\\d+(\\.\\d+)?");
    smatch match;
    return regex_match(str, match, pattern);
}