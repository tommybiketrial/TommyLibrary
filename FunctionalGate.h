#pragma once

class FunctionalGate {
private:

	//DEBUG VARIABLES (for now)
	int number_of_calls = 0;
	
	vector<FunctionalGate*> removeVectorDuplicates(vector<FunctionalGate*> v) {
		std::sort(v.begin(), v.end());
		auto last = std::unique(v.begin(), v.end());
		v.erase(last, v.end());
		return v;
	}

public:

	string Name = "";
	int Selector = 0;
	vector<int> ArrayInput; //Manual Array Input, the other Array Input/s depend on the PrevGates->ArrayOutput.
	vector<int> ArrayOutput; //Calculated Array Output

	int LoopLimit = 0;
	int Looped = 0;

	vector<FunctionalGate*> PrevGates;
	vector<FunctionalGate*> LoopPrevGates; //Store PrevGates looped results

	vector<FunctionalGate*> NextGates;

	vector<FunctionalGate*> CachGates;

	vector<int> TheArray;

	FunctionalGate(string gate_name) {
		Name = gate_name;
	}

	void input(vector<FunctionalGate*> next_gates) {
		NextGates = next_gates;
	}
	void input(int selector, vector<int> array_input) {
		Selector = selector;
		ArrayInput = array_input;
	}
	void input(int selector) {
		Selector = selector;
	}

	void setLoopLimit(int CallLimit_input) {
		LoopLimit = CallLimit_input;
	}

	void run() {
		for (int i = 0; i < CachGates.size(); i++) {
			if (CachGates[i] == this && Looped < LoopLimit) { Looped++; goto SkipCheck; }
			else if (CachGates[i] == this && Looped >= LoopLimit) return;
		}
		SkipCheck:

		number_of_calls++;

		//Calculate based on the functions. E.x. Current object uses both manual input and PrevGate->ArrayOutput to calculate its own ArrayOutput.

		callFunc(Selector,ArrayInput);

		for (int i = 0; i < NextGates.size(); i++) {
			if (NextGates[i]->PrevGates.size() == 0) { NextGates[i]->PrevGates.push_back(this); } //need to do this otherwise the next line (the for loop) will never get activated since size would be 0
			for (int j = 0; j < NextGates[i]->PrevGates.size(); j++) {//check no duplicates while pushing back
				if (NextGates[i]->PrevGates[j] == this) { continue; }
				else if (j == NextGates[i]->PrevGates.size() && NextGates[i]->PrevGates[j] != this) { NextGates[i]->PrevGates.push_back(this); }
			}
			
			NextGates[i]->LoopPrevGates.push_back(this);

			if (NextGates[i]->CachGates.size() == 0) {//need to do this otherwise after this if statement (the for loop) will never get activated since size would be 0
				NextGates[i]->CachGates.push_back(this);
				for (int j = 0; j < this->CachGates.size(); j++) {
					NextGates[i]->CachGates.push_back(this->CachGates[j]);
				}
			} 
			for (int j = 0; j < NextGates[i]->CachGates.size(); j++) {//check no duplicates while pushing back
				if (NextGates[i]->CachGates[j] == this) { continue; }
				else if (j == NextGates[i]->CachGates.size() && NextGates[i]->CachGates[j] != this) {
					NextGates[i]->CachGates.push_back(this);
					for (int j = 0; j < this->CachGates.size(); j++) {
						NextGates[i]->CachGates.push_back(this->CachGates[j]);
					}
				}
			}

			NextGates[i]->run();
		}
	}

	void callFunc(int selector, vector<int> array_input) { //manipulate the array using switch statements, input variables to select the statements to use. (1st variable = selector, 2nd variable = array variable)
		int tmp = 0;
		vector<int> tmp_add;
		switch (selector) {
		case 1: //simple array addition //LATER
			for (int i = 0; i < this->PrevGates.size(); i++) {
				for (int j = 0; j < this->PrevGates[i]->ArrayOutput.size(); j++) {
					tmp += this->PrevGates[i]->ArrayOutput[j];
				}
				tmp_add.push_back(tmp);
			}
		break;
		default:
		break;
		}
	}

	void print() {
		cout << "Gate ["<< this->Name <<"] properties:" << endl;
		cout << "number_of_calls = " << number_of_calls << endl;
		cout << "Time/s Looped = " << Looped << endl;
		cout << "PrevGates: ";
		for (int i = 0; i < this->PrevGates.size(); i++) {
			cout << PrevGates[i]->Name << " | ";
		}cout << endl;
		cout << "NextGates: ";
		for (int i = 0; i < this->NextGates.size(); i++) {
			cout << NextGates[i]->Name << " | ";
		}cout << endl;
		cout << "CachGates: ";
		for (int i = 0; i < this->CachGates.size(); i++) {
			cout << CachGates[i]->Name << " | ";
		}cout << endl;
		cout << "------" << endl;
		cout << "Manual Input: ";
		for (int i = 0; i < this->ArrayInput.size(); i++) {
			cout << this->ArrayInput[i] << " | ";
		}cout << endl;
		cout << "PrevGates ArrayOutput/s: (could be inputs for this gate)" << endl;
		for (int i = 0; i < this->PrevGates.size(); i++) {
			cout << PrevGates[i]->Name << ": ";
			for (int j = 0; j < this->PrevGates[i]->ArrayOutput.size(); j++) {
				cout << this->PrevGates[i]->ArrayOutput[j] << " | ";
			}cout << endl;
		}cout << endl;
		cout << "this->ArrayOutput: ";
		for (int i = 0; i < this->ArrayOutput.size(); i++) {
			cout << this->ArrayOutput[i] << " | ";
		}cout << endl;
		cout << "==========================" << endl << endl;
	}

	void print_looped_results() {
		cout << "Gate [" << this->Name << "] properties:" << endl;
		cout << "number_of_calls = " << number_of_calls << endl;
		cout << "Time/s Looped = " << Looped << endl;
		cout << "PrevGates: ";
		for (int i = 0; i < this->PrevGates.size(); i++) {
			cout << PrevGates[i]->Name << " | ";
		}cout << endl;
		cout << "NextGates: ";
		for (int i = 0; i < this->NextGates.size(); i++) {
			cout << NextGates[i]->Name << " | ";
		}cout << endl;
		cout << "CachGates: ";
		for (int i = 0; i < this->CachGates.size(); i++) {
			cout << CachGates[i]->Name << " | ";
		}cout << endl;
		cout << "------" << endl;
		cout << "Manual Input: ";
		for (int i = 0; i < this->ArrayInput.size(); i++) {
			cout << this->ArrayInput[i] << " | ";
		}cout << endl;
		cout << "LoopPrevGates ArrayOutput/s: (could be inputs for this gate)" << endl;
		for (int i = 0; i < this->LoopPrevGates.size(); i++) {
			cout << LoopPrevGates[i]->Name << ": ";
			for (int j = 0; j < this->LoopPrevGates[i]->ArrayOutput.size(); j++) {
				cout << this->LoopPrevGates[i]->ArrayOutput[j] << " | ";
			}cout << endl;
		}cout << endl;
		cout << "this->ArrayOutput: ";
		for (int i = 0; i < this->ArrayOutput.size(); i++) {
			cout << this->ArrayOutput[i] << " | ";
		}cout << endl;
		cout << "==========================" << endl << endl;
	}

};

