#ifndef EX33_COMMANDINTERFACE_H
#define EX33_COMMANDINTERFACE_H
#include <vector>
#include <map>
#include "Expression.h"
using namespace std;

/*
 *  "CommandInterface" is a interface that has default constractur and has one function which called execute.
 *  by this function we get the information of evere object that will be create from type CommandInterface.
 */
class CommandInterface {
public:
    virtual int execute(vector<string> valString) = 0;
};
#endif //EX33_COMMANDINTERFACE_H