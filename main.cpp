#include <string>
#include "CommandInterface.h"
#include "commandPattern.h"
#include "Singleton.h"
using namespace std;
int main(int argc,char* argv[]) {
    // create Singleton object
    Singleton* s = Singleton::getInstance();
    s->createSimValuesMap();
    // the lexer function will return vector of strings which contain all the information in fly.txt
    vector<string> lexerVector = s->lexer(argv[1]);
    // the parser will run all the command functions
    s->parser(lexerVector);
    return 0;
}