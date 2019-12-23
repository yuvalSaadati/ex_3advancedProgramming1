#include "ObjectData.h"
ObjectData::ObjectData(int inOut, string sim, float value){
  this->inOut = inOut;
  this->sim = sim;
  this->value = value;
}
string ObjectData::getSim() {
  return this->sim;
}
void ObjectData::setValue(float val) {
  this->value = val;
}