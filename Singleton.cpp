//
// Created by yuval on 18/12/2019.
//

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
    CommandInterface *defineVarCommand = new DefineVarCommand();
    commandMAp["var"] = defineVarCommand;
    CommandInterface *printCommand = new PrintCommand();
    commandMAp["Print"] = printCommand;
    CommandInterface *sleepCommand = new SleepCommand();
    commandMAp["Sleep"] = sleepCommand;
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
                 // insert the token into the lexer (vector)
                 lexerVector.push_back(token);
                 // the next token in the line
                 token = strtok (NULL, "() ,\"");
             }
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
    // parser
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
                valString.push_back(lexerVector[i+1]);
                valString.push_back(lexerVector[i+2]);
                valString.push_back(lexerVector[i+4]);
            }
            else if(lexerVector[i].compare("Print")==0) {
                valString.push_back(lexerVector[i+1]);
            }
            else if(lexerVector[i].compare("Sleep")==0) {
                valString.push_back(lexerVector[i+1]);
            }
            // make execute on the command
                int skipCount = (it->second)->execute(valString);

            /*if(lexerVector[i].compare("var")==0) {
                struct VaribableData objectDAta;
                objectDAta.sim =lexerVector[i+4];
                if(lexerVector[i+2].compare("->")){
                    objectDAta.inOut = 1;
                } else {
                    objectDAta.inOut = 0;
                }
                this->symbolTable[lexerVector[i+1]] = objectDAta;
            }*/
            i+=skipCount;
        }
    }
}
