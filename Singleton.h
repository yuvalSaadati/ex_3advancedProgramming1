//
// Created by yuval on 18/12/2019.
//

#ifndef EX33_SINGLETON_H
#define EX33_SINGLETON_H
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "CommandInterface.h"
#include "commandPattern.h"
#include "Singleton.h"
#include <map>


class Singleton {
private:
    /* Here will be the instance stored. */
    static Singleton* instance;
    /* Private constructor to prevent instancing. */
    Singleton();

public:
    /* Static access method. */
    static Singleton* getInstance();
    map<string,CommandInterface*> commandMAp;
    //map<string,VaribableData> symbolTable;
    void parser(vector<string> lexerVector);
    vector<string> lexer(string s);

};


#endif //EX33_SINGLETON_H
