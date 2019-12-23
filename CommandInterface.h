#ifndef EX33_COMMANDINTERFACE_H
#define EX33_COMMANDINTERFACE_H
#include <vector>
#include <map>
#include "Expression.h"
using namespace std;

class CommandInterface {

public:
    virtual int execute(vector<string> valString) = 0;
 protected:
  float simValues[36];
  char *orderForSim;
};
#endif //EX33_COMMANDINTERFACE_H
