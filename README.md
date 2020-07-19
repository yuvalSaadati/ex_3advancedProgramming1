# ex_3advancedProgramming1
Advanced Programming 1 
# Flight Gear

## Description

Interpreter for a new programming language which allows remote control of the flight Gear Simulator.

 ## Installation

download the [flightGear](https://www.flightgear.org/) from the formal website.

## Contributing

### Design - Singleton Pattern

we chose this pattern so we can create one Object and access it from any other classes. for example, the __SymbolTable__ map was created in the Singleton, so different Command classes can access it , and also the __valuesFromSim__ array which keeping all the Simulator directories also there for the same reason. 

The  __lexer__ and the __parser__ are also in the Singleton class. The  __lexer__ function goes over the fly.txt file and splits the text to a Strings Vector. The __parser__ goes over the Vector and executes the Commands. That's why we created a __CommandMap__ that connect the String to a Command Object, for example: for "openDataSrver" String the CommandMap will create a command object and then press the execute function of the same Command.

### Command Pattern

Each Command has execute() function. let's focus on some of them:

__OpenDataSrever__ - the simulator connects to our program and we're reeding values from it into a buffer. while go over the buffer we can change the values or add new ones to the SymbolTable.

__ConnectClientCommand__ - our program connects the simulator and sending to it values to update using a queue of Strings. the queue has Strings like "‫‪set controls/engines/current-engine/throttle 1".

__DefineVarCommand__ - getting two types of values to update:

1. -> / <- 

   adding the symbolTable new variables and defined who will update them - the Simulator(<-) or the our code (->).

2. =

   when there is a new variable that equals to an exist variable in the SymbolTable, so we add it.

__EqualCommand__ - whenever we have to send the simulator new value for a variable.

__Sleep__ - stop reading the text file.

__Print__ - print a String or a value of variable.

__LoopCommand__/ __ifCommand__ - using the same logic for both. First, we parsed the condition part- we're looking for the operator and the expressions before and after it. Second, we go over the body part as long as the condition it true and send it to the parser function in the Singleton. 

### Expressions and Calculate

calculates complex expressions such as: "alt+rpm", "(5+9)*7", "5400 + 2", etc.

 we're using the Shunting yard Algorithm to get the expression in prefix representation, and then using stack and queue we calculate the expression.  

In each Command in the Command Pattern we check for complex expression and if needed, then calculate it. The calculation returns a float number.  
