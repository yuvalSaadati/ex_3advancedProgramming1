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
#include "Expression.h"
#include "Calculate.h"
#include <mutex>

using namespace std;
#ifndef EX33_COMMANPATTERN_H
#define EX33_COMMANPATTERN_H
static bool is_done = false;
//static bool getInfo = true;

class commandPattern {};

class OpenServerCommand : public CommandInterface{
   int client_socket;
   int port;
public:
    int openServer();
    void simDataParser(char buffer[]);
    virtual int execute(vector<string> valString);
        int readFromSim();
virtual ~OpenServerCommand() {}
};

class ConnectCommand : public CommandInterface{
  int client_socket;
  string ip;
  int port;
public:
    int openClient();
    int execute(vector<string> valString) override;
    virtual ~ConnectCommand() {}
};

class DefineVarCommand : public CommandInterface{
    string name;
    string simDirectoris[36];
 public:
    DefineVarCommand(string simDirectoris[]);
    virtual int execute(vector<string> valString) override;
    virtual ~DefineVarCommand() {}
};
class EqualCommand : public CommandInterface {
 public:
  int execute(vector<string> valString) override;
  virtual ~EqualCommand(){}
};
class PrintCommand : public CommandInterface {
public:
    int execute(vector<string> valString) override;
    virtual ~PrintCommand() {}
};

class SleepCommand : public CommandInterface{
public:
    //SleepCommand () {}
    virtual int execute(vector<string> valString) override;
    virtual ~SleepCommand() {}
};
/*
class ConditionParser : public CommandInterface{
    bool condition{};
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
    bool condition{};
public:
    IfCommand () {}
    virtual int execute(vector<string> valString);
    virtual ~IfCommand() {}
};
*/
#endif //EX33_COMMANPATTERN_H
