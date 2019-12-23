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
    return 0;
}