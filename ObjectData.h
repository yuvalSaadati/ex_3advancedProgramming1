#ifndef EX33__OBJECTDATA_H_
#define EX33__OBJECTDATA_H_
#include <iostream>
#include <string>
using namespace std;
/*
 * the class contain all the information that is needed for variables
 */
class ObjectData {
public:
    ObjectData(int inOut1, string sim1, float value1);
    int inOut; // in(->)=1 out(<-)=0
    string sim; // the sim of the variable
    float value;// the value of the variable
    float getValue();
    ~ObjectData();
};
#endif //EX33__OBJECTDATA_H_