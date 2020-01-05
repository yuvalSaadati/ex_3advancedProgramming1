#ifndef EX33__CALCULATE_H_
#define EX33__CALCULATE_H_
#include <iostream>
#include <stack>
#include <string>
#include <cstring>
#include <deque>
#include <regex>
#include "Expression.h"
using namespace std;
/*
 * this classes from ex01 which can calculate experssion
 * */
class Calculate {};

class Variable : public Expression{
    string valName;
    float myValue;
public:
    Variable (string name, float x):valName(name), myValue(x) {}
    float calculate();
    Variable& operator++();
    Variable& operator++(int);
    Variable& operator--();
    Variable& operator--(int);
    Variable& operator+=(float x);
    Variable& operator-=(float x);
    virtual ~Variable() {}

};

class BinaryOperator : public Expression  {
protected:
    Expression* left;
    Expression* right;
    virtual ~BinaryOperator() {
        delete left;
        delete right;
    }
};

class UnaryOperator : public Expression  {
protected:
    Expression* expr;
    virtual ~UnaryOperator() {
        delete expr;
    }
};

class Value : public Expression {
    double value;
public:
    Value (float x): value(x) {}
    float calculate() { return  this->value; }
    virtual ~Value() {}

};

class Plus : public BinaryOperator {
public:
    Plus(Expression* l, Expression* r) {
        this->left = l;
        this->right = r;
    }
    float calculate();
    virtual ~Plus() {}

};

class Minus : public BinaryOperator {
public:
    Minus(Expression* l, Expression* r) {
        this->left = l;
        this->right = r;
    }
    float calculate();
    virtual ~Minus() {}

};

class Mul : public BinaryOperator {
public:
    Mul(Expression* l, Expression* r) {
        this->left = l;
        this->right = r;
    }
    float calculate();
    virtual ~Mul() {}

};

class Div : public BinaryOperator {
public:
    Div(Expression* l, Expression* r) {
        this->left = l;
        this->right = r;
    }
    float calculate();
    virtual ~Div() {}

};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression* e) {
        this->expr = e;
    }
    float calculate();
    virtual ~UPlus() {}

};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression* e) {
        this->expr = e;
    }
    float calculate() override;
    virtual ~UMinus() {}

};
class Interpreter{
public:
    map<string,string> variableMap;
    Interpreter() {}
    Expression * interpret(string expressionString);
    void setVariables(string variables);
    bool isnumber(string s);
    virtual ~Interpreter() {}
};
#endif //EX33__CALCULATE_H_