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
class Calculate {};
class Value : public Expression {
 private:
  double const value;
 public:
  explicit Value(double val);
  virtual ~Value();
  double calculate() override ;
  void print();
};
class Variable : public Expression{
 protected:
  double value;
  string name;
 public:
  Variable(string name1, double value1);
  virtual ~Variable();
  double calculate() override;
  Variable &operator++();
  Variable &operator--();
  Variable &operator+=(double val);
  Variable &operator-=(double val);
  Variable &operator++(int);
  Variable &operator--(int);
  void print();
};
class BinaryOperator : public Expression{
 protected:
  Expression *right;
  Expression *left;
 public:
  explicit BinaryOperator(Expression *left1, Expression *right1);
  virtual ~BinaryOperator();
  virtual void print() = 0;
};
class Div : public BinaryOperator{
 public:
  Div(Expression *left1, Expression *right1);
  virtual ~Div();
  double calculate() override ;
  void print() override ;
};
class Minus : public BinaryOperator{
 public:
  Minus(Expression *left1, Expression *right1);
  virtual ~Minus();
  double calculate() override ;
  void print() override ;
};
class Mul : public BinaryOperator{
 public:
  Mul(Expression *left1, Expression *right1);
  virtual ~Mul();
  double calculate() override ;
  void print() override ;
};
class Plus : public BinaryOperator{
 public:
  Plus(Expression *left1, Expression *right1);
  virtual ~Plus();
  double calculate() override ;
  void print() override ;
};
class UnaryOperator : public Expression {
 protected:
  Expression *expression;
 public:
  explicit UnaryOperator(Expression *expression1);
  virtual ~UnaryOperator() ;
  virtual void print() = 0;
};
class UPlus : public UnaryOperator {
 public:
  explicit UPlus(Expression *expression1);
  double calculate() override;
  void print() override;
};
class UMinus : public UnaryOperator {
 public:
  explicit UMinus(Expression *expression1);
  double calculate() override;
  void print() override;
};
class ShuntingYard {
  stack<string> stack_;
  deque<string> deque_;
 public:
  ShuntingYard();
  ~ShuntingYard() = default;
  void fromInfixToPostfix(string& infixString, map<string, double>& values);
  deque<string> getPostfix();
  bool isNumber(string str);
};
class Interpreter {
  map<string, double> values;
  ShuntingYard sh;
  Expression* result;
 public:
  void setVariables(string str);
  void setVariablesHelper(string str);
  Expression* interpret(string str);
  void calculateExpression(deque<string>& deque);
  double fromStringToDouble(string& string1);
};

#endif //EX33__CALCULATE_H_
