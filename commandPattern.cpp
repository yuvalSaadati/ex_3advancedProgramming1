#include <string>
#include <vector>
#include "commandPattern.h"
#include <iostream>
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
      std::cout<<"Hello message sent\n"<<std::endl;
    }
}
void OpenServerCommand::simDataParser(char * buffer) {
  int i = 0;
  char * token;
  token = strtok (buffer,",");
  while(token != NULL)
  {
    this->simValues[i] = stof(token);
    token = strtok (NULL, ",");
    i++;
  }
}
int ConnectCommand::execute(vector<string> valString){
  ip = valString[0];
  port = stoi(valString[1]);
  thread tc(&ConnectCommand::openClient,this);
  tc.detach();
  return 2;
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
                           Singleton().getInstance()->messages_from_client.size(),
                           0);
        Singleton().getInstance()->messages_from_client.pop();
        mutex_lock.unlock();
        if (is_sent == -1) {
          std::cout<<"Error sending message"<<std::endl;
        } else {
          std::cout<<"Hello message sent to server" <<std::endl;
        }
      }
      else {
        this_thread::sleep_for(chrono::microseconds(10));
      }
    }
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
      ObjectData *objectData = new ObjectData(0, valString[2],0);
      for(int i = 0, flag = 0; i < 36 && flag == 0; i++){
        if(this->simDirectoris[i] == objectData->getSim()){
          objectData->setValue(this->simValues[i]);
          flag = 1;
          cout<<objectData->sim << " = " << objectData->value << endl;
        }
      }
      Singleton().getInstance()->symbolTable[valString[0]] = objectData;
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
    Singleton().getInstance()->symbolTable[valString[0]]->value = stof(valString[1]);
    string str = "set " + Singleton().getInstance()->symbolTable[valString[0]]->sim + " " + valString[1] + "\r\n";
    Singleton().getInstance()->messages_from_client.push(str);
    mutex_lock.unlock();
  }
    /*int flag = 0;
    map<Expression*,VariableData>::iterator it = this->symbolTable.begin();
    while(it != this->symbolTable.end() && flag == 0){
      if (valString[2] == it->second.name){
        flag = 1;
        Expression *e = new Variable(valString[2], it->first->calculate());
        struct VariableData objectData = it->second;
        objectData.name = valString[2];
        this->symbolTable.insert(pair<Expression*,VariableData>(e, objectData));
      }
      else{
        it++;
      }
    }*/
    //int flag = 0;
    //map<Expression*,VariableData>::iterator it = this->symbolTable.begin();
    /*while(it != this->symbolTable.end() && flag == 0){
      if (valString[0] == it->second.name){
        flag = 1;
        it->second.value = stof(valString[1]);
        string str = "set " + it->second.sim + " " + valString[1];
        strcpy(this->orderForSim, str.c_str());
      }
      else{
        it++;
      }
    }
  }*/
    //case 3: rudder = (h0 - heading)/80
    return 1;
}
int PrintCommand::execute(vector<string> valString){
    string outPutString = valString[0];
    cout << outPutString << endl;
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
    return 0;
}

int IfCommand::execute(vector<string> valString){
    return 0;
}