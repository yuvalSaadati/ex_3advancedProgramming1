#include "ObjectData.h"
/*
 * constracture that get all the information of the variable
 */
ObjectData::ObjectData(int inOut1, string sim1, float value1){
    this->inOut = inOut1;
    this->sim = sim1;
    this->value = value1;
}
/*
 * return the value of the variable
 */
float ObjectData::getValue() {
    return this->value;
}
