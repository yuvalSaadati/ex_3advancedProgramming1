#include <string>
#include <vector>
#include "commandPattern.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
std::mutex mutex_lock;
int OpenServerCommand::execute(vector<string> valString){
    port = stoi(valString[0]);
    thread ts(&OpenServerCommand::openServer,this);
    ts.detach();
    while(!is_done)
    {
        this_thread::sleep_for(chrono::microseconds(100));
    }
    return 1;
}

int OpenServerCommand::openServer(){
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr<<"Could not bind the socket to an IP"<<std::endl;
        return -2;
    }
    if (listen(socketfd, 5) == -1) {
        std::cerr<<"Error during listening command"<<std::endl;
        return -3;
    } else{
        std::cout<<"Server is now listening ..."<<std::endl;
    }
    client_socket = accept(socketfd, (struct sockaddr *)&address, (socklen_t*)&address);
    if (client_socket == -1) {
        std::cerr<<"Error accepting client"<<std::endl;
        return -4;
    }
    close(socketfd);
    is_done = true;
    char buffer[1024] = {0};
    while(true){
        int valread = read( client_socket , buffer, 1024);
        cout<<buffer<<std::endl;
        simDataParser(buffer);
        char *hello = "Hello, I can hear you! \n";
        send(client_socket , hello , strlen(hello) , 0);
      //  std::cout<<"Hello message sent\n"<<std::endl;
    }
}
int ConnectCommand::openClient() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr((ip).c_str());
    address.sin_port = htons(port);
    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server"<<std::endl;
        return -2;
    } else {
        std::cout<<"Client is now connected to server" <<std::endl;
    }
    while(true){
        if (!Singleton().getInstance()->messages_from_client.empty()) {
            mutex_lock.lock();
            int is_sent = send(client_socket,
                               Singleton().getInstance()->messages_from_client.front().c_str(),
                               strlen(Singleton().getInstance()->messages_from_client.front().c_str()),
                               0);
            Singleton().getInstance()->messages_from_client.pop();
            mutex_lock.unlock();
            if (is_sent == -1) {
                std::cout<<"Error sending message"<<std::endl;
            } else {
            //    std::cout<<Singleton().getInstance()->messages_from_client.front()<<std::endl;
           //     std::cout<<"Hello message sent to server" <<std::endl;
            }
        }
        else {
            this_thread::sleep_for(chrono::microseconds(10));
        }
    }
}
void OpenServerCommand::simDataParser(char * buffer) {
    int i = 0;
    char * token;
    token = strtok (buffer,",");
    while(token != NULL)
    {
        this->simValues[i] = stof(token);
        for(auto it =  Singleton().getInstance()->symbolTable.cbegin(); it !=  Singleton().getInstance()->symbolTable.cend(); ++it)
        {
            if(it->second->getSim().compare(Singleton().getInstance()->valuesFromSim[i]) == 0 && it->second->getInOut() == 0){
                it->second->setValue(stof(token));
            }
        }
        token = strtok (NULL, ",");
        i++;
    }
}
int ConnectCommand::execute(vector<string> valString){
    //is_done = false;

    ip = valString[0];
    port = stoi(valString[1]);
    thread tc(&ConnectCommand::openClient,this);
    tc.detach();
    /*while(!is_done)
    {
      this_thread::sleep_for(chrono::microseconds(100));
    }*/
    return 2;
}

DefineVarCommand::DefineVarCommand(string simDirectories[]){
    for(int i= 0; i < 36; i++){
        this->simDirectoris[i] = simDirectories[i];
    }
}
int DefineVarCommand::execute(vector<string> valString){
    if(Singleton().getInstance()->symbolTable.find(valString[2]) == Singleton().getInstance()->symbolTable.end()){
        if(valString[1].compare("->")==0){
            ObjectData *objectData = new ObjectData(1, valString[2],0);
            Singleton().getInstance()->symbolTable[valString[0]] = objectData;
            return 4;
        }
        else if(valString[1].compare("<-")==0){
            //mutex_lock.lock();
            ObjectData *objectData = new ObjectData(0, valString[2],0);

            for(int i = 0; i < 36; i++){
                if(Singleton().getInstance()->valuesFromSim[i].compare(objectData->getSim())==0){
                    objectData->setValue(this->simValues[i]);
                 //  cout<<objectData->sim << " = " << objectData->value << endl;
                    break;
                }
            }
            Singleton().getInstance()->symbolTable[valString[0]] = objectData;
            //mutex_lock.unlock();
            return 4;
        }
    } else{
        Singleton().getInstance()->symbolTable[valString[0]] = Singleton().getInstance()->symbolTable.find(valString[2])->second;
    }
    //Expression *e = new Variable(objectData.name, objectData.value);//value is float, needs double
    //this->symbolTable.insert(pair<Expression*,VariableData>(e,objectData));
    return 3;
}
int EqualCommand::execute(vector<string> valString) {
    if (regex_match(valString[1], regex("^[-+]?[0-9]+\\.?[0-9]*$"))) {//autostart = 1
        mutex_lock.lock();
        Singleton().getInstance()->symbolTable.find(valString[0])->second->setValue(stof(valString[1]));
        string str = "set " + Singleton().getInstance()->symbolTable.find(valString[0])->second->sim + " " + valString[1] + "\r\n";
        Singleton().getInstance()->messages_from_client.push(str);
        mutex_lock.unlock();
    } else {//autostart = 1+1
        float f = Singleton().getInstance()->calculateExpression(valString[1]);
        Singleton().getInstance()->symbolTable.find(valString[0])->second->setValue(f);
    }
    return 1;
}
int PrintCommand::execute(vector<string> valString){
    string outPutString = valString[0];
    string firstWord = outPutString.substr(0, outPutString.find(" "));
    map<string,ObjectData*>::iterator it= Singleton().getInstance()->symbolTable.find(firstWord);
    if (it != Singleton().getInstance()->symbolTable.end()) {
        float f = Singleton().getInstance()->calculateExpression(outPutString);
        cout << f << endl;
    } else {
        cout << outPutString << endl;
    }
    return 1;
}
int SleepCommand::execute(vector<string> valString){
    int sleep = stoi(valString[0]);
    this_thread::sleep_for(chrono::milliseconds(sleep));
    return 1;
}

int ConditionParser::execute(vector<string> valString){
    return 0;
}

int LoopCommand::execute(vector<string> valString){
    //  for (auto i = valString.begin(); i != valString.end(); ++i)
    //   cout << *i << " ";
    // creating leftWhile, operatorWhile, rightWhile all in each string
    string leftWhile="";
    string rightWhile="";
    string operatorWhile="";
    bool finish = false;
    int k, i, j;
    for (k = 0; k < valString.size(); k++) {

        if(!finish) {
            if (valString[k].compare("<") == 0) {
                operatorWhile = "<";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare(">") == 0) {
                operatorWhile = ">";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("<=") == 0) {
                operatorWhile = "<=";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare(">=") == 0) {
                operatorWhile = ">=";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("==") == 0) {
                operatorWhile = "==";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("!=") == 0) {
                operatorWhile = "!=";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            }
        }
    }
    float valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
    float valRight = Singleton().getInstance()->calculateExpression(rightWhile);
    // creating valString without the line of while
    vector<string> copy = valString;
    int sum = i+j;
    while(sum>0){
        copy.erase(copy.begin());
        sum--;
    }
    if(operatorWhile.compare("<")==0) {
        while (valLeft < valRight) {
            //mutex_lock.lock();
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
            // mutex_lock.unlock();
        }
    } else if(operatorWhile.compare(">")==0) {
        while (valLeft > valRight) {
         //   mutex_lock.lock();
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
          //  mutex_lock.unlock();
        }
    } else if(operatorWhile.compare("<=")==0) {
        while (valLeft <= valRight) {
         //   mutex_lock.lock();
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
         //   mutex_lock.unlock();
        }
    } else if (operatorWhile.compare(">=")==0) {
        while (valLeft >= valRight) {
          //  mutex_lock.lock();
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
           // mutex_lock.unlock();
        }
    } else if (operatorWhile.compare("==")==0) {
        while (valLeft == valRight) {
           // mutex_lock.lock();
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
            mutex_lock.unlock();}
    } else if(operatorWhile.compare("!=")==0) {
        while (valLeft != valRight) {
            mutex_lock.lock();
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
         //   mutex_lock.unlock();
        }
    }
    return valString.size();
}

int IfCommand::execute(vector<string> valString){
    return 0;
}

// the methos check if the string is a number
bool  LoopCommand :: isnumber(string s) {
    int stringLength = s.length();
    char* sArray = new char[stringLength + 1];
    strcpy(sArray, s.c_str());
    bool flag = false;
    if (s.length() == 1 && isdigit(s.at(0))) {
        return true;
    }
    for (unsigned i=1; i<s.length();i++)
    {
        if (isdigit(s[i])) {
            flag = true;
        } else if ((s.length() > 2 && s[i]=='.' && isdigit(s[i-1]) && isdigit(s[i+1]))) {
            flag = true;
        } else {
            return false;
        }
    }
    return flag;
}