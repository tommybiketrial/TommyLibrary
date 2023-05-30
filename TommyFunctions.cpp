#pragma once
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

void TommyLogic_Circular_Dependency_Test() {
	//TESTING RECURSIVE GATES
	LogicGate obj1("obj1"), obj2("obj2"), obj3("obj3"), obj4("obj4"), obj5("obj5"), obj6("obj6"), obj7("obj7"), obj8("obj8"), obj9("obj9");

	obj1.assignFunctions(1);//YES
	obj2.assignFunctions(1);//YES
	obj3.assignFunctions(2);//NOT
	obj4.assignFunctions(3);//AND
	obj5.assignFunctions(3);//AND
	obj6.assignFunctions(4);//OR
	obj7.assignFunctions(5);//DELAY
	obj8.assignFunctions(5);//DELAY
	obj9.assignFunctions(1);//YES

	obj3.input({ &obj1 });
	obj4.input({ &obj3,&obj7 });
	obj5.input({ &obj1,&obj2 });
	obj6.input({ &obj4,&obj5 });
	obj7.input({ &obj6 });
	obj8.input({ &obj7 });
	obj9.input({ &obj8 });

	UseGate gate(&obj9);
	gate.toggleDebugMode_Connected(5);

	SplitGate testgate(&obj9);
	testgate.printSpecialNodes();
	testgate.printSplits();

	obj9.call();

	gate.clear();

	gate.setTick(5);


	cout << endl << "=====TICK?=====" << endl;
	for (int i = 0; i < gate.CurrentObjArr.size(); i++) {
		cout << gate.CurrentObjArr[i]->Name << ": " << gate.CurrentObjArr[i]->Tick << endl;
	}

	obj1.inputarr({ {0,1},{1,0},{0,1} });
	obj2.inputarr({ {1,0},{0,1},{1,0} });
	gate.activate({ &obj1,&obj2 });

	gate.printAll();



	/* print whether other objs have received the Input Obj list
	for (int i = 0; i < gate.CurrentObjArr.size(); i++) {
		cout << gate.CurrentObjArr[i]->Name << ": ";
		for (int j = 0; j < gate.CurrentObjArr[i]->ObjInputList.size(); j++) {
			cout << gate.CurrentObjArr[i]->ObjInputList[j]->Name << " ";
		}cout << endl;
	}
	*/

	/* print whether other objs have received the CurrentObjArr list
	for (int i = 0; i < gate.CurrentObjArr.size(); i++) {
		cout << gate.CurrentObjArr[i]->Name << ": ";
		for (int j = 0; j < gate.CurrentObjArr[i]->AllObj.size(); j++) {
			cout << gate.CurrentObjArr[i]->AllObj[j]->Name << " ";
		}cout << endl;
	}
	*/

	/* print to check the FirstDelayNode conditions for all gates
	for (int i = 0; i < gate.CurrentObjArr.size(); i++) {
		cout << gate.CurrentObjArr[i]->Name << ": " << gate.CurrentObjArr[i]->FirstDelayNode;
		cout << endl;
	}
	*/
	
}

void TommyLogic_Legacy_Non_Circular_Depending_Test() {

	LogicGate obj1("obj1"), obj2("obj2"), obj3("obj3"), obj4("obj4"), obj5("obj5"), obj6("obj6"), obj7("obj7"), obj8("obj8"), obj9("obj9");

	obj1.assignFunctions(1);//YES
	obj2.assignFunctions(1);//YES
	obj3.assignFunctions(3);//AND
	obj4.assignFunctions(3);//AND
	obj5.assignFunctions(2);//NOT
	obj6.assignFunctions(2);//NOT
	obj7.assignFunctions(1);//YES
	obj8.assignFunctions(1);//YES
	obj9.assignFunctions(1);//YES

	obj3.input({ &obj1,&obj2 });
	obj4.input({ &obj2 });

	obj5.input({ &obj3 });
	obj6.input({ &obj4 });
	obj7.input({ &obj5 });
	obj8.input({ &obj6 });
	obj9.input({ &obj7, &obj8 });

	UseGate gate(&obj9);
	gate.toggleDebugMode_Connected(5);

	SplitGate testgate(&obj9);
	testgate.printSpecialNodes();
	testgate.printSplits();

	obj1.input(1);
	obj2.input(1);

	obj9.call();

	gate.printAll();
}

void TommyCounter_Test() {
	string test;
	getline(cin, test);
	TommyCounter CountingTest(test);
	cout << CountingTest.CountCommas() << endl;
	cout << CountingTest.getNumberOfEnters();
}

void FunctionalGate_Test() {

	FunctionalGate obj1("g1"), obj2("g2"), obj3("g3"), obj4("g4"), obj5("g5"), obj6("g6");

	obj1.input({ &obj2 });
	obj2.input({ &obj3 });
	obj3.input({ &obj1, &obj4, &obj5, &obj6 });
	obj4.input({ &obj1 });
	obj5.input({ &obj1 });
	obj6.input({ &obj1 });

	obj1.input(1, { 1,2,3,4 });
	obj2.input(1, { 3,2,1 });
	obj3.input(1, { 0,0,0,0,0,999,1 });
	obj4.input(1, { 0,0,0,0,0,0,0,250 });
	obj5.input(1, { 0,0,0,0,0,0,0,0,0,999999 });
	obj6.input(1, { 0,0,0,0,0,0,0,0,0,0,111111 });

	obj1.setLoopLimit(0);
	obj1.initialize();

	obj1.setLoopLimit(5);
	obj2.setLoopLimit(5);
	obj3.setLoopLimit(5);
	obj4.setLoopLimit(5);
	obj5.setLoopLimit(5);
	obj6.setLoopLimit(5);
	//obj1.run();

	FunctionalGate_Helper helper1("the helper");

	helper1.input_Start_Gate(&obj1);
	helper1.input_Gates_for_Printing({ &obj1, &obj2, &obj3, &obj4, &obj5, &obj6 });
	helper1.print();

}

void Tommy_BigInt_Test() {
	PROFINY_SCOPE
	InfInt binary = "101110101000101101110101111000011110100001011001110011110001111100100011101111100010000100000111111100101001";
	InfInt decimal = InfInt_Extra::binaryToDecimal(binary);
	//decimal = decimal + binary;
	string decimal_str = decimal.toString();
	cout << "Decimal: " << decimal_str << endl;
}

void LitreGallonFuelCalculator() {
	VehicleFuelConsumptionPerTrip trip;
	trip.setLitreOfFuelConsumed(24.59);
	trip.setLastTimePumpedFuelMileage_Mile(55321);
	trip.setThisTimePumpedFuelMileage_Mile(55572);
	//trip.setMPG(48);
	trip.setFuelPricePerLitre(1.42);
	trip.setEstimate_Trip_in_Miles(16);
	trip.setFuelType("Petrol");

	trip.calculateVariables();

	cout << "LastTimePumpedFuelMileage_Kilometer: " << trip.getLastTimePumpedFuelMileage_Kilometer() << endl;
	cout << "ThisTimePumpedFuelMileage_Kilometer: " << trip.getThisTimePumpedFuelMileage_Kilometer() << endl;
	cout << "DeltaPumpedFuelMileage_Kilometer: " << trip.getDeltaPumpedFuelMileage_Kilometer() << endl;
	cout << "LastTimePumpedFuelMileage_Mile: " << trip.getLastTimePumpedFuelMileage_Mile() << endl;
	cout << "ThisTimePumpedFuelMileage_Mile: " << trip.getThisTimePumpedFuelMileage_Mile() << endl;
	cout << "DeltaPumpedFuelMileage_Mile: " << trip.getDeltaPumpedFuelMileage_Mile() << endl;
	cout << "MPG: " << trip.getMPG() << endl;
	cout << "Estimate_Trip_in_Kilometer: " << trip.getEstimate_Trip_in_Kilometer() << endl;
	cout << "Estimate_Trip_in_Miles: " << trip.getEstimate_Trip_in_Miles() << endl;
	cout << "FuelCostPerKilometer: " << trip.getFuelCostPerKilometer() << endl;
	cout << "FuelCostPerMile: " << trip.getFuelCostPerMile() << endl;
	cout << "FuelCost_Estimate_Trip: " << trip.getFuelCost_Estimate_Trip() << endl;
	cout << "FuelPricePerGallon: " << trip.getFuelPricePerGallon() << endl;
	cout << "FuelPricePerLitre: " << trip.getFuelPricePerLitre() << endl;
	cout << "FuelType: " << trip.getFuelType() << endl;
	cout << "GallonOfFuelConsumed: " << trip.getGallonOfFuelConsumed() << endl;
	cout << "LitreOfFuelConsumed: " << trip.getLitreOfFuelConsumed() << endl;
	
}

void generateSudoku() {
	SudokuGenerator puzzle;
	int(*output)[N] = puzzle.getGrid();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << output[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	puzzle.makePuzzle(20);
	int(*output2)[N] = puzzle.getGrid();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << output2[i][j] << " ";
		}
		cout << endl;
	}
}

void generateThenSolveSudoku() {
	vector<SudokuWrapper> sudokus;
	srand(time(0));
	for (int i = 0; i < 10; i++) {
		SudokuWrapper tmp;
		sudokus.push_back(tmp);
	}
	for (int n = 0; n < sudokus.size(); n++) {
		cout << "Initial Valid combination:" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << sudokus[n].input[i][j] << " ";
			}
			cout << endl;
		}
		sudokus[n].makePuzzle(30+n);
		sudokus[n].generate();
		cout << "Puzzle:" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (sudokus[n].puzzle[i][j] == 0) {
					cout << "  ";
				}
				else {
					cout << sudokus[n].puzzle[i][j] << " ";
				}
			}
			cout << endl;
		}
		cout << "Solutions:" << endl;
		for (int k = 0; k < sudokus[n].grids.size(); k++) {
			cout << k+1 << ":" << endl;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					cout << sudokus[n].grids[k][i][j] << " ";
				}
				cout << endl;
			}
			cout << "---------" << endl;
		}
		sudokus[n].deleteData();
	}
	cin.get();
}