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
#include <regex>
#include "CommandInterface.h"
#include "ObjectData.h"
using namespace std;
/*
 * Singleton will save all the data structure that we will need to use during the program
 */
class Singleton {
public:
    Singleton();
    static Singleton* instance;
    // get the object
    static Singleton* getInstance();
    // map of all the variables. the key is the name of the variable and the value is ObjectData
    map<string,ObjectData*> symbolTable;
    // map of all the command object in fly.txt
    map<string,CommandInterface*> commandMAp;
    // array which save the sim of each variable
    string valuesFromSim[36];
    // queue of all the set command that weill be send to server
    queue<string>messages_from_client;
    // save the last variables in the buffer when the line dosn't ended with'\n'
    string leftVals;
    // all the function in Singleton are :
    void parser(vector<string> lexerVector);
    vector<string> lexer(string s);
    void createSimValuesMap();
    float calculateExpression(string s);
    void simDataParser(char * buffer);
};
#endif //EX33_SINGLETON_H