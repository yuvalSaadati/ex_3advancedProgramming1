//
// Created by yuval on 18/12/2019.
//
#include <string>
#include <vector>
#include "commandPattern.h"
#include <iostream>
#include <string>
int OpenServerCommand::serverThread(int port){
    //create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }
    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(port);
    //we need to convert our number
    // to a number that the network understands.

    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr<<"Could not bind the socket to an IP"<<std::endl;
        return -2;
    }
    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr<<"Error during listening command"<<std::endl;
        return -3;
    } else{
        std::cout<<"Server is now listening ..."<<std::endl;
    }
    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *)&address, (socklen_t*)&address);
    if (client_socket == -1) {
        std::cerr<<"Error accepting client"<<std::endl;
        return -4;
    }
    close(socketfd); //closing the listening socket
    //reading from client
    char buffer[1024] = {0};
    //while reade
    while(read( client_socket , buffer, 1024))
    {
        std::cout<<buffer<<std::endl;
    }
    //int valread = read( client_socket , buffer, 1024);
    //std::cout<<buffer<<std::endl;
    //writing back to client
    char *hello = "Hello, I can hear you! \n";
    //send(client_socket , hello , strlen(hello) , 0 );
    //std::cout<<"Hello message sent\n"<<std::endl;
    return 0;
}

int OpenServerCommand::execute(vector<string> valString){
    int port = stoi(valString[0]);
    thread tr(serverThread, port);
    tr.detach();
    while(!is_done)
    {
        this_thread::sleep_for(chrono::microseconds(100));
     //   is_done = true;
    }
    return 1;
}
int ConnectCommand::clientThread(string ip, int port) {
    is_done = false;
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }
    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr((ip).c_str());  //the localhost address
    address.sin_port = htons(port);
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.
    // Requesting a connection with the server on local host with port 8081
    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server"<<std::endl;
        return -2;
    } else {
        std::cout<<"Client is now connected to server" <<std::endl;
    }
    //if here we made a connection
    char hello[] = "Hi from client";
    int is_sent = send(client_socket , hello , strlen(hello) , 0 );
    if (is_sent == -1) {
        std::cout<<"Error sending message"<<std::endl;
    } else {
        std::cout<<"Hello message sent to server" <<std::endl;
    }
    char buffer[1024] = {0};
    int valread = read( client_socket , buffer, 1024);
    std::cout<<buffer<<std::endl;
    close(client_socket);
    return 0;
}
int ConnectCommand::execute(vector<string> valString){
    string ip = valString[0];
    int port = stoi(valString[1]);
    thread tr(clientThread, ip, port);
    tr.detach();
    while(!is_done)
    {
        this_thread::sleep_for(chrono::microseconds(100));
      //  is_done = true;
    }
    return 2;
}

int DefineVarCommand::execute(vector<string> valString){
    //to add the xml map later
    struct VaribableData objectDAta;
    objectDAta.sim =valString[3];
    if(valString[1].compare("->")){
        objectDAta.inOut = 1;
    } else {
        objectDAta.inOut = 0;
    }
    this->symbolTable[valString[0]] = objectDAta;
    return 4;
}

int PrintCommand::execute(vector<string> valString){
    this->outPutString = valString[0];
    cout << valString[0] << endl;
    return 1;
}

int SleepCommand::execute(vector<string> valString){
    this->sleep = stoi(valString[0]);
    this_thread::sleep_for(chrono::milliseconds(this->sleep));
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