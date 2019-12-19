//
// Created by yuval on 18/12/2019.
//
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "CommandInterface.h"
#include "Singleton.h"
using namespace std;
#ifndef EX33_COMMANPATTERN_H
#define EX33_COMMANPATTERN_H

static bool is_done = false;
class commandPattern {

};

struct VaribableData
{
    int inOut; // in(->)=1 out(<-)=0
    string sim;
    int value = 0;
};

class OpenServerCommand : public CommandInterface{
    mutex mutex_lock;
public:
    OpenServerCommand () {}
    static int serverThread(int port);
    void xmlParser();
    virtual int execute(vector<string> valString);
    virtual ~OpenServerCommand() {}

};

class ConnectCommand : public CommandInterface{
    mutex mutex_lock;
public:
    ConnectCommand () {}
    static int clientThread(string ip, int port);
    virtual int execute(vector<string> valString);
    virtual ~ConnectCommand() {}
};

class DefineVarCommand : public CommandInterface{
    map<string,VaribableData> symbolTable;
    //string name;
    //struct VaribableData objectData;

    //adding here files and constrctor
public:
    DefineVarCommand () {}
    virtual int execute(vector<string> valString);
    virtual ~DefineVarCommand() {}
};

class PrintCommand : public CommandInterface{
    string outPutString;
public:
    PrintCommand () {}
    virtual int execute(vector<string> valString);
    virtual ~PrintCommand() {}
};

class SleepCommand : public CommandInterface{
    int sleep;
public:
    SleepCommand () {}
    virtual int execute(vector<string> valString);
    virtual ~SleepCommand() {}
};

class ConditionParser : public CommandInterface{
    bool condition;
public:
    ConditionParser () {}
    virtual int execute(vector<string> valString);
    virtual ~ConditionParser() {}
};

class LoopCommand : public CommandInterface{
    bool condition;
public:
    LoopCommand () {}
    virtual int execute(vector<string> valString);
    virtual ~LoopCommand() {}
};

class IfCommand : public CommandInterface{
    bool condition;
public:
    IfCommand () {}
    virtual int execute(vector<string> valString);
    virtual ~IfCommand() {}
};
#endif //EX33_COMMANPATTERN_H
