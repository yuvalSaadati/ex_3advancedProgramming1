#include "Calculate.h"
Value::Value(double val) : value(val) {}
Value::~Value() {}
double Value::calculate() {
  return value;
}
void Value::print() {
  cout<<value<<endl;
}
Variable::Variable(string name1, double value1) {
  name = name1;
  value = value1;
}
Variable::~Variable() {}
double Variable::calculate() {
  return value;
}
Variable &Variable::operator++() {
  value = value + 1;
  return *this;
}
Variable &Variable::operator--() {
  value = value - 1;
  return *this;
}
Variable &Variable::operator++(int) {
  value = value + 1;
  return *this;
}
Variable &Variable::operator--(int) {
  value = value - 1;
  return *this;
}
Variable &Variable::operator+=(double val) {
  value = value + val;
  return *this;
}
Variable &Variable::operator-=(double val) {
  value = value - val;
  return *this;
}
void Variable::print() {
  cout<<name<<"="<<value<<endl;
}
BinaryOperator::BinaryOperator(Expression *left1, Expression *right1) {
  right = right1;
  left = left1;
}
BinaryOperator::~BinaryOperator() {
  delete(right);
  delete(left);
}
Div::Div(Expression *left1, Expression *right1) : BinaryOperator(left1, right1){}
Div::~Div() {}
double Div::calculate() {
  if(right->calculate() == 0) {
    throw ("division by zero");
  }
  return left->calculate() / right->calculate();
}
void Div::print() {
  cout<<"("<<left<<"/"<<right<<")"<<endl;
}
Minus::Minus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1){}
Minus::~Minus() {}
double Minus::calculate() {
  return right->calculate() - left->calculate();
}
void Minus::print() {
  cout<<"("<<left<<"-"<<right<<")"<<endl;
}
Mul::Mul(Expression *left1, Expression *right1) : BinaryOperator(left1, right1){}
Mul::~Mul() {}
double Mul::calculate() {
  return left->calculate() * right->calculate();
}
void Mul::print() {
  cout<<"("<<left<<"*"<<right<<")"<<endl;
}
Plus::Plus(Expression *left1, Expression *right1) : BinaryOperator(left1, right1){}
Plus::~Plus() {}
double Plus::calculate() {
  return right->calculate() + left->calculate();
}
void Plus::print() {
  cout<<"("<<left<<"+"<<right<<")"<<endl;
}
UnaryOperator::UnaryOperator(Expression *expression1) {
  expression = expression1;
}
UnaryOperator::~UnaryOperator() {
  delete(expression);
}
UPlus::UPlus(Expression *expression1) : UnaryOperator(expression1) {
  expression = expression1;
}
double UPlus::calculate() {
  return expression->calculate();
}
void UPlus::print() {
  cout<<"("<<expression<<")"<<endl;
}
UMinus::UMinus(Expression *expression1) : UnaryOperator(expression1) {
  expression = expression1;
}
double UMinus::calculate() {
  return expression->calculate() * (-1);
}
void UMinus::print() {
  cout<<"-("<<expression<<")"<<endl;
}
ShuntingYard::ShuntingYard() {}
void ShuntingYard::fromInfixToPostfix(string& infixString, map<string, double>& values) {
  //check if I get ** OR //
  if (infixString.find("**") != string::npos) {
    throw ("illegal math expression");
  }
  if (infixString.find("//") != string::npos) {
    throw ("illegal math expression");
  }
  if (infixString.find("--") != string::npos) {
    throw ("illegal math expression");
  }
  if (infixString.find("++") != string::npos) {
    throw ("illegal math expression");
  }
  for (int i = 0; (unsigned)i < infixString.size(); i++) {
    string stringToCheck = infixString.substr(i, 1);
    //if it's a letter
    if (regex_match(stringToCheck, regex("^[a-zA-Z][0-9]?_?$"))) {
      if(infixString.size() > (unsigned)i && isNumber(infixString.substr(i+1, 1))) {
        throw ("Error");
      }
      if (values.find(stringToCheck) == values.end()) {
        // not found
        throw ("variable doesn't exist");
      } else {
        // found
        double x = values[stringToCheck];
        this->deque_.push_back(to_string(x));
      }
    }
      //if it's an operator
    else if (regex_match(stringToCheck, regex("^[-+()*\\/]$"))) {
      if (stringToCheck == ")") {
        while (this->stack_.top() != "(" && !this->stack_.empty()) {
          this->deque_.push_back(this->stack_.top());
          this->stack_.pop();
        }
        if (this->stack_.empty()) {
          throw ("brackets error");
        } else {
          this->stack_.pop();
        }
        //if it's a Unary operators
      } else if (stringToCheck == "+" || stringToCheck == "-") {
        string lastToken(1, infixString[i - 1]);
        if (stringToCheck == "+") {
          if (i == 0 || lastToken == "(") {
            this->stack_.push("@");
          }
          else {
            this->stack_.push("+");
          }
        }
        if (stringToCheck == "-") {
          if (i == 0 || lastToken == "(") {
            this->stack_.push("#");
          }
          else {
            this->stack_.push("-");
          }
        }
      } else { //for all other operators
        this->stack_.push(stringToCheck);
      }
    }
      //if it's a number
    else {
      string token = stringToCheck;
      int flag = 0;
      int chars = 1;
      while (isNumber(stringToCheck) && (unsigned)(i + chars) < infixString.length()) {
        flag = 1;
        token = stringToCheck;
        stringToCheck = infixString.substr(i, ++chars);
      }
      this->deque_.push_back(token);
      if (flag == 1) {
        i = i + chars - 2;
      }
    }
  }
  //clear the stack
  while (!this->stack_.empty() && this->stack_.top() != "(") {
    this->deque_.push_back(this->stack_.top());
    this->stack_.pop();
  }
  if (!this->stack_.empty() && this->stack_.top() == "(") {
    throw("brackets error");
  }
}
deque<string> ShuntingYard::getPostfix() {
  return this->deque_;
}
bool ShuntingYard::isNumber(string str) {
  return regex_match(str, regex("^[-+]?[0-9]+\\.?[0-9]*$"));
}
void Interpreter::setVariables(string str) {
  string commaDelimiter = ";";
  size_t commaPos = 0;
  string token;
  while ((commaPos = str.find(commaDelimiter)) != string::npos) {
    token = str.substr(0, commaPos);
    setVariablesHelper(token);
    str.erase(0, commaPos + commaDelimiter.length());
  }
  setVariablesHelper(str);
}
void Interpreter::setVariablesHelper(string str) {
  regex matchLetter("^[a-zA-Z][0-9]?[_]?[0-9]?$");
  regex matchNumber("^[-+]?[0-9]+\\.?[0-9]*$");
  string equalDelimiter = "=";
  size_t equalPos = 0;
  string beforeEqual, afterEqual;
  if ((equalPos = str.find(equalDelimiter)) != string::npos) {
    afterEqual = str.substr(str.find(equalDelimiter) + 1);
    beforeEqual = str.substr(0, equalPos);
    if (!regex_match(afterEqual, matchNumber)) {
      throw ("illegal variable assignment!");
    }
    if (!regex_match(beforeEqual, matchLetter)) {
      throw ("illegal variable assignment!");
    }
    if (this->values.find(beforeEqual) != this->values.end()) {
      this->values.find(beforeEqual)->second = fromStringToDouble(afterEqual);
    }
    else {
      this->values.insert(pair<string, double>(beforeEqual, atof(afterEqual.c_str())));
    }
  }
}
Expression* Interpreter::interpret(string str) {
  sh.fromInfixToPostfix(str, this->values);
  deque<string> deque = sh.getPostfix();
  calculateExpression(deque);
  return this->result;
}
void Interpreter::calculateExpression(deque<string>& deque) {
  regex machNumber("^[-+]?[0-9]+\\.?[0-9]*$");
  stack<double> stackCalculate;
  while (!deque.empty()) {
    //if its a number
    if (regex_match(deque.front(), machNumber)) {
      stackCalculate.push(fromStringToDouble(deque.front()));
    }
      // if its a unary operator
    else if(deque.front() == "#") {
      Expression* e = new UMinus(new Value(stackCalculate.top()));
      stackCalculate.pop();
      stackCalculate.push(e->calculate());
    }
    else if(deque.front() == "@") {
      Expression* e = new UPlus(new Value(stackCalculate.top()));
      stackCalculate.pop();
      stackCalculate.push(e->calculate());
    }
      // if its a binary operator
    else if (deque.front() == "*") {
      double operand_1 = stackCalculate.top();
      stackCalculate.pop();
      double operand_2 = stackCalculate.top();
      stackCalculate.pop();
      Expression* e = new Mul(new Value(operand_1), new Value(operand_2));
      stackCalculate.push(e->calculate());
    }
    else if (deque.front() == "+") {
      double operand_1 = stackCalculate.top();
      stackCalculate.pop();
      double operand_2 = stackCalculate.top();
      stackCalculate.pop();
      Expression* e = new Plus(new Value(operand_1), new Value(operand_2));
      stackCalculate.push(e->calculate());
    }
    else if (deque.front() == "-") {
      double operand_1 = stackCalculate.top();
      stackCalculate.pop();
      double operand_2 = stackCalculate.top();
      stackCalculate.pop();
      Expression* e = new Minus(new Value(operand_1), new Value(operand_2));
      stackCalculate.push(e->calculate());
    }
    else if (deque.front() == "/") {
      double operand_1 = stackCalculate.top();
      stackCalculate.pop();
      double operand_2 = stackCalculate.top();
      stackCalculate.pop();
      Expression* e = new Div(new Value(operand_1), new Value(operand_2));
      stackCalculate.push(e->calculate());
    }
    deque.erase(deque.begin());
  }
  this->result = new Value(stackCalculate.top());
}
double Interpreter::fromStringToDouble(string &string1) {
  string::size_type sz;
  double num = stod (string1,&sz);
  return num;
}
