#include <iostream>
#include "TommyHeader.h"

using namespace TommyLogic;
/*
TRUTH TABLE

AND----OR----NAND----NOR----XNOR----XOR
00 0   00 0  00 1    00 1   00 1    00 0
01 0   01 1  01 1    01 0   01 0    01 1
10 0   10 1  10 1    10 0   10 0    10 1
11 1   11 1  11 0    11 0   11 1    11 0

*/

//1 YES, 2 NOT, 3 AND, 4 OR, 5 DELAY

int main() {
	
	//TESTING RECURSIVE GATES
	LogicGate obj1("obj1"), obj2("obj2"), obj3("obj3"), obj4("obj4"), obj5("obj5"), obj6("obj6"), obj7("obj7"), obj8("obj8"), obj9("obj9");

	obj1.assignFunctions(1);//YES
	obj2.assignFunctions(1);//YES
	obj3.assignFunctions(3);//AND
	obj4.assignFunctions(1);//NOT USED
	obj5.assignFunctions(5);//DELAY
	obj6.assignFunctions(1);//YES
	obj7.assignFunctions(5);//DELAY
	obj8.assignFunctions(5);//DELAY
	obj9.assignFunctions(3);//AND

	obj3.input({ &obj1,&obj2 });
	obj5.input({ &obj3 });
	obj6.input({ &obj5 });
	obj7.input({ &obj6 });
	obj8.input({ &obj6 });
	obj9.input({ &obj7,&obj8 });

	obj1.input(1);
	obj2.input(0);

	obj9.call();

	UseGate gate(&obj9);
	gate.printAll();
	gate.clear();

	cout << endl << "======================" << endl << endl;

	obj1.input(1);
	obj2.input(1);

	obj9.call();

	gate.printAll();

	SplitGate testgate(&obj9);
	testgate.printSpecialNodes();


}
