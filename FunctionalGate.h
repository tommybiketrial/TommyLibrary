#pragma once

class FunctionalGate {
private:

	//DEBUG VARIABLES (for now)
	int number_of_calls = 0;
	
	void callFunc(int selector, vector<int> array_input) { //manipulate the array using switch statements, input variables to select the statements to use. (1st variable = selector, 2nd variable = array variable)
		vector<vector<int>> tmp;
		switch (selector) {
		case 1: //simple array addition //LATER
			tmp.push_back(array_input);
			for (int i = 0; i < this->PrevGates.size(); i++) {
				tmp.push_back(PrevGates[i]->ArrayOutput);
			}
			this->ArrayOutput = addition_vector_of_vectors(tmp);
			break;
		case 8:
			this->ArrayOutput = TheArray;
			TheArray = ArrayInput;
			break;
		default:
			break;
		}
	}

	vector<FunctionalGate*> removeVectorDuplicates(vector<FunctionalGate*> v) {
		std::sort(v.begin(), v.end());
		auto last = std::unique(v.begin(), v.end());
		v.erase(last, v.end());
		return v;
	}

	vector<FunctionalGate*> addVector_Unique(vector<vector<FunctionalGate*>> vv) {
		for (std::vector<FunctionalGate*>& inner : vv) { // Remove duplicates from each vector using std::unique
			inner.erase(std::unique(inner.begin(), inner.end()), inner.end());
		}
		std::vector<FunctionalGate*> merged;
		for (const std::vector<FunctionalGate*>& inner : vv) { // Append all the vectors in v into a single vector
			merged.insert(merged.end(), inner.begin(), inner.end());
		}
		return merged;
	}

	vector<int> addition_vector_of_vectors(vector<vector<int>> input) {
		vector<int> result;
		if (input.size() == 0)return result;
		int size = 0;
		for (const auto& inner_vec : input) {
			size = std::max(size, (int)inner_vec.size());
		}
		for (auto& inner_vec : input) {// Pad the shorter vectors with zeros
			if (inner_vec.size() < size) {
				inner_vec.resize(size, 0);  // pad with zeros
			}
		}
		for (int j = 0; j < input[0].size(); j++) {
			int tmp = 0;
			for (int i = 0; i < input.size(); i++) {
				tmp += input[i][j];
			}
			result.push_back(tmp);
		}
		return result;
	}

public:

	string Name = "";
	int Selector = 0;
	vector<int> ArrayInput; //Manual Array Input, the other Array Input/s depend on the PrevGates->ArrayOutput.
	vector<int> ArrayOutput; //Calculated Array Output

	int LoopLimit = 0;
	int Looped = 0;

	vector<FunctionalGate*> PrevGates;
	vector<FunctionalGate> LoopPrevGates; //Store PrevGates looped results

	vector<FunctionalGate*> NextGates;

	vector<FunctionalGate*> CachGates;

	vector<FunctionalGate> ArchivedGates; //Store Archived Versions of this Gate.

	vector<int> TheArray;

	FunctionalGate(string gate_name) {
		Name = gate_name;
	}
	FunctionalGate(const FunctionalGate& other) : Name(other.Name),Selector(other.Selector), PrevGates(other.PrevGates), LoopPrevGates(other.LoopPrevGates),NextGates(other.NextGates),CachGates(other.CachGates),ArchivedGates(other.ArchivedGates),ArrayInput(other.ArrayInput),ArrayOutput(other.ArrayOutput),TheArray(other.TheArray) {} //deep copy constructor

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

	void initialize() {
		for (int i = 0; i < CachGates.size(); i++) {
			if (CachGates[i] == this && Looped < LoopLimit) { Looped++; goto SkipCheck; }
			else if (CachGates[i] == this && Looped >= LoopLimit) return;
		}
	SkipCheck:

		for (int i = 0; i < NextGates.size(); i++) {
			if (NextGates[i]->PrevGates.size() == 0) { NextGates[i]->PrevGates.push_back(this); } //need to do this otherwise the next line (the for loop) will never get activated since size would be 0
			for (int j = 0; j < NextGates[i]->PrevGates.size(); j++) {//check no duplicates while pushing back, push all NextGates into the PrevGates of the NextGates while avoiding pushing the ones that are already there.


				if (NextGates[i]->PrevGates[j] != this) { NextGates[i]->PrevGates.push_back(this); }//j == NextGates[i]->PrevGates.size() && 
				
				
			}

			if (NextGates[i]->CachGates.size() == 0) {//need to do this otherwise after this if statement (the for loop) will never get activated since size would be 0
				NextGates[i]->CachGates.push_back(this);
				for (int j = 0; j < this->CachGates.size(); j++) {
					NextGates[i]->CachGates.push_back(this->CachGates[j]);
				}
			}
			for (int j = 0; j < NextGates[i]->CachGates.size(); j++) {//check no duplicates while pushing back
				if (NextGates[i]->CachGates[j] == this) { continue; }
				else if (j == NextGates[i]->CachGates.size() && NextGates[i]->CachGates[j] != this) {//
					NextGates[i]->CachGates.push_back(this);
					for (int k = 0; k < this->CachGates.size(); k++) {
						NextGates[i]->CachGates.push_back(this->CachGates[k]);
					}
				}
			}

			NextGates[i]->initialize();
		}
	}

	void run() {
		for (int i = 0; i < CachGates.size(); i++) {
			if (CachGates[i] == this && Looped < LoopLimit) { Looped++; goto SkipCheck; }
			else if (CachGates[i] == this && Looped >= LoopLimit) return;
		}
		SkipCheck:

		number_of_calls++;

		//ARCHIVE THE this->GATE;
		FunctionalGate tmp(*this);
		ArchivedGates.push_back(tmp);

		//Calculate based on the functions. E.x. Current object uses both manual input and PrevGate->ArrayOutput to calculate its own ArrayOutput.
		callFunc(Selector,ArrayInput);

		for (int i = 0; i < NextGates.size(); i++) {
			if (NextGates[i]->PrevGates.size() == 0) { NextGates[i]->PrevGates.push_back(this); } //need to do this otherwise the next line (the for loop) will never get activated since size would be 0
			for (int j = 0; j < NextGates[i]->PrevGates.size(); j++) {//check no duplicates while pushing back
				if (NextGates[i]->PrevGates[j] == this) { continue; }
				else if (j == NextGates[i]->PrevGates.size() && NextGates[i]->PrevGates[j] != this) { NextGates[i]->PrevGates.push_back(this); } //
			}
			
			NextGates[i]->LoopPrevGates.push_back(tmp);

			if (NextGates[i]->CachGates.size() == 0) {//need to do this otherwise after this if statement (the for loop) will never get activated since size would be 0
				NextGates[i]->CachGates.push_back(this);
				for (int j = 0; j < this->CachGates.size(); j++) {
					NextGates[i]->CachGates.push_back(this->CachGates[j]);
				}
			} 
			for (int j = 0; j < NextGates[i]->CachGates.size(); j++) {//check no duplicates while pushing back
				if (NextGates[i]->CachGates[j] == this) { continue; }
				else if (j == NextGates[i]->CachGates.size() && NextGates[i]->CachGates[j] != this) {//
					NextGates[i]->CachGates.push_back(this);
					for (int k = 0; k < this->CachGates.size(); k++) {
						NextGates[i]->CachGates.push_back(this->CachGates[k]);
					}
				}
			}

			NextGates[i]->run();
		}
	}

	void print() {
		cout << "Gate ["<< this->Name <<"] (selector " << Selector << ") properties:" << endl;
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
		cout << "Gate [" << this->Name << "] (selector " << Selector << ") properties:" << endl;
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
			cout << LoopPrevGates[i].Name << ": ";
			for (int j = 0; j < this->LoopPrevGates[i].ArrayOutput.size(); j++) {
				cout << this->LoopPrevGates[i].ArrayOutput[j] << " | ";
			}cout << endl;
		}cout << endl;
		cout << "this->ArrayOutput: ";
		for (int i = 0; i < this->ArrayOutput.size(); i++) {
			cout << this->ArrayOutput[i] << " | ";
		}cout << endl;
		cout << "this->ArchivedGate: (history of this gate)" << endl;
		for (int i = 0; i < this->ArchivedGates.size(); i++) {
			for (int j = 0; j < this->ArchivedGates[i].ArrayOutput.size(); j++) {
				cout << this->ArchivedGates[i].ArrayOutput[j] << " | ";
			}cout << endl;
		}
		cout << "==========================" << endl << endl;
	}

};