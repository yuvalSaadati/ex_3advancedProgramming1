#ifndef EX33__OBJECTDATA_H_
#define EX33__OBJECTDATA_H_
#include <iostream>
#include <string>
using namespace std;
class ObjectData {
public:
    ObjectData(int inOut, string sim, float value);
    int inOut; // in(->)=1 out(<-)=0
    string sim;
    float value;
    string getSim();
    void setValue(float val);
    float getValue();
    int getInOut();
    ~ObjectData();
};
#endif //EX33__OBJECTDATA_H_
