#pragma once

class FunctionalGate_Helper {
public:

	string HelperName = "";

	FunctionalGate* StartGate;

	vector<FunctionalGate*> PrintGates;

	FunctionalGate_Helper(string Helper_Name) {
		HelperName = Helper_Name;
	}

	void input_Start_Gate(FunctionalGate* Start_Gate) {
		StartGate = Start_Gate;
	}

	void input_Gates_for_Printing(vector<FunctionalGate*> Gates_for_Printing) {
		PrintGates = Gates_for_Printing;
	}



	void print() {
		for (int i = 0; i < PrintGates.size(); i++) {
			PrintGates[i]->print_looped_results();
		}
	}

};