#include <iostream>
#include "TommyHeader.h"

using TommyLogic::generateTruthtableXY; using TommyLogic::loopInputUseGate;
/*
TRUTH TABLE

AND----OR----NAND----NOR----XNOR----XOR
00 0   00 0  00 1    00 1   00 1    00 0
01 0   01 1  01 1    01 0   01 0    01 1
10 0   10 1  10 1    10 0   10 0    10 1
11 1   11 1  11 0    11 0   11 1    11 0

*/

//1 YES, 2 NOT, 3 AND, 4 OR

int main() {
	//TESTING RECURSIVE GATES
	LogicGate obj1("obj1"), obj2("obj2"), obj3("obj3"), obj4("obj4"), obj5("obj5"), obj6("obj6"), obj7("obj7"), obj8("obj8"), obj9("obj9"), obj10("obj10");

	//LogicGate obj10("my Nand Gate"), obj11()



	obj1.assignFunctions(1);//YES
	obj2.assignFunctions(1);//YES
	obj3.assignFunctions(3);//AND
	obj4.assignFunctions(3);//AND
	obj5.assignFunctions(2);//NOT
	obj6.assignFunctions(2);//NOT
	obj7.assignFunctions(1);//YES
	obj8.assignFunctions(1);//YES
	obj9.assignFunctions(1);//YES



	obj3.input({ &obj1 }); // Without circular dependency
	obj4.input({ &obj2 }); // Without circular dependency

	//obj3.input({ &obj1, &obj6 }); // With circular dependency
	//obj4.input({ &obj2, &obj5 }); // With circular dependency

	obj5.input({ &obj3 });
	obj6.input({ &obj4 });
	obj7.input({ &obj5 });
	obj8.input({ &obj6 });
	obj9.input({ &obj7, &obj8 });

	UseGate testgate(&obj9);



	loopInputUseGate(testgate, generateTruthtableXY(2), { &obj7,&obj8 });
}




//TODO, implement a TICK system similar to that of CircularDependency.h

//TODO then, modify arrays so that it flush data into a 2 dimensional array PER TICK.