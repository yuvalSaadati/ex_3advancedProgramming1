#ifndef EX33_SINGLETON_H
#define EX33_SINGLETON_H
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "CommandInterface.h"
#include "commandPattern.h"
#include "Singleton.h"
#include <map>
#include <queue>
#include "CommandInterface.h"
#include "ObjectData.h"
using namespace std;
class Singleton {
public:
    Singleton();
    static Singleton* instance;
    static Singleton* getInstance();
    map<string,ObjectData*> symbolTable;
    map<string,CommandInterface*> commandMAp;
    string valuesFromSim[36];
    void parser(vector<string> lexerVector);
    vector<string> lexer(string s);
    void createSimValuesMap();
    float calculateExpression(string s);
    queue<string>messages_from_client;
};
#endif //EX33_SINGLETON_H