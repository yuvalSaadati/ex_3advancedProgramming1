#include <string>
#include "CommandInterface.h"
#include "commandPattern.h"
#include "Singleton.h"
using namespace std;
int main(int argc,char* argv[]) {
    Singleton* s = Singleton::getInstance();
    s->createSimValuesMap();
    vector<string> lexerVector = s->lexer(argv[1]);
    s->parser(lexerVector);
/*
    for(auto it = Singleton().getInstance()->symbolTable.cbegin(); it != Singleton().getInstance()->symbolTable.cend(); ++it)
    {
        std::cout << it->first << " " << it->second->value << " " <<  "\n";
    }
    */
    return 0;
}