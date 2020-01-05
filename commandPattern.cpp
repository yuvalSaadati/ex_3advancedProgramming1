#include <string>
#include <vector>
#include "commandPattern.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
// we will use mutex when we will set value to command map
std::mutex mutex_lock;
/*
 * in the execute of OpenServerCommand we open thread to run openServer function
 */
int OpenServerCommand::execute(vector<string> valString){
    // set the port of the server, in the fly txt is 5400
    port  =  Singleton().getInstance()->calculateExpression(valString[0]);
    thread ts(&OpenServerCommand::openServer,this);
    ts.detach();
    // after the server will listen to client the while loop will stop by set the flag is_done too true
    while(!is_done)
    {
        // the main thread will sleep while the thread ts open the server
        this_thread::sleep_for(chrono::microseconds(100));
    }
    return 1;
}
/*
 * In this function we open server to listen to client which is the simulator
 */
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
        int valread = read(client_socket , buffer, 1024);
        // send the buffer which contain values of 36 variables, we got this buffer from the server
        simDataParser(buffer);
        char *hello = "Hello, I can hear you! \n";
        send(client_socket , hello , strlen(hello) , 0);
    }
}
/*
 * buffer contain the 36 values and set them in command map
 */
void OpenServerCommand::simDataParser(char * buffer) {
    int i = 0;
    if(leftVals != "") {//if there's some values left from the previous buffer
        //convert leftVals to char array
        int n = leftVals.length();
        char* char_array = new char[n+1];
        strcpy(char_array, leftVals.c_str());
        //go over the leftVals by demi "," and change accordingly the symbolTable
        char *token = strtok(char_array, ",");
        while (token != NULL) {
            for (auto it = Singleton().getInstance()->symbolTable.cbegin();
                 it != Singleton().getInstance()->symbolTable.cend(); ++it) {
                if (Singleton().getInstance()->valuesFromSim[i] == it->second->sim) {
                    if (it->second->inOut == 0) {
                        mutex_lock.lock();
                        it->second->value = stof(token);
                        mutex_lock.unlock();
                    }
                    cout << it->second->value << ", ";
                }
            }
            i++;
            token = strtok(NULL, ",");
        }
        delete[] char_array;
    }
    int leftValFlag = 0;
    while (*buffer != '\0') {
        while (*buffer != '\n') {
            if (*buffer == ',') {
                buffer++;
            } else {
                string str = string(buffer);
                string value;
                int pos = str.find(",");
                int pos2 = str.find("\n");
                //if you cant find \n pos, then save left values in the leftVals
                if (pos2 == -1) {
                    leftValFlag = 1;
                    value = str;
                    leftVals+=value;
                    buffer += leftVals.size();
                    break;
                } else if (pos != -1 && pos < pos2) {
                    value = str.substr(0, pos);
                } else {
                    pos = pos2;
                    value = str.substr(0, pos);
                }
                int flag = 0;
                //searching the sim in the symbolTable
                for (auto it = Singleton().getInstance()->symbolTable.cbegin();
                     it != Singleton().getInstance()->symbolTable.cend(); ++it) {
                    if (Singleton().getInstance()->valuesFromSim[i] == it->second->sim) {
                        flag = 1; //if it was found
                        if (it->second->inOut == 0) { //if the value is a one given from the simulator
                            mutex_lock.lock();
                            it->second->value = stof(value);
                            mutex_lock.unlock();
                        }
                        cout<<it->second->value<<", ";
                    }
                }
                if (flag == 0) { //if the sim isn't in the symbolTable then add it
                    mutex_lock.lock();
                    Singleton().getInstance()->symbolTable[Singleton().getInstance()->valuesFromSim[i]] =
                            new ObjectData(0, Singleton().getInstance()->valuesFromSim[i], stof(value));
                    mutex_lock.unlock();
                    cout<<value<<", ";
                } else {
                    flag = 0;
                }
                i++;
                buffer += pos;
            }
        }
        if(leftValFlag == 0){ //if we read 36 values and got \n
            buffer++;
            cout<<""<<endl;
            i = 0;
        }
        leftValFlag = 0;
    }
}
/*
 * the function connect the client to server by the port in ConnectCommand filed
 */
int ConnectCommand::openClient() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Could not create a socket" << std::endl;
        return -1;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr((ip).c_str());
    address.sin_port = htons(port);
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server" << std::endl;
        return -2;
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }
    while (true) {
        if (!Singleton().getInstance()->messages_from_client.empty()) {
            // pop string from Singleton().getInstance()->messages_from_client which is queue.
            // every string is set command which the server need to do
            int is_sent = write(client_socket,
                                Singleton().getInstance()->messages_from_client.front().c_str(),
                                strlen(Singleton().getInstance()->messages_from_client.front().c_str()));
            // pop the next set command to server
            Singleton().getInstance()->messages_from_client.pop();
            if (is_sent == -1) {
                std::cout<<"Error sending message"<<std::endl;
            }
        }
        else {
            // the queue is empty so the tread will sleep until some set commad will be insert to the queue
            this_thread::sleep_for(chrono::microseconds(10));
        }
    }
}
/*
 * the execute function of ConnectCommand get 2 strings in valString.
 * valString[0] is the ip of the client and valString[1] is the port.
 * by those values  thread tc will run on openClient function
 */
int ConnectCommand::execute(vector<string> valString){
    ip = valString[0];
    port = Singleton().getInstance()->calculateExpression(valString[1]);
    thread tc(&ConnectCommand::openClient,this);
    tc.detach();
    return 2;
}
/*
 * the constracture of DefineVarCommand set the sim directories of each value
 */
DefineVarCommand::DefineVarCommand(string simDirectories[]){
    for(int i= 0; i < 36; i++){
        this->simDirectoris[i] = simDirectories[i];
    }
}
/*
 * set the values of each variable by the information which given in the flt.txt
 */
int DefineVarCommand::execute(vector<string> valString){
    // in this case the variable valString[2] already has been set in
    // the symbolTable so we just set the valString[0] to be the same like valString[2]
    // for example: var h0 = heading
    if(valString[1].compare("=")==0){
        mutex_lock.lock();
        // we change the value of the variable in the symbolTable so we will use mutex_lock
        Singleton().getInstance()->symbolTable[valString[0]] = Singleton().getInstance()->symbolTable.find(valString[2])->second;
        mutex_lock.unlock();
        return 3;
    }
    else if(valString[1].compare("->")==0){
        //if the value is already in the symbolTable
        if(Singleton().getInstance()->symbolTable.find(valString[2]) != Singleton().getInstance()->symbolTable.end()){
            mutex_lock.lock();
            //update the arrow to 1
            Singleton().getInstance()->symbolTable[valString[2]]->inOut = 1;
            //add to symbolTable with the new name given in valString[0]
            Singleton().getInstance()->symbolTable[valString[0]] = Singleton().getInstance()->symbolTable.find(valString[2])->second;
            mutex_lock.unlock();
        }
        else{
            //if it doesn't in the symbolTable
            ObjectData *objectData = new ObjectData(1, valString[2],0);
            mutex_lock.lock();
            Singleton().getInstance()->symbolTable[valString[0]] = objectData;
            mutex_lock.unlock();
        }
        return 4;
    }
    else if(valString[1].compare("<-")==0) {
        //if the value is already in the symbolTable
        if(Singleton().getInstance()->symbolTable.find(valString[2]) != Singleton().getInstance()->symbolTable.end()){
            mutex_lock.lock();
            //add to symbolTable with the new name given in valString[0]
            Singleton().getInstance()->symbolTable[valString[0]] = Singleton().getInstance()->symbolTable.find(valString[2])->second;
            mutex_lock.unlock();
        }
        else{
            //if it doesn't in the symbolTable
            //valString[2] is the sim of the variable, and the value of the variable isn't
            // given in the fly.txt so we set it to be 0
            ObjectData *objectData = new ObjectData(0, valString[2], 0);
            mutex_lock.lock();
            Singleton().getInstance()->symbolTable[valString[0]] = objectData;
            mutex_lock.unlock();
        }
        return 4;
    }
}
/*
 * the function set the value of variable. we have 2 cases:
 * one case is that the value is a number and the second case is that the value is expression
 */
int EqualCommand::execute(vector<string> valString) {
    if (regex_match(valString[1], regex("^[-+]?[0-9]+\\.?[0-9]*$"))) {// for ex. autostart = 1
        mutex_lock.lock();
        //change the value in the symbolTable
        Singleton().getInstance()->symbolTable.find(valString[0])->second->value = stof(valString[1]);
        mutex_lock.unlock();
        //create set command to the Simulator and add it to the queue
        ObjectData *object_data = Singleton().getInstance()->symbolTable.find(valString[0])->second;
        string str = "set " + object_data->sim + " " + valString[1] + "\r\n";
        Singleton().getInstance()->messages_from_client.push(str);
    }
    else { //for expressions
        // calculateExpression will return float number which is the result of the expression
        float f = Singleton().getInstance()->calculateExpression(valString[1]);
        mutex_lock.lock();
        Singleton().getInstance()->symbolTable.find(valString[0])->second->value = f;
        mutex_lock.unlock();
        string str = "set " + Singleton().getInstance()->symbolTable.find(valString[0])->second->sim + " " + to_string(f) + "\r\n";
        Singleton().getInstance()->messages_from_client.push(str);
    }
    return 1;
}
/*
 * PrintCommand get in valString[0] the string that the function need to print
 */
int PrintCommand::execute(vector<string> valString){
    string printString =  valString[0].c_str();
    //check if we have to print a value from the symbolTable
    if(Singleton().getInstance()->symbolTable.find(valString[0]) != Singleton().getInstance()->symbolTable.end()){
        cout << Singleton().getInstance()->symbolTable.find(valString[0])->second->getValue() << endl;
    } else if (printString.find("\"") != string::npos) {
        // if " in the string it is mean that we need to print all the string given in valString[0]
        printString.erase(std::remove(printString.begin(), printString.end(), '\"'), printString.end());
        cout << printString << endl;
    } else {
        // valString[0[ is expression. so we will calculate it by using calculateExpression function.
        float f = Singleton().getInstance()->calculateExpression(valString[0]);
        cout << f << endl;
    }
    return 1;
}
/*
 * the thread which call this function will sleep for the value in valString[0]
 */
int SleepCommand::execute(vector<string> valString){
    int sleep =  Singleton().getInstance()->calculateExpression(valString[0]);
    this_thread::sleep_for(chrono::milliseconds(sleep));
    return 1;
}
/*
 * valString that the function get contain the condition of the while loop and also the body of the loop.
 * the function will run the while loop by its data
 */
int LoopCommand::execute(vector<string> valString){
    // the left string of the condition
    string leftWhile="";
    // the right string of the condition
    string rightWhile="";
    // the operator of the condition
    string operatorWhile="";
    bool finish = false;
    int k, i, j;
    for (k = 0; k < valString.size(); k++) {
        if(!finish) {
            // finding the operatore and create leftWhile and rightWhile
            if (valString[k].compare("<") == 0) {
                operatorWhile = "<";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];//the word before the operator
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];//the word after the operator
                    if (valString[j].compare("{")) {
                        // finding the left and right strings and the operator of the condition
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare(">") == 0) {
                operatorWhile = ">";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];//the word before the operator
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];//the word after the operator
                    if (valString[j].compare("{")) {
                        // finding the left and right strings and the operator of the condition
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("<=") == 0) {
                operatorWhile = "<=";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];//the word before the operator
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];//the word after the operator
                    if (valString[j].compare("{")) {
                        // finding the left and right strings and the operator of the condition
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare(">=") == 0) {
                operatorWhile = ">=";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];//the word before the operator
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];//the word after the operator
                    if (valString[j].compare("{")) {
                        // finding the left and right strings and the operator of the condition
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("==") == 0) {
                operatorWhile = "==";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];//the word before the operator
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];//the word after the operator
                    if (valString[j].compare("{")) {
                        // finding the left and right strings and the operator of the condition
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("!=") == 0) {
                operatorWhile = "!=";
                for ( i = 1; i < k; i++) {
                    leftWhile += valString[i];//the word before the operator
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightWhile += valString[j];//the word after the operator
                    if (valString[j].compare("{")) {
                        // finding the left and right strings and the operator of the condition
                        break;
                    }
                }
                finish = true;
                break;
            }
        }
    }
    //calculate the condition part
    float valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
    float valRight = Singleton().getInstance()->calculateExpression(rightWhile);
    // creating valString without the line of while
    vector<string> copy = valString;
    int sum = i+j;
    //delete the condition part, and keep the part between the {}
    while(sum > 0){
        copy.erase(copy.begin());
        sum--;
    }
    if(operatorWhile.compare("<")==0) {
        while (valLeft < valRight) {
            //run the parser function in Singleton with the left commands in the lexer
            Singleton().getInstance()->parser(copy);
            //calculate again the condition part because thet may changed
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
        }
    } else if(operatorWhile.compare(">")==0) {
        while (valLeft > valRight) {
            //run the parser function in Singleton with the left commands in the lexer
            Singleton().getInstance()->parser(copy);
            //calculate again the condition part because thet may changed
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
        }
    } else if(operatorWhile.compare("<=")==0) {
        while (valLeft <= valRight) {
            //run the parser function in Singleton with the left commands in the lexer
            Singleton().getInstance()->parser(copy);
            //calculate again the condition part because thet may changed
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
        }
    } else if (operatorWhile.compare(">=")==0) {
        while (valLeft >= valRight) {
            //run the parser function in Singleton with the left commands in the lexer
            Singleton().getInstance()->parser(copy);
            //calculate again the condition part because thet may changed
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
        }
    } else if (operatorWhile.compare("==")==0) {
        while (valLeft == valRight) {
            //run the parser function in Singleton with the left commands in the lexer
            Singleton().getInstance()->parser(copy);
            //calculate again the condition part because thet may changed
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
        }
    } else if(operatorWhile.compare("!=")==0) {
        while (valLeft != valRight) {
            //run the parser function in Singleton with the left commands in the lexer
            Singleton().getInstance()->parser(copy);
            //calculate again the condition part because thet may changed
            valLeft = Singleton().getInstance()->calculateExpression(leftWhile);
            valRight = Singleton().getInstance()->calculateExpression(rightWhile);
        }
    }
    return 0;
}
/*
 *  valString that the function get contain the condition of if condition and also the body of the condition.
 * if the condition is true so the body of the condition will run
 */
int IfCommand::execute(vector<string> valString){
    string leftIf="";
    string rightIf="";
    string operatorIf="";
    bool finish = false;
    int k, i, j;
    for (k = 0; k < valString.size(); k++) {
        if(!finish) {
            if (valString[k].compare("<") == 0) {
                operatorIf = "<";
                for ( i = 1; i < k; i++) {
                    leftIf += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightIf += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare(">") == 0) {
                operatorIf = ">";
                for ( i = 1; i < k; i++) {
                    leftIf += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightIf += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("<=") == 0) {
                operatorIf = "<=";
                for ( i = 1; i < k; i++) {
                    leftIf += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightIf += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare(">=") == 0) {
                operatorIf = ">=";
                for ( i = 1; i < k; i++) {
                    leftIf += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightIf += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("==") == 0) {
                operatorIf = "==";
                for ( i = 1; i < k; i++) {
                    leftIf += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightIf += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            } else if (valString[k].compare("!=") == 0) {
                operatorIf = "!=";
                for ( i = 1; i < k; i++) {
                    leftIf += valString[i];
                }
                for ( j = k + 1; j < valString.size(); j++) {
                    rightIf += valString[j];
                    if (valString[j].compare("{")) {
                        break;
                    }
                }
                finish = true;
                break;
            }
        }
    }
    float valLeft = Singleton().getInstance()->calculateExpression(leftIf);
    float valRight = Singleton().getInstance()->calculateExpression(rightIf);
    // creating valString without the line of while
    vector<string> copy = valString;
    int sum = i+j;
    while(sum > 0){
        copy.erase(copy.begin());
        sum--;
    }
    if(operatorIf.compare("<")==0) {
        if (valLeft < valRight) {
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftIf);
            valRight = Singleton().getInstance()->calculateExpression(rightIf);
        }
    } else if(operatorIf.compare(">")==0) {
        if (valLeft > valRight) {
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftIf);
            valRight = Singleton().getInstance()->calculateExpression(rightIf);
        }
    } else if(operatorIf.compare("<=")==0) {
        if (valLeft <= valRight) {
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftIf);
            valRight = Singleton().getInstance()->calculateExpression(rightIf);
        }
    } else if (operatorIf.compare(">=")==0) {
        if (valLeft >= valRight) {
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftIf);
            valRight = Singleton().getInstance()->calculateExpression(rightIf);
        }
    } else if (operatorIf.compare("==")==0) {
        if (valLeft == valRight) {
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftIf);
            valRight = Singleton().getInstance()->calculateExpression(rightIf);
        }
    } else if(operatorIf.compare("!=")==0) {
        if (valLeft != valRight) {
            Singleton().getInstance()->parser(copy);
            valLeft = Singleton().getInstance()->calculateExpression(leftIf);
            valRight = Singleton().getInstance()->calculateExpression(rightIf);
        }
    }
    return 0;
}
/*
 * the method will return true if s is number and false otherwise
 */
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
    delete sArray;
    return flag;
}
/*
 * the method will return true if token is number and false otherwise
 */
bool  OpenServerCommand :: isFloatnumber(char *token) {
    std::string s(token);
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