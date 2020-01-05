#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex>
#include "CommandInterface.h"
#include "Singleton.h"
#include "Expression.h"
#include "Calculate.h"
#include <mutex>
using namespace std;
#ifndef EX33_COMMANPATTERN_H
#define EX33_COMMANPATTERN_H
static bool is_done = false;
class commandPattern {};
/*
 * class that open server which will conact to the simulator
 */
class OpenServerCommand : public CommandInterface{
   int client_socket;
   int port;
   string leftVals = "";
public:
    bool isFloatnumber(char *token);
    int openServer();
    void simDataParser(char buffer[]);
    virtual int execute(vector<string> valString);
        int readFromSim();
    virtual ~OpenServerCommand(){}
};
/*
 * class that connect the client to server
 */
class ConnectCommand : public CommandInterface{
  int client_socket;
  string ip;
  int port;
public:
    int openClient();
    int execute(vector<string> valString) override;
    virtual ~ConnectCommand(){}
};
/*
 * the class create new variables and put them in symbol table
 */
class DefineVarCommand : public CommandInterface{
    string name;
    string simDirectoris[36];
 public:
    DefineVarCommand(string simDirectoris[]);
    virtual int execute(vector<string> valString) override;
    virtual ~DefineVarCommand(){}
};
/*
 * the class handle with variable that will be update with diffrent value
 */
class EqualCommand : public CommandInterface {
 public:
  int execute(vector<string> valString) override;
  virtual ~EqualCommand(){}
};
/*
 * print to the screen a string or expression
 */
class PrintCommand : public CommandInterface {
public:
    int execute(vector<string> valString) override;
    virtual ~PrintCommand(){}
};
/*
 * the thread that run this function will sleep
 */
class SleepCommand : public CommandInterface{
public:
    virtual int execute(vector<string> valString) override;
    virtual ~SleepCommand(){}
};
/*
 * run while loop
 */
class LoopCommand : public CommandInterface{
    float rpm_to_check = 0;
    bool condition;
public:
    vector<CommandInterface> comamandVector;
    vector<vector<string>> vectorStringCommand;
    virtual int execute(vector<string> valString);
    bool isnumber(string s);
    virtual ~LoopCommand(){}
};
/*
 * run if condition
 */
class IfCommand : public CommandInterface{
    bool condition{};
public:
    virtual int execute(vector<string> valString);
    virtual ~IfCommand(){}
};

#endif //EX33_COMMANPATTERN_H