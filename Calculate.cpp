#include "Calculate.h"
#include <map>
#include <string>
#include <string.h>
#include <queue>
#include <stack>
#include <ctype.h>
#include <stdlib.h>
/*
 * this classes from ex01 which can calculate experssion
 * */
Variable& Variable::operator++(){
    this->myValue = this->myValue + 1;
    return *this;
}
Variable& Variable::operator++(int){
    this->myValue = this->myValue + 1;
    return *this;
}
Variable& Variable::operator--(){
    this->myValue = this->myValue - 1;
    return *this;
}
Variable& Variable::operator--(int){
    this->myValue = this->myValue - 1;
    return *this;
}
Variable& Variable::operator+=(float x){
    this->myValue = this->myValue + x;
    return *this;
}

Variable& Variable::operator-=(float x){
    this->myValue = this->myValue - x;
    return *this;
}
// return the value of the variable
float Variable::calculate(){
    return this->myValue;
}
float Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}
float Minus::calculate() {
    return this->left->calculate() - this->right->calculate();
}
float Mul::calculate() {
    return this->left->calculate() * this->right->calculate();
}
float Div::calculate() {
    if (this->right->calculate() == 0) {
        // cant divide a number with zero
        throw ("division by zero");
    } else {
        return this->left->calculate() / this->right->calculate();
    }
}
float UMinus ::calculate() {
    return this->expr->calculate() * -1;
}
float UPlus ::calculate() {
    // return the value
    return this->expr->calculate();
}
Expression* Interpreter:: interpret(string expressionString) {
    queue <string> outPutQueue;
    stack <string> operatorStack;
    // creating the string of the value and pushing it to the outPutQueue
    for (unsigned i = 0; i < expressionString.length(); i++) {
        // if the variable start with letter we will check if it is set in the map
        if (isalpha(expressionString[i])) {
            string valStringName;
            //valStringName is the name of the variable that has been set
            valStringName = valStringName + expressionString[i];
            while (isalpha(expressionString[i+1]) || isdigit(expressionString[i+1]) || expressionString[i+1] == '_') {
                valStringName = valStringName + expressionString[i+1];
                i++;
            }
            map<string,string>::iterator it = this->variableMap.find(valStringName);
            if(it != this->variableMap.end()) {
                // taking the value of the variable from the map
                string val = it->second;
                outPutQueue.push(val);
            } else {
                throw ("illegal math expression");
            }
            continue;
        }
        // creating negative float number
        if (expressionString[i] == '-' && isdigit(expressionString[i+1])) {
            string number = "-";
            while (isdigit(expressionString[i+1]) || expressionString[i+1] == '.' ) {
                number = number + expressionString[i+1];
                i++;
            }
            if (number.compare("") != 0){
                outPutQueue.push(number);
                continue;
            }
            continue;
        }
        // creating float number
        bool enter = false;
        string number;
        while (isdigit(expressionString[i]) || expressionString[i] == '.' ) {
            number = number + expressionString[i];
            i++;
            enter = true;
        }
        if(enter) {
            i--;
        }
        if (number.compare("") != 0){
            outPutQueue.push(number);
            continue;
        }
        // pushing - or + operator
        if (expressionString[i] == '+' || expressionString[i] == '-' ) {
            // "-()"
            if (expressionString[i] == '-' && (i == 0 || expressionString[i+1] == '(')) {
                operatorStack.push("@");
                continue;
            }
            //+() only at the start of the expression
            if (expressionString[0] == '+' && (i == 0 || expressionString[i+1] == '(')) {
                operatorStack.push("%");
                continue;
            }
            // (+())
            if (expressionString[i] == '+' && expressionString[i+1] == '(' &&  expressionString[i-1] == '(' ) {
                operatorStack.push("$");
                continue;
            }
            // creating praiority order in operatorStack
            if (!operatorStack.empty()) {
                while (operatorStack.top() == "*" || operatorStack.top() == "/") {
                    outPutQueue.push(operatorStack.top());
                    operatorStack.pop();
                }
                string s(1, expressionString[i]);
                operatorStack.push(s);
            } else {
                string s(1, expressionString[i]);
                operatorStack.push(s);
            }
            // pushing * or / operator
        } else if (expressionString[i] == '*' ||expressionString[i] == '/' ) {
            string s(1, expressionString[i]);
            operatorStack.push(s);
        }
            // pushing ( operator
        else if (expressionString[i] == '(') {
            string s(1, expressionString[i]);
            operatorStack.push(s);
        }
            // erasing ( operator and pop all the operators until ) operator
        else if (expressionString[i] == ')') {
            while (operatorStack.top().compare("(") != 0) {
                outPutQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            // pop the left bracket from the stack and discard it
            operatorStack.pop();

            if (!operatorStack.empty()) {
                if (operatorStack.top().compare("@") == 0) {
                    outPutQueue.push("@");
                    operatorStack.pop();
                    continue;
                }
                if (operatorStack.top().compare("$") == 0) {
                    outPutQueue.push("$");
                    operatorStack.pop();
                    continue;

                }
                if (operatorStack.top().compare("#") == 0) {
                    outPutQueue.push("#");
                    operatorStack.pop();
                    continue;

                }
            }
        }
    }
    // pop all the operator that left
    while (!operatorStack.empty()) {
        outPutQueue.push(operatorStack.top());
        operatorStack.pop();
    }
    stack <Expression*> resultStack;
    while (!outPutQueue.empty()) {
        float value = strtod(outPutQueue.front().c_str(), NULL);
        if (outPutQueue.front().compare("0") == 0) {
            // this is the number zero
            resultStack.push(new Value(0));
            outPutQueue.pop();
            continue;
        } else if (value != 0) {
            // this is number that diffrent from zero
            resultStack.push(new Value(value));
            outPutQueue.pop();
            continue;
        } else {
            // creating the right expression
            Expression *right = resultStack.top();
            if (!isnumber(to_string(right->calculate()))) {
                throw ("illegal variable assignment!");
            }
            resultStack.pop();
            if (outPutQueue.front().compare("@") == 0) {
                outPutQueue.pop();
                // -()
                resultStack.push(new UMinus(right));
                continue;
            }
            if (outPutQueue.front().compare("$") == 0) {
                outPutQueue.pop();
                if(outPutQueue.empty()) {
                    //+() in the start
                    return new UPlus(right);
                }
                //+()
                resultStack.push(new UPlus(right));
                continue;
            }
            if (outPutQueue.front().compare("%") == 0) {
                outPutQueue.pop();
                //+()
                return new UPlus(right);
            }
            if (resultStack.empty()) {
                throw ("too many operators!");
            }
            Expression *left = resultStack.top();
            if (!isnumber(to_string(left->calculate()))) {
                throw ("illegal variable assignment!");
            }
            resultStack.pop();
            if (outPutQueue.front().compare("+") == 0) {
                outPutQueue.pop();
                resultStack.push(new Plus(left, right));
                continue;
            } else if (outPutQueue.front().compare("-")== 0) {
                outPutQueue.pop();
                resultStack.push(new Minus(left, right));
                continue;
            } else if (outPutQueue.front().compare("*")== 0) {
                outPutQueue.pop();
                resultStack.push(new Mul(left, right));
                continue;
            } else if (outPutQueue.front().compare("/")== 0) {
                if (right->calculate() == 0.0) {
                    throw ("division by zero");
                } else {
                    outPutQueue.pop();
                    resultStack.push(new Div(left, right));
                    continue;
                }
            }
        }
    }
    if(resultStack.size() == 1) {
        return resultStack.top();
    } else if (resultStack.size() > 1) {
        // pop all the expression that left
        while (resultStack.size() != 1) {
            Expression *r = resultStack.top();
            resultStack.pop();
            Expression *l = resultStack.top();
            resultStack.pop();
            resultStack.push(new Plus(l, r));
        }
    }
    return resultStack.top();
}
// the methos check if the string is a number
bool  Interpreter :: isnumber(string s) {
    int stringLength = s.length();
    char* sArray = new char[stringLength + 1];
    strcpy(sArray, s.c_str());
    bool flag = false;
    if (s.length() == 1 && isdigit(s.at(0))) {
        return true;
    }
    for (unsigned i=1; i<s.length();i++)
    {
        if (isdigit(s[i])) {
            flag = true;
        } else if ((s.length() > 2 && s[i]=='.' && isdigit(s[i-1]) && isdigit(s[i+1]))) {
            flag = true;
        } else {
            return false;
        }
    }
    return flag;
}
// set the variable in map
void Interpreter :: setVariables(string variables) {
    size_t pos = 0;
    while ((pos = variables.find(";")) != std::string::npos) {
        string token;
        string valName; //the variable name
        string valValueString; // the value
        token = variables.substr(0, pos); //take the part x=2
        variables.erase(0, pos + 1);
        unsigned tokenLength = token.length();
        char* tokenArray = new char[tokenLength +1];
        strcpy(tokenArray, token.c_str());
        int lenghtValString = 1;
        if (isalpha(tokenArray[0])) {
            valName = valName + tokenArray[0];
            while (tokenArray[lenghtValString] != '=') {
                //creating the variable name
                if (isdigit(tokenArray[lenghtValString]) || isalpha(tokenArray[lenghtValString]) ||
                    tokenArray[lenghtValString] == '_') {
                    valName = valName + tokenArray[lenghtValString];
                    lenghtValString++;
                } else {
                    throw ("illegal variable assignment!");
                }
            }
            for (unsigned i = lenghtValString + 1; i < tokenLength; i++) {
                //creating the value
                valValueString = valValueString + tokenArray[i];
            }
            if (isnumber(valValueString)) {
                this->variableMap[valName] = valValueString;
                continue;
            } else {
                throw ("illegal variable assignment!");
            }
        } else {
            throw ("the value dosnt start with letter");
        }
    }
    // puting the last value into map
    if (isalpha(variables.at(0))) {
        string valName;
        string valValueString;

        unsigned variablesLength = variables.length();
        char* variablesArray = new char[variablesLength +1];
        strcpy(variablesArray, variables.c_str());
        int lenghtValString = 1;

        valName = valName + variables[0];
        while (variablesArray[lenghtValString] != '=') {
            if (isdigit(variablesArray[lenghtValString]) || isalpha(variablesArray[lenghtValString]) ||
                variablesArray[lenghtValString] == '_') {
                valName = valName + variablesArray[lenghtValString];
                lenghtValString++;
            } else {
                throw ("invalid value name");
            }
        }
        for (unsigned i = lenghtValString + 1; i < variablesLength; i++) {
            valValueString = valValueString + variablesArray[i];
        }
        float valueDouble = strtod(valValueString.c_str(), NULL);
        if ((valueDouble == 0 && valValueString.compare("0") == 0 )|| valValueString.compare("0.000000") == 0||valValueString.compare("-0.000000") == 0) {
            this->variableMap[valName] = "0";
        } else if (valueDouble != 0) {
            this->variableMap[valName] = valValueString;
        } else {
            throw ("the value of the variable isnt double number");
        }
    } else {
        throw ("the value name is illegal");
    }
}