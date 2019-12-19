#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <thread>
#include "CommandInterface.h"
#include "commandPattern.h"
#include "Singleton.h"
#include <map>

using namespace std;
int main(int argc,char* argv[]) {
    Singleton* s = Singleton::getInstance();
    vector<string> lexerVector = s->lexer(argv[1]);
    s->parser(lexerVector);
    return 0;
}