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
	VehicleFuelConsumptionCalc trip;
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

void run_exprtk_examples() {
	//Calculus_1 test;
	//exprtk_examples::trig_function<double>();
	//exprtk_examples::rpn_example<double>();
	//exprtk_examples::fibonacci<double>();
	//exprtk_examples::vector_overflow_example<double>();
	exprtk_examples::numerical_derivative<double>();
}

void exprtk_practice() {
	std::string equation = "y^2 + 5 - 3"; // Define the equation

	double y_target = 3; // Value that y is approaching
	double initial_step = 0.5; // Initial step size
	double curve_factor = 1.0; // Factor to adjust the step size(exponential) in each iteration
	double step_factor = 0.05; // Factor to adjust the step size(uniform) in each iteration
	double stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)
	int max_iterations = 40; // Maximum iterations(once reached it finish running)

	LimitApproximator approximator(equation, y_target, initial_step, curve_factor, step_factor, stop_accuracy, max_iterations);
	approximator.print_result();
}

void exprtk_practice_xy() {
	std::string equation = "y^2 + x - 3"; // Define the equation

	double x_target = 3; // Value that x is approaching
	double x_initial_step = 0.5; // Initial step size
	double x_curve_factor = 1.0; // Factor to adjust the step size(exponential) in each iteration
	double x_step_factor = 0.04; // Factor to adjust the step size(uniform) in each iteration
	double x_stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)

	double y_target = 3; // Value that y is approaching
	double y_initial_step = 0.5; // Initial step size
	double y_curve_factor = 1.1; // Factor to adjust the step size(exponential) in each iteration
	double y_step_factor = 0.00; // Factor to adjust the step size(uniform) in each iteration
	double y_stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)

	int max_iterations = 40; // Maximum iterations(once reached it finish running)

	LimitApproximator approximator(equation, x_target, x_initial_step, x_curve_factor, x_step_factor, x_stop_accuracy, y_target, y_initial_step, y_curve_factor, y_step_factor, y_stop_accuracy, max_iterations);
	approximator.print_result_xy();
}

void exprtk_SFML_LimitApproximator_practice() {

	std::string equation = "sin(y)"; // Define the equation

	double y_target = 0; // Value that y is approaching
	double initial_step = 10; // Initial step size
	double curve_factor = 1.0; // Factor to adjust the step size(exponential) in each iteration
	double step_factor = 0.2; // Factor to adjust the step size(uniform) in each iteration
	double stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)
	int max_iterations = 100; // Maximum iterations(once reached it finish running)

	LimitApproximator approximator(equation, y_target, initial_step, curve_factor, step_factor, stop_accuracy, max_iterations);
	approximator.disable_stop_accuracy();
	approximator.print_result();

	std::vector<double> xValues_1;
	std::vector<double> yValues_1;
	std::vector<double> xValues_2;
	std::vector<double> yValues_2;
	std::vector<std::pair<double, double>> From_Left = approximator.get_From_Left();
	std::vector<std::pair<double, double>> From_Right = approximator.get_From_Right();
	/*
	TommyTools::splitPairsToVectors(From_Left, xValues_1, yValues_1);

    Graph graph1(800, 600, "graph1");

	graph1.setPoints(xValues_1, yValues_1);
	graph1.setXAxisHeight(0.5); // Set y=0 position (0.5 for middle)
	graph1.setAutoResolution();
	graph1.print_variables();
	graph1.draw();
	*/
	TommyTools::splitPairsToVectors(From_Right, xValues_2, yValues_2);

	Graph graph2(800, 600, "graph2");

	graph2.setPoints(xValues_2, yValues_2);
	graph2.setXAxisHeight(0.5); // Set y=0 position (0.5 for middle)
	graph2.setAutoResolution();
	graph2.print_variables();
	graph2.draw();

}

void exprtk_SFML_simpleplot_practice() {
	std::string equation = "tan(x)"; // Example equation
	double x_min = -20 * M_PI, x_max = 10 * M_PI;

	Plot_Values plotter(equation, x_min, x_max, 0, 1000);

	const auto& xValues = plotter.getXValues();
	const auto& yValues = plotter.getYValues();

	for (size_t i = 0; i < xValues.size(); ++i) {
		std::cout << "x: " << xValues[i] << ", y: " << yValues[i] << std::endl;
	}

	Graph graph(1920, 1080, "graph");

	graph.setPoints(xValues, yValues);
	graph.setXAxisHeight(0.5); // Set y=0 position (0.5 for middle)
	graph.setYAxisPosition(0.5); // Set y=0 position (0.5 for middle)
	graph.setAutoResolution();
	graph.print_variables();
	graph.draw();

	Graph graph1(1920, 1080, "graph");

	graph1.setPoints(xValues, yValues);
	graph1.setXAxisHeight(0.2); // Set y=0 position (0.5 for middle)
	graph1.setYAxisPosition(0.2); // Set y=0 position (0.5 for middle)
	graph1.setAutoResolution();
	graph1.print_variables();
	graph1.draw();

	Graph graph2(1920, 1080, "graph");

	graph2.setPoints(xValues, yValues);
	graph2.setXAxisHeight(0.2); // Set y=0 position (0.5 for middle)
	graph2.setYAxisPosition(0.8); // Set y=0 position (0.5 for middle)
	graph2.setAutoResolution();
	graph2.print_variables();
	graph2.draw();

	Graph graph3(1920, 1080, "graph");

	graph3.setPoints(xValues, yValues);
	graph3.setXAxisHeight(0.8); // Set y=0 position (0.5 for middle)
	graph3.setYAxisPosition(0.8); // Set y=0 position (0.5 for middle)
	graph3.setAutoResolution();
	graph3.print_variables();
	graph3.draw();

	Graph graph4(1920, 1080, "graph");

	graph4.setPoints(xValues, yValues);
	graph4.setXAxisHeight(0.8); // Set y=0 position (0.5 for middle)
	graph4.setYAxisPosition(0.2); // Set y=0 position (0.5 for middle)
	graph4.setAutoResolution();
	graph4.print_variables();
	graph4.draw();

	Graph graph5(1920, 1080, "graph");

	graph5.setPoints(xValues, yValues);
	graph5.setXAxisHeight(0.35); // Set y=0 position (0.5 for middle)
	graph5.setYAxisPosition(0.35); // Set y=0 position (0.5 for middle)
	graph5.setAutoResolution();
	graph5.print_variables();
	graph5.draw();

	Graph graph6(1920, 1080, "graph");

	graph6.setPoints(xValues, yValues);
	graph6.setXAxisHeight(0.35); // Set y=0 position (0.5 for middle)
	graph6.setYAxisPosition(0.65); // Set y=0 position (0.5 for middle)
	graph6.setAutoResolution();
	graph6.print_variables();
	graph6.draw();

	Graph graph7(1920, 1080, "graph");

	graph7.setPoints(xValues, yValues);
	graph7.setXAxisHeight(0.65); // Set y=0 position (0.5 for middle)
	graph7.setYAxisPosition(0.65); // Set y=0 position (0.5 for middle)
	graph7.setAutoResolution();
	graph7.print_variables();
	graph7.draw();

	Graph graph8(1920, 1080, "graph");

	graph8.setPoints(xValues, yValues);
	graph8.setXAxisHeight(0.65); // Set y=0 position (0.5 for middle)
	graph8.setYAxisPosition(0.35); // Set y=0 position (0.5 for middle)
	graph8.setAutoResolution();
	graph8.print_variables();
	graph8.draw();
}

void exprtk_SFML_recursiveplot_logistic_map() {

	std::string logistic_equation;
	double growth_rate;  // For non-chaotic behavior, r < 3, 3 < r < 3.57 (diverge), r > 3.57 (chaos), when r >= 4 it's very easy to hit 0 or infinity due to limit of the floating point system
	double initial_x;    // Initial population ratio
	int iterations;      // Number of iterations

	logistic_equation = "r * x * (1 - x)";
	growth_rate = 2.9;
	initial_x = 0.5;  
	iterations = 100;    

	Recursive_Plot_Values logistic_map1(logistic_equation, growth_rate, initial_x, iterations);

	for (size_t i = 0; i < logistic_map1.getXValues().size(); ++i) {
		std::cout << "x: " << logistic_map1.getXValues()[i] << ", y: " << logistic_map1.getYValues()[i] << std::endl;
	}

	Graph graph1(1920, 1080, "graph");

	graph1.setPoints(logistic_map1.getXValues(), logistic_map1.getYValues());
	graph1.setXAxisHeight(0.2); // Set y=0 position (0.5 for middle)
	graph1.setYAxisPosition(0.2); // Set y=0 position (0.5 for middle)
	graph1.setAutoResolution(0.8);
	graph1.print_variables();
	graph1.draw();

	logistic_equation = "r * x * (1 - x)";
	growth_rate = 3.1;
	initial_x = 0.5;
	iterations = 100;

	Recursive_Plot_Values logistic_map2(logistic_equation, growth_rate, initial_x, iterations);

	for (size_t i = 0; i < logistic_map2.getXValues().size(); ++i) {
		std::cout << "x: " << logistic_map2.getXValues()[i] << ", y: " << logistic_map2.getYValues()[i] << std::endl;
	}

	Graph graph2(1920, 1080, "graph");

	graph2.setPoints(logistic_map2.getXValues(), logistic_map2.getYValues());
	graph2.setXAxisHeight(0.2); // Set y=0 position (0.5 for middle)
	graph2.setYAxisPosition(0.2); // Set y=0 position (0.5 for middle)
	graph2.setAutoResolution(0.8);
	graph2.print_variables();
	graph2.draw();

	logistic_equation = "r * x * (1 - x)";
	growth_rate = 3.6;
	initial_x = 0.5;
	iterations = 100;

	Recursive_Plot_Values logistic_map3(logistic_equation, growth_rate, initial_x, iterations);

	for (size_t i = 0; i < logistic_map3.getXValues().size(); ++i) {
		std::cout << "x: " << logistic_map3.getXValues()[i] << ", y: " << logistic_map3.getYValues()[i] << std::endl;
	}

	Graph graph3(1920, 1080, "graph");

	graph3.setPoints(logistic_map3.getXValues(), logistic_map3.getYValues());
	graph3.setXAxisHeight(0.2); // Set y=0 position (0.5 for middle)
	graph3.setYAxisPosition(0.2); // Set y=0 position (0.5 for middle)
	graph3.setAutoResolution(1.2);
	//graph3.setResolution(11,1500);
	graph3.print_variables();
	graph3.draw();

	logistic_equation = "r * x * (1 - x)";
	growth_rate = 3.99;
	initial_x = 0.5;
	iterations = 100;

	Recursive_Plot_Values logistic_map4(logistic_equation, growth_rate, initial_x, iterations);

	for (size_t i = 0; i < logistic_map4.getXValues().size(); ++i) {
		std::cout << "x: " << logistic_map4.getXValues()[i] << ", y: " << logistic_map4.getYValues()[i] << std::endl;
	}

	Graph graph4(1920, 1080, "graph");

	graph4.setPoints(logistic_map4.getXValues(), logistic_map4.getYValues());
	graph4.setXAxisHeight(0.2); // Set y=0 position (0.5 for middle)
	graph4.setYAxisPosition(0.2); // Set y=0 position (0.5 for middle)
	graph4.setAutoResolution(1);
	graph4.print_variables();
	graph4.draw();
}