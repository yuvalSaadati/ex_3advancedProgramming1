//
// Created by yuval on 18/12/2019.
//

#ifndef EX33_COMMANDINTERFACE_H
#define EX33_COMMANDINTERFACE_H
#include <vector>

using namespace std;

class CommandInterface {

public:
    virtual int execute(vector<string> valString) =0;
};
#endif //EX33_COMMANDINTERFACE_H
