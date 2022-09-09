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
	obj2.assignFunctions(5);//DELAY
	obj3.assignFunctions(4);//OR
	obj4.assignFunctions(1);//YES
	obj5.assignFunctions(1);//YES
	obj6.assignFunctions(5);//DELAY
	obj7.assignFunctions(5);//DELAY
	obj8.assignFunctions(3);//AND
	obj9.assignFunctions(1);//YES

	obj2.input({ &obj3 });
	obj3.input({ &obj1,&obj4 });
	obj4.input({ &obj2 });
	obj5.input({ &obj4 });
	obj6.input({ &obj5 });
	obj7.input({ &obj5 });
	obj8.input({ &obj6,&obj7 });
	obj9.input({ &obj8 });

	obj1.input(1);

	UseGate gate(&obj9);
	gate.toggleDebugMode_Connected(4);

	SplitGate testgate(&obj9);
	testgate.printSpecialNodes();
	testgate.printBranches();

	obj9.call();

	

	cout << obj1.Name << "(Debug Mode): " << obj1.DebugModeLevel << endl;
	cout << obj2.Name << "(Debug Mode): " << obj2.DebugModeLevel << endl;
	cout << obj3.Name << "(Debug Mode): " << obj3.DebugModeLevel << endl;
	cout << obj4.Name << "(Debug Mode): " << obj4.DebugModeLevel << endl;
	cout << obj5.Name << "(Debug Mode): " << obj5.DebugModeLevel << endl;
	cout << obj6.Name << "(Debug Mode): " << obj6.DebugModeLevel << endl;
	cout << obj7.Name << "(Debug Mode): " << obj7.DebugModeLevel << endl;
	cout << obj8.Name << "(Debug Mode): " << obj8.DebugModeLevel << endl;
	cout << obj9.Name << "(Debug Mode): " << obj9.DebugModeLevel << endl;

	cout << endl << "============EVERYTHING EXECUTED==========" << endl << endl;
	





}
