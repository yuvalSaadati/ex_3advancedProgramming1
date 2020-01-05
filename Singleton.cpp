#include "Singleton.h"
/* Null, because instance will be initialized on demand. */
Singleton* Singleton::instance = 0;
/*
 * return Singleton object
 */
Singleton* Singleton::getInstance()
{
    if (instance == 0)
    {
        instance = new Singleton();
    }
    return instance;
}
/*
 * set in commandMAp all the command objects that can by in fly.txt
 */
Singleton::Singleton()
{
    //creating map which the key is string and the value is command
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
    CommandInterface *loopCommand = new LoopCommand();
    commandMAp["while"] = loopCommand;
    CommandInterface *ifCommand = new IfCommand();
    commandMAp["if"] = ifCommand;
}
/*
 *
 */
vector<string> Singleton::lexer(string s){
    // vector of strings
    vector<string> lexerVector;
    // open the file "fly.txt" which save in file ex33
    std::ifstream flyText;
    flyText.open(s);
    if(flyText) {
        string myLine;
        // reade the file line by line
        while (getline(flyText, myLine)) {
            string s1 = myLine.c_str();
            string val="";
            int lineLength = s1.length();
            char* lineCharArray = new char[lineLength + 1];
            strcpy(lineCharArray, s1.c_str());
            char * token;
            // split the line char array by ( ) and space
            token = strtok (lineCharArray,"( ),\"\t");
            int i = 0;
            string firstWord = myLine.substr(0, myLine.find(" "));
            size_t pos = 0;
            firstWord = myLine.substr(0, myLine.find("("));
            if(firstWord.compare("openDataServer") == 0){
                lexerVector.push_back("openDataServer");
                myLine.erase(0, 14);
                myLine.erase(remove(myLine.begin(), myLine.end(), '('), myLine.end());
                myLine.erase(remove(myLine.begin(), myLine.end(), ')'), myLine.end());
                myLine.erase(remove(myLine.begin(), myLine.end(), ' '), myLine.end());
                lexerVector.push_back(myLine);
                token = NULL;
                continue;
            }
            string tokenConnectControlClient;
            if (firstWord.compare("connectControlClient") == 0) {
                lexerVector.push_back("connectControlClient");
                myLine.erase(0, 20);
                myLine.erase(remove(myLine.begin(), myLine.end(), '('), myLine.end());
                myLine.erase(remove(myLine.begin(), myLine.end(), ')'), myLine.end());
                myLine.erase(remove(myLine.begin(), myLine.end(), ' '), myLine.end());
                if ((pos = myLine.find(",")) != string::npos) {
                    tokenConnectControlClient = myLine.substr(0, pos);
                    tokenConnectControlClient.erase(remove(tokenConnectControlClient.begin(), tokenConnectControlClient.end(), '\"'), tokenConnectControlClient.end());
                    lexerVector.push_back(tokenConnectControlClient);
                    myLine.erase(0, pos + 1);
                    lexerVector.push_back(myLine);
                }
                token = NULL;
                continue;
            }
            if (firstWord.compare("Sleep") == 0) {
                lexerVector.push_back("Sleep");
                myLine.erase(0, 6);
                myLine.erase(myLine.length()-1, myLine.length());
                lexerVector.push_back(myLine);
                token = NULL;
                continue;
            }
            string tokenEqual;
            firstWord = myLine.substr(0, myLine.find(" "));
            if(firstWord.compare("while") != 0 && firstWord.compare("if") != 0) {

                if ((pos = myLine.find("=")) != string::npos) {
                    myLine.erase(std::remove(myLine.begin(), myLine.end(), '\t'), myLine.end());
                    if (firstWord.compare("var") == 0) {
                        // var ho = head
                        lexerVector.push_back("var");
                        string valName = myLine.substr(0, pos);
                        valName.erase(0, 4);
                        valName.erase(std::remove(valName.begin(), valName.end(), '='), valName.end());
                        valName.erase(std::remove(valName.begin(), valName.end(), ' '), valName.end());
                        lexerVector.push_back(valName);
                        lexerVector.push_back("=");
                        myLine.erase(0, pos + 1);
                        myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
                        lexerVector.push_back(myLine);
                    } else {
                        //wrap = 3400
                        string valName = myLine.substr(0, pos);
                        valName.erase(std::remove(valName.begin(), valName.end(), '='), valName.end());
                        valName.erase(std::remove(valName.begin(), valName.end(), ' '), valName.end());
                        lexerVector.push_back(valName);
                        lexerVector.push_back("=");
                        myLine.erase(0, pos + 1);
                        myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
                        lexerVector.push_back(myLine);
                    }
                    myLine.erase();
                    token = NULL;
                    continue;
                }
            }
            while (token != NULL){
                i+=strlen(token);
                if(strcmp(token, "Print")==0){
                    lexerVector.push_back(token);
                    // myLine=myLine.substr(myLine.find_first_of("(")+1);
                    // create the line print withput"()
                    myLine.erase(std::remove(myLine.begin(), myLine.end(), '\t'), myLine.end());
                    myLine.erase(0, 6);
                    myLine.erase(std::remove(myLine.begin(), myLine.end(), ')'), myLine.end());
                    lexerVector.push_back(myLine);
                    token = NULL;
                }else if(strcmp(token, "=")==0){
                    lexerVector.push_back(token);
                    token = strtok(NULL, "\"");
                    while (token != NULL)
                    {
                        string str(token);
                        val+=str;
                        token = strtok(NULL, "() ,\"");
                    }
                    // myLine.erase(0, i+2);
                    lexerVector.push_back(val);
                }else {
                    // insert the token into the lexer (vector)
                    lexerVector.push_back(token);
                    // the next token in the line
                    token = strtok(NULL, "() ,\"\t");
                }
            }
            delete[] lineCharArray;
        }
        flyText.close();
    }
    else {
        cout << "Error while openning the file" << endl;
        exit(1);
    }
    return lexerVector;
}

void Singleton:: parser(vector<string> lexerVector) {
    for (int i = 0; i < lexerVector.size(); i++) {
        map<string,CommandInterface*>::iterator it = this->commandMAp.find(lexerVector[i]);
        if (it != this->commandMAp.end()) {
            vector<string> valString;
            if(lexerVector[i].compare("while") == 0) {
                while(lexerVector[i].compare("}") != 0) {
                    valString.push_back(lexerVector[i]);
                    i++;
                }
            } else if(lexerVector[i].compare("if") == 0){
                while(lexerVector[i].compare("}") != 0) {
                    valString.push_back(lexerVector[i]);
                    i++;
                }
            } else if(lexerVector[i].compare("openDataServer")==0) {
                valString.push_back(lexerVector[i+1]);
            }
            else if(lexerVector[i].compare("connectControlClient")==0) {
                valString.push_back(lexerVector[i+1]);
                valString.push_back(lexerVector[i+2]);
            }
            else if(lexerVector[i].compare("var")==0) {
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
                valString.push_back(lexerVector[i-1]);
                valString.push_back(lexerVector[i+1]);
            }
            int skipCount = (it->second)->execute(valString);
            i+=skipCount;
        }
    }
}
/*
 * update the valuesFromSim map with data from the xml file. every key in the map represent differnt variable
 */
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
 * the function get string and return the result of this expression
 */
float Singleton::calculateExpression(string s){
    Interpreter* interpreter = new Interpreter();
    Expression* e = nullptr;
    string stringExpression = s;
    char *arrayExpression = new char[stringExpression.length()+1];
    strcpy(arrayExpression, stringExpression.c_str());
    // making set to all var which is in symboltable
    bool createVal = false;
    bool startWord  = true;
    string variableName = "";
    // set all the variable
    for(int i=0; i<stringExpression.length();i++){
        if(isalpha(arrayExpression[i])&&startWord) {
            // adding letters into one word which is a name of var
            variableName += arrayExpression[i];
            createVal = true;
            startWord = false;
            continue;
        }
        // the name of variable must start with letter and contain only letters and numbers
        while ((isalpha(arrayExpression[i]) || isdigit(arrayExpression[i]) || arrayExpression[i] == '_')&&createVal) {
            variableName += arrayExpression[i];
            i++;
        }
        if(createVal) {
            // the word ended, so we will find the value of val
            variableName += "=" + to_string(Singleton().getInstance()->symbolTable.find(variableName)->second->getValue());
            // insert each val into the setVariables
            interpreter->setVariables(variableName);
            // clean the string in order to insert her new var
            createVal=false;
            startWord  = true;
            variableName.erase();
        }
    }
    try{
        e = interpreter->interpret(s);
        // calculate the interpreter by the s tat has given to the function
        float f= e->calculate();
        delete e;
        delete interpreter;
        delete[] arrayExpression;
        return f;
    } catch (const char* e1) {
        if (e1 != nullptr) {
            delete e1;
        }
        if (interpreter != nullptr) {
            delete interpreter;
        }
    }
}