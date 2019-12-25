#include "Singleton.h"
/* Null, because instance will be initialized on demand. */
Singleton* Singleton::instance = 0;
Singleton* Singleton::getInstance()
{
    if (instance == 0)
    {
        instance = new Singleton();
    }
    return instance;
}

Singleton::Singleton()
{
    //creating map with string and command
    CommandInterface *openDataServer = new OpenServerCommand();
    commandMAp["openDataServer"] = openDataServer;
    CommandInterface *connectCommand = new ConnectCommand();
    commandMAp["connectControlClient"] = connectCommand;
    CommandInterface *defineVarCommand = new DefineVarCommand(this->valuesFromSim);
    commandMAp["var"] = defineVarCommand;
    CommandInterface *printCommand = new PrintCommand();
    commandMAp["Print"] = printCommand;
    CommandInterface *sleepCommand = new SleepCommand();
    commandMAp["Sleep"] = sleepCommand;
    CommandInterface *equalCommand = new EqualCommand();
    commandMAp["="] = equalCommand;
}

 vector<string> Singleton::lexer(string s){
     // vector of strings
     vector<string> lexerVector;
     // open the file "fly_with_func.txt" which save in file ex33
     std::ifstream flyText;
     flyText.open(s);
     if(flyText) {
         string myLine;
         // reade the file line by line
         while (getline(flyText, myLine)) {

             string s = myLine.c_str();
             int lineLength = s.length();
             char lineCharArray[lineLength + 1];
             strcpy(lineCharArray, s.c_str());
             char * token;
             // split the line char array by ( ) and space
             token = strtok (lineCharArray,"( ),\"");
             while (token != NULL)
             {
                if(string(token).compare("Print") == 0){
                  lexerVector.push_back(token);
                  token = strtok (NULL, "\")");
                }
                // insert the token into the lexer (vector)
                lexerVector.push_back(token);
                // the next token in the line.c_str()
                token = strtok (NULL, "() ,\"");
             }
         }
         flyText.close();
     }
     else {
         cout << "Error while opening the file" << endl;
         exit(1);
     }
     return lexerVector;
}

void Singleton:: parser(vector<string> lexerVector) {
  int j = 1;
    for (int i = 0; i < lexerVector.size(); i++) {
        map<string,CommandInterface*>::iterator it= this->commandMAp.find(lexerVector[i]);
      if (it != this->commandMAp.end()) {
            vector<string> valString;
            if(lexerVector[i].compare("openDataServer")==0) {
              valString.push_back(lexerVector[i+1]);
            }
            else if(lexerVector[i].compare("connectControlClient")==0) {
                valString.push_back(lexerVector[i+1]);
                valString.push_back(lexerVector[i+2]);
            }
            else if(lexerVector[i].compare("var")==0) {
                j++;
                valString.push_back(lexerVector[i+1]);
                valString.push_back(lexerVector[i+2]);
                if(valString[valString.size() - 1] == "=")
                {
                  valString.push_back(lexerVector[i+3]);
                } else{
                  valString.push_back(lexerVector[i+4].erase(0,1));
                }
            }
            else if(lexerVector[i].compare("Print")==0) {
                valString.push_back(lexerVector[i+1]);
            }
            else if(lexerVector[i].compare("Sleep")==0) {
                valString.push_back(lexerVector[i+1]);
            }
            else if(lexerVector[i].compare("=") == 0) {
              j++;
              valString.push_back(lexerVector[i-1]);
              valString.push_back(lexerVector[i+1]);
            }
            // make execute of the command
            int skipCount = (it->second)->execute(valString);
            i+=skipCount;
        }
    }
}
void Singleton:: createSimValuesMap(){
  this->valuesFromSim[0] = "instrumentation/airspeed-indicator/indicated-speed-kt";
  this->valuesFromSim[1] = "sim/time/warp";
  this->valuesFromSim[2] = "controls/switches/magnetos";
  this->valuesFromSim[3] = "instrumentation/heading-indicator/offset-deg";
  this->valuesFromSim[4] = "instrumentation/altimeter/indicated-altitude-ft";
  this->valuesFromSim[5] = "instrumentation/altimeter/pressure-alt-ft";
  this->valuesFromSim[6] = "instrumentation/attitude-indicator/indicated-pitch-deg";
  this->valuesFromSim[7] = "instrumentation/attitude-indicator/indicated-roll-deg";
  this->valuesFromSim[8] = "instrumentation/attitude-indicator/internal-pitch-deg";
  this->valuesFromSim[9] = "instrumentation/attitude-indicator/internal-roll-deg";
  this->valuesFromSim[10] = "instrumentation/encoder/indicated-altitude-ft";
  this->valuesFromSim[11] = "instrumentation/encoder/pressure-alt-ft";
  this->valuesFromSim[12] = "instrumentation/gps/indicated-altitude-ft";
  this->valuesFromSim[13] = "instrumentation/gps/indicated-ground-speed-kt";
  this->valuesFromSim[14] = "instrumentation/gps/indicated-vertical-speed";
  this->valuesFromSim[15] = "instrumentation/heading-indicator/indicated-heading-deg";
  this->valuesFromSim[16] = "instrumentation/magnetic-compass/indicated-heading-deg";
  this->valuesFromSim[17] = "instrumentation/slip-skid-ball/indicated-slip-skid";
  this->valuesFromSim[18] = "instrumentation/turn-indicator/indicated-turn-rate";
  this->valuesFromSim[19] = "instrumentation/vertical-speed-indicator/indicated-speed-fpm";
  this->valuesFromSim[20] = "controls/flight/aileron";
  this->valuesFromSim[21] = "controls/flight/elevator";
  this->valuesFromSim[22] = "controls/flight/rudder";
  this->valuesFromSim[23] = "controls/flight/flaps";
  this->valuesFromSim[24] = "controls/engines/engine/throttle";
  this->valuesFromSim[25] = "controls/engines/current-engine/throttle";
  this->valuesFromSim[26] = "controls/switches/master-avionics";
  this->valuesFromSim[27] = "controls/switches/starter";
  this->valuesFromSim[28] = "engines/active-engine/auto-start";
  this->valuesFromSim[29] = "controls/flight/speedbrake";
  this->valuesFromSim[30] = "sim/model/c172p/brake-parking";
  this->valuesFromSim[31] = "controls/engines/engine/primer";
  this->valuesFromSim[32] = "controls/engines/current-engine/mixture";
  this->valuesFromSim[33] = "controls/switches/master-bat";
  this->valuesFromSim[34] = "controls/switches/master-alt";
  this->valuesFromSim[35] = "engines/engine/rpm";
}
/*
float Singleton::calculateExpression(string s) {

  Interpreter *interpreter = new Interpreter();
  Expression *e = nullptr;
  string stringExpression = s;
  char arrayExpression[stringExpression.length() + 1];
  strcpy(arrayExpression, stringExpression.c_str());
  // making set to all var which is in symboltable
  bool createVal = false;
  bool startWord = true;
  string variableName = "";
  for (int i = 0; i < stringExpression.length(); i++) {
    if (isalpha(arrayExpression[i]) && startWord) {
      // adding letters into one word which is a name of var
      variableName += arrayExpression[i];
      createVal = true;
      startWord = false;
      continue;
    }
    while ((isalpha(arrayExpression[i]) || isdigit(arrayExpression[i]) || arrayExpression[i] == '_') && createVal) {
      variableName += arrayExpression[i];
      i++;
    }
    if (createVal) {
      // the word ended, so we will find the value of val
      variableName += "=" + to_string(Singleton().getInstance()->symbolTable.find(variableName)->second->getValue());
      // insert each val into the setVaribles
      interpreter->setVariables(variableName);
      // clean the string in order to insert her new var
      createVal = false;
      startWord = true;
      variableName.erase();
    }
  }
  try {
    e = interpreter->interpret(s);
    float f = e->calculate();
    delete e;
    delete interpreter;
    return f;
  } catch (const char *e) {
    if (e != nullptr) {
      delete e;
    }
    if (interpreter != nullptr) {
      delete interpreter;
    }
  }
}
*/