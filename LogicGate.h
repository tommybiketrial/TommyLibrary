#pragma once

class LogicGate; struct ObjectStore;

//TODO Split the tree to avoid circular dependency at the point of DELAY nodes.

struct ObjectStore {
	vector<LogicGate*> ObjTree;
	vector<LogicGate*> DelayNodeArr;
	vector<LogicGate*> InputObjArr;

	void clear() {
		ObjTree.clear();
		DelayNodeArr.clear();
		InputObjArr.clear();
	}
};

class LogicGate {
protected:

	void recurObjFunc(ObjectStore* obj) { //Return a whole list of objects in the hierarchy instead of just the one below
		for (int i = 0; i < obj->ObjTree.size(); i++) {//Avoid Duplicates
			if (obj->ObjTree[i] == this) { 
				return;
			}
		}
		obj->ObjTree.push_back(this);
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->recurObjFunc(obj);
		}
	}

	void toggleDebugMode(ObjectStore* obj, unsigned Mode) {
		for (int i = 0; i < obj->ObjTree.size(); i++) {//Avoid Duplicates
			if (obj->ObjTree[i] == this) {
				return;
			}
		}
		obj->ObjTree.push_back(this);
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->DebugModeLevel = Mode;
			ObjList[i]->toggleDebugMode(obj, Mode);
		}
	}

	void findSpecialNodes(ObjectStore* DelayNodes) { //Return a list of Delay Nodes and save it into an array		
		for (int i = 0; i < DelayNodes->DelayNodeArr.size(); i++) {
			if (DelayNodes->DelayNodeArr[i] == this && this->FundamentalGateName != "DELAY") {
				if(DebugModeLevel>1)cout << "Warning: Circular Dependency (from Special Nodes Function)" << endl;
				return;
			}else if (DelayNodes->DelayNodeArr[i] == this && this->FundamentalGateName == "DELAY") {
				if (DebugModeLevel>2)cout << "Hit a Delay node, stopped searching" << endl;
				return;
			}
		}
		if (this->ObjList.size() == 0) {//When the Objlist of an object is 0, this object is an Input object
			DelayNodes->InputObjArr.push_back(this);
		}
		//It's possible that Input node can also be Delay Node :)
		if (this->FundamentalGateName == "DELAY") {//Save this DELAY node into array
			DelayNodes->DelayNodeArr.push_back(this);
		}
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->findSpecialNodes(DelayNodes);
		}
		return;
	}

private:

	int NumOfInputs = NULL;

	int (LogicGate::* TempFunc) (pair<int, int> inputvalue);

	int YES(pair<int, int> inputvalue) {
		return inputvalue.first;
	}
	int AND(pair<int, int> inputvalue) {
		return (inputvalue.first == 1 && inputvalue.second == 1);
	}
	int OR(pair<int, int> inputvalue) {
		return (inputvalue.first == 1 || inputvalue.second == 1);
	}
	int NOT(pair<int, int> inputvalue) {
		if (inputvalue.first != 0) return 0;
		else return 1;
	}
	int DELAY(pair<int, int> inputvalue) { //always hold the data from input, then release it to output in the next function call
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (DebugModeLevel>3)cout << "sleeping..." << endl; //FundamentalGateName = "DELAY"

		DelayPair.second = DelayPair.first;
		DelayPair.first = inputvalue.first;
		
		return DelayPair.second;
	}



public:

	vector<string> ObjNames;

	pair<int, int> Input = { 2,2 };

	vector<int> CalculatedInput;	//Put the Output into CalculatedInput because intermiediate gates and the final gate for result also function as gates
	pair<int, int> CalculatedInputPair;

	pair<int, int> DelayPair = { 2,2 };

	//============================================

	vector<LogicGate*> ClearFuncCache; //Cache for clear() to clear the entire tree at once
	vector<LogicGate*> ClearFuncCache_MopUp; //Cache for clearMopUp() to clear ClearFuncCache, must be called after clear()

	vector<int> Output;

	unsigned DebugModeLevel = 0;

	vector<int> StoredTickResults; //the stored results whenever activate() is called, store them.

	vector<LogicGate*> ObjList; //the objects to retrieve inputs from

	vector<LogicGate*> ObjReceiverList; //remember all the objects pointed to by previous objects, to recognize whether there's circular dependency

	vector<LogicGate*> ObjTargetList; //the objects to send outputs to

	string Name = "";
	string FundamentalGateName = "";

	vector<int> CalculatedOutput;

	vector<vector<int>> TimedOutput = {{}};

	bool IsDelay = false;

	int GotCalled = 0;
	int GotTriggered = 0;
	int ObjListSize = 0;

	int TriggerGateNum = NULL;

	LogicGate(string name) {
		Name = name;
	}

	void call() {

		ObjListSize = (int)this->ObjList.size();

		this->ObjNames.clear();
		this->Output.clear();
		this->CalculatedOutput.clear();
		this->CalculatedInput.clear();
		this->CalculatedInputPair = {};

		if (ObjList.size() == 0) {
			Trigger();
			return;
		}
		Called();
	}

	void clear() { //It requires another clear function to clear out the ClearFuncCache... TODO!!
		this->Output = {};
		this->ObjReceiverList.clear();
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->ClearFuncCache.push_back(this);
			for (int j = 0; j < ObjList.size(); j++) {
				ObjList[i]->ClearFuncCache.push_back(ObjList[j]);
			}
			for (int j = 0; j < ClearFuncCache.size(); j++) {
				ObjList[i]->ClearFuncCache.push_back(ClearFuncCache[j]);
			}
		}
		for (int j = 0; j < ClearFuncCache.size(); j++) {
			for (int i = 0; i < ObjList.size(); i++) {
				if (ClearFuncCache[j] == ObjList[i] && this->FundamentalGateName != "DELAY") {
					if (DebugModeLevel > 0)cout << "Warning: Circular Dependency" << endl;
					return;
				}
				else if (ClearFuncCache[j] == ObjList[i] && this->FundamentalGateName == "DELAY") {
					if (DebugModeLevel > 1)cout << "Hit a Delay node, stopped searching" << endl;
					return;
				}
			}
		}
		if (DebugModeLevel > 3) {
			cout << this->Name << "'s ObjList:";
			for (int i = 0; i < this->ObjList.size(); i++) {
				cout << this->ObjList[i]->Name << ", ";
			}cout << endl;
			cout << this->Name << "'s ClearFuncCache:";
			for (int i = 0; i < this->ClearFuncCache.size(); i++) {
				cout << this->ClearFuncCache[i]->Name << ", ";
			}cout << endl;
			cout << "=============" << endl;
		}
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->clear();
			ObjList[i]->ClearFuncCache_MopUp = {}; //to allow clearMopUp to be called next time
			ObjList[i]->Output = {};
			ObjList[i]->Input = {2,2};
			ObjList[i]->CalculatedInput = {};
			ObjList[i]->CalculatedInputPair = {};
			ObjList[i]->ObjReceiverList.clear(); if (DebugModeLevel > 3)cout << ObjList[i]->Name << " is cleared" << endl;
			if (DebugModeLevel > 3)cout << this->Name << " is cleared(this)" << endl;
		}
	}

	void clearMopUp() { //It's a function to mop up the function clear() by clearing the ClearFuncCache variable
		this->Output = {};
		this->ObjReceiverList.clear();
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->ClearFuncCache_MopUp.push_back(this);
			for (int j = 0; j < ObjList.size(); j++) {
				ObjList[i]->ClearFuncCache_MopUp.push_back(ObjList[j]);
			}
			for (int j = 0; j < ClearFuncCache_MopUp.size(); j++) {
				ObjList[i]->ClearFuncCache_MopUp.push_back(ClearFuncCache_MopUp[j]);
			}
		}
		for (int j = 0; j < ClearFuncCache_MopUp.size(); j++) {
			for (int i = 0; i < ObjList.size(); i++) {
				if (ClearFuncCache_MopUp[j] == ObjList[i] && this->FundamentalGateName != "DELAY") {
					if (DebugModeLevel > 0)cout << "Warning: Circular Dependency" << endl;
					return;
				}
				else if (ClearFuncCache_MopUp[j] == ObjList[i] && this->FundamentalGateName == "DELAY") {
					if (DebugModeLevel > 1)cout << "Hit a Delay node, stopped searching" << endl;
					return;
				}
			}
		}
		if (DebugModeLevel > 3) {
			cout << this->Name << "'s ObjList:";
			for (int i = 0; i < this->ObjList.size(); i++) {
				cout << this->ObjList[i]->Name << ", ";
			}cout << endl;
			cout << this->Name << "'s ClearFuncCache_MopUp:";
			for (int i = 0; i < this->ClearFuncCache_MopUp.size(); i++) {
				cout << this->ClearFuncCache_MopUp[i]->Name << ", ";
			}cout << endl;
			cout << "=============" << endl;
		}
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->clearMopUp();
			ObjList[i]->ClearFuncCache = {}; //to allow clear() to be called next time
			ObjList[i]->Output = {};
			ObjList[i]->Input = { 2,2 };
			ObjList[i]->CalculatedInput = {};
			ObjList[i]->CalculatedInputPair = {};
			ObjList[i]->ObjReceiverList.clear(); if (DebugModeLevel > 3)cout << ObjList[i]->Name << " is cleared" << endl;
			if (DebugModeLevel > 3)cout << this->Name << " is cleared(this)" << endl;
		}
	}

	void assignFunctions(int inputgates) {
		TriggerGateNum = inputgates;
		switch (inputgates)
		{
		case 1:
			TempFunc = &LogicGate::YES;
			FundamentalGateName = "YES";
			IsDelay = false;
			NumOfInputs = 1;
			break;
		case 2:
			TempFunc = &LogicGate::NOT;
			FundamentalGateName = "NOT";
			IsDelay = false;
			NumOfInputs = 1;
			break;
		case 3:
			TempFunc = &LogicGate::AND;
			FundamentalGateName = "AND";
			IsDelay = false;
			NumOfInputs = 2;
			break;
		case 4:
			TempFunc = &LogicGate::OR;
			FundamentalGateName = "OR";
			IsDelay = false;
			NumOfInputs = 2;
			break;
		case 5:
			TempFunc = &LogicGate::DELAY;
			FundamentalGateName = "DELAY";
			IsDelay = true;
			NumOfInputs = 1;
			break;
		default:
			cout << "WARNING: Wrong Function Assigned" << endl;
			TempFunc = &LogicGate::YES;
			FundamentalGateName = "WRONG GATE";
			IsDelay = false;
			break;//Nothing...
		}
	}

	void Trigger() { //to calculate using fundamental gates
		GotTriggered++;

		this->Output.push_back((this->*TempFunc)(this->Input));
	}

	void Called() { //to summarize calculations from lower objects
		GotCalled++;

		vector<LogicGate*> RecurObj; vector<LogicGate*> NotRecurObj; //Obj that repeats (Don't call at the end), vice versa.

		for (int i = 0; i < ObjList.size(); i++) {
			for (int j = 0; j < this->ObjReceiverList.size(); j++) {//Check if the objects to call exist in the ObjReceiverList, if so, DON'T CALL
				if (this->ObjList[i]->Name == this->ObjReceiverList[j]->Name) {
					if(DebugModeLevel>0)cout << "Warning: Circular Dependency - exterminating this call from object " << this->Name << " when calling (retrieving data from) " << this->ObjReceiverList[j]->Name << endl;
					RecurObj.push_back(this->ObjList[i]);
				}
			}
		}
		
		for (int i = 0; i < ObjList.size(); i++) {

			ObjList[i]->ObjTargetList.push_back(this);//PUSH this object into the receiver's ObjTargetList
			
			for (int j = 0; j < RecurObj.size(); j++) {
				if (RecurObj[j] == ObjList[i])goto skip; //Put elements from ObjList to NotRecurObj, EXCEPT the objects that also appears from RecurObj.
			}
			NotRecurObj.push_back(ObjList[i]);
		skip:;
		}

		for (int i = 0; i < NotRecurObj.size(); i++) {
			if(DebugModeLevel>3)cout << "NOT REPEATED OBJs: " << NotRecurObj[i]->Name << ", ";
		}if(DebugModeLevel>3)cout << endl;

		for (int i = 0; i < NotRecurObj.size(); i++) {

			//For every object you call, you give that object yourself and the objects that you call.
			NotRecurObj[i]->ObjReceiverList.push_back(this); //passing itself(this) to that targeting object
			for (int j = 0; j < NotRecurObj.size(); j++) { //passing objects from this->ObjList to that targeting object
				NotRecurObj[i]->ObjReceiverList.push_back(NotRecurObj[j]);
			}
			for (int j = 0; j < this->ObjReceiverList.size(); j++) {//passing objects from this->ObjReceiverList to that targeting object
				NotRecurObj[i]->ObjReceiverList.push_back(this->ObjReceiverList[j]);
			}
			if (DebugModeLevel>3)cout << "CALL!?" << endl;
			NotRecurObj[i]->call(); //Recursive calling into the hierarchy

			this->ObjNames.push_back(NotRecurObj[i]->Name);//Get ObjNames down from the hierarchy, put into this-> ObjNames array.

			for (int j = 0; j < NotRecurObj[i]->Output.size(); j++) {//Get Fundamental Gates result from their Output down from the hierarchy, put into this-> Output array
				this->Output.push_back(NotRecurObj[i]->Output[j]);
			}
		}

		if (TriggerGateNum != NULL) { //When the intermiediate object is assigned a gate function

			for (int i = 0; i < this->Output.size(); i++) {
				CalculatedInput.push_back(this->Output[i]);
			}
			//cout << "Call Calculating, CalculatedInput.size(): " << CalculatedInput.size() << endl;
			if (CalculatedInput.size() == 1) {
				CalculatedInputPair.first = CalculatedInput[0];
				CalculatedInputPair.second = 2;
				this->CalculatedOutput.push_back((this->*TempFunc)(this->CalculatedInputPair));
			}
			if (CalculatedInput.size() >= 2) {
				CalculatedInputPair.first = CalculatedInput[0];
				CalculatedInputPair.second = CalculatedInput[1];
				this->CalculatedOutput.push_back((this->*TempFunc)(this->CalculatedInputPair));
			}
			

			if (Input.second == 2 && Input.first == 2) {//if the object has Manual Input, ignore the gate output
				Output = CalculatedOutput;
			}
			else {
				Output.clear();
				Output.push_back(Input.first);
				Output.push_back(Input.second);
			}
		}

	}

	void input(int first) { //for NOT and YES gates
		Input.first = first;
		Input.second = 2;
	}
	void input(int first, int second) { //for AND and OR or maybe more in the future for performance purposes
		Input.first = first;
		Input.second = second;
	}
	void input(vector<LogicGate*> objList) { //Complex Object Gate
		ObjList = objList;
		this->ObjListSize = (int)this->ObjList.size();
	}

	void print() {
		cout << Name; if (FundamentalGateName != "") { cout << " (" << FundamentalGateName << ")"; } cout << " // Number of Inputs: " << NumOfInputs << " // Trigger Gate Number: " << TriggerGateNum << endl;
		cout << Name << " Got Called: " << GotCalled << " // Object List Size: " << ObjListSize << endl;
		cout << Name << " Got Triggered: " << GotTriggered << endl;
		if (Input.first != 2 && CalculatedInputPair.first != 2) {
			if (CalculatedInputPair.second == 2) {
				cout << "Input from Obj(ignored): " << CalculatedInputPair.first << endl;
			}
			else {
				cout << "Input from Obj(ignored): " << CalculatedInputPair.first << " , " << CalculatedInputPair.second << endl;
			}
			if (Input.second == 2) {
				cout << "Input(Manual Input): " << Input.first << endl;
			}
			else {
				cout << "Input(Manual Input): " << Input.first << " , " << Input.second << endl;
			}
		}
		else if (Input.first == 2) {
			if (CalculatedInputPair.first == 2) {
				cout << "Input: N/A" << endl;
			}
			else if (CalculatedInputPair.second == 2) {
				cout << "Input from Obj: " << CalculatedInputPair.first << endl;
			}
			else {
				cout << "Input from Obj: " << CalculatedInputPair.first << " , " << CalculatedInputPair.second << endl;
			}
		}
		else if (Input.second == 2) {
			cout << "Input: " << Input.first << endl;
		}
		else {
			cout << "Input: " << Input.first << " , " << Input.second << endl;
		}
		cout << "Output: ";
		for (int i = 0; i < Output.size(); i++) {
			cout << Output[i] << " ";
		}
		cout << endl;
		cout << "ObjList.size() = " << this->ObjList.size() << " Object Names: ";
		for (int i = 0; i < this->ObjList.size(); i++) {
			cout << ObjList[i]->Name << " ";
		}cout << endl;
		cout << "ObjTargetList.size() = " << this->ObjTargetList.size() << " Object Names: ";
		for (int i = 0; i < this->ObjTargetList.size(); i++) {
			cout << ObjTargetList[i]->Name << " ";
		}
		cout << endl << "==============" << endl;
	}

	//TODO Search from DELAY node, ends at beginning or at DELAY nodes.
	//TODO clear the ObjList from ALL DELAY nodes (cut the tie)
	void recurDelayBranches(ObjectStore* Branches) {
		for (int i = 0; i < Branches->ObjTree.size(); i++) {//Stop at Delay node or at the beginning or at itself
			if (Branches->ObjTree[i] == this) {
				//Branches->ObjTree.push_back(this);
				return;
			}
			else if (this->FundamentalGateName == "DELAY") {
				//Branches->ObjTree.push_back(this);
				return;
			}
		}
		Branches->ObjTree.push_back(this);
		for (int i = 0; i < ObjList.size(); i++) {
			ObjList[i]->recurDelayBranches(Branches);
		}
	}

	//Need to clear ObjReceiverList() before calling it, in either sub-class or driver.
	void activate() {

		vector<LogicGate*> RecurObj; vector<LogicGate*> NotRecurObj; //Obj that repeats (Don't call at the end), vice versa.

		cout << this->Name << "Activating..." << endl;

		for (int i = 0; i < ObjTargetList.size(); i++) {
			for (int j = 0; j < this->ObjReceiverList.size(); j++) {//Check if the objects to call exist in the ObjReceiverList, if so, DON'T CALL
				if (this->ObjTargetList[i]->Name == this->ObjReceiverList[j]->Name) {
					if (DebugModeLevel > 0)cout << "Warning: Circular Dependency - exterminating this call from object " << this->Name << " when trying to activate " << this->ObjReceiverList[j]->Name << endl;
					return;
				}
			}
		}

		if (DebugModeLevel > 3)cout << this->Name << ", ObjTargetList: ";
		for (int i = 0; i < ObjTargetList.size(); i++) {
			if (DebugModeLevel > 3)cout << " " << ObjTargetList[i]->Name;
		}cout << endl;
		if (DebugModeLevel > 3)cout << this->Name << ", ObjReceiverList: ";
		for (int i = 0; i < ObjReceiverList.size(); i++) {
			if (DebugModeLevel > 3)cout << " " << ObjReceiverList[i]->Name;
		}cout << endl;

		//=======================
		for (int i = 0; i < ObjTargetList.size(); i++) {
			ObjTargetList[i]->ObjReceiverList.push_back(this);
			for (int j = 0; j < ObjTargetList.size(); j++) {
				ObjTargetList[i]->ObjReceiverList.push_back(this->ObjTargetList[j]);
			}
			for (int j = 0; j < ObjReceiverList.size(); j++) {
				ObjTargetList[i]->ObjReceiverList.push_back(this->ObjReceiverList[j]);
			}
		}
		//=======================
		for (int i = 0; i < ObjTargetList.size(); i++) {
			ObjTargetList[i]->activate();
		}

		

		//store the result into StoredTickResults.
	}
};

class RunGate{
private:

	vector<LogicGate*> InputList; //Gates to input from (from current obj)
	vector<LogicGate*> OutputList; //Gates to output to (from current obj)

public:

	RunGate(vector<LogicGate*> inputList, vector<LogicGate*> outputList) {
		InputList = inputList;
		OutputList = outputList;
	}



};

class SplitGate :LogicGate {
private:

	LogicGate* Obj;

	vector<LogicGate*> CurrentObjArr;

	vector<LogicGate*> DelayNodeArr;
	vector<LogicGate*> InputNodeArr;

	vector<vector<LogicGate*>> OverallObjArr;

	vector<vector<LogicGate*>> DelayNodeBranches;

public:
	SplitGate(LogicGate* ObjGate) :LogicGate(*ObjGate) {
		Obj = ObjGate;
		cout << "ObjGate.ObjListSize = " << ObjGate->ObjListSize << endl;

		ObjectStore DelayNodes;
		this->findSpecialNodes(&DelayNodes);
		DelayNodeArr = DelayNodes.DelayNodeArr;
		InputNodeArr = DelayNodes.InputObjArr;

		
		ObjectStore DelayBranch;

		this->recurDelayBranches(&DelayBranch);//From the end
		DelayNodeBranches.push_back(DelayBranch.ObjTree);
		DelayBranch.ObjTree.clear();
		
		for (int i = 0; i < DelayNodeArr.size(); i++) { //From DELAY nodes.
			DelayNodeArr[i]->recurDelayBranches(&DelayBranch);
			DelayNodeBranches.push_back(DelayBranch.ObjTree);
			DelayBranch.ObjTree.clear();
		}
		
		//TODO while loop to generate all branches from all DELAY nodes and also from the end.

		
	}

	void printSplits() {
		cout << "All Splits: (" << DelayNodeBranches.size() << " splits by Delay Node/s)" << endl << "{" << endl;;

		for (int i = 0; i < DelayNodeBranches.size(); i++) {
			cout << "	{" << endl;
			for (int j = 0; j < DelayNodeBranches[i].size(); j++) {
				cout << "		" << i << "." << j << ": " << DelayNodeBranches[i][j]->Name << "-" << DelayNodeBranches[i][j]->FundamentalGateName << endl;
			}
			cout << "	}" << endl;
		}
		cout << "}" << endl;
	}

	void printSpecialNodes() {
		cout << "All Delay Nodes: ";
		for (int i = 0; i < DelayNodeArr.size(); i++) {
			cout << DelayNodeArr[i]->Name << " ~ ";
		}cout << endl;
		cout << "All Input Nodes: ";
		for (int i = 0; i < InputNodeArr.size(); i++) {
			cout << InputNodeArr[i]->Name << " ~ ";
		}cout << endl;
	}

};

class UseGate :LogicGate { //Only for non recursive repetitive use (truth table) //OR PRINTING
private:
	LogicGate* Obj;

	ObjectStore objtree;

public:

	vector<LogicGate*> CurrentObjArr;

	UseGate(LogicGate* ObjGate) :LogicGate(*ObjGate) {
		Obj = ObjGate;
		//cout << "ObjGate.ObjListSize = " << ObjGate.ObjListSize << endl;

		this->recurObjFunc(&objtree);
		CurrentObjArr = objtree.ObjTree;
	}

	void call() {//reverse activate the list to make it available for future use.
		Obj->call();
	}

	void activate(vector<LogicGate*> Origins) {//Use the list after call.
		for (int i = 0; i < Origins.size(); i++) {
			Origins[i]->activate();
		}
	}

	void toggleDebugMode_Connected(unsigned Mode) {
		objtree.clear();
		Obj->DebugModeLevel = Mode;
		this->toggleDebugMode(&objtree, Mode);
	}

	//Make a Function to not go through the hierarchy normally, and detect circular dependancies.

	void checkNumOfInputs() {
		int Counter = 0;
		for (int i = 0; i < CurrentObjArr.size(); i++) {
			if (CurrentObjArr[i]->ObjListSize == 0) {
				switch (CurrentObjArr[i]->TriggerGateNum) {
				case 1: //YES
					Counter++;
					break;
				case 2: //NOT
					Counter++;
					break;
				case 3: //AND
					Counter = Counter + 2;
					break;
				case 4: //OR
					Counter = Counter + 2;
					break;
				case 5: //DELAY
					Counter++;
					break;
				default:
					break;
				}
			}
		}
		cout << "REMINDER: You should input " << Counter << " number of inputs." << endl;
	}

	void inputAll(vector<int> inputBool) {
		int Counter = 0;
		for (int i = 0; i < CurrentObjArr.size(); i++) {
			if (Counter >= inputBool.size()) {
				cout << "TOO FEW INPUTS PROVIDED (UseGate:LogicGate void inputAll)" << endl;
				checkNumOfInputs();
				return;
			}
			if (CurrentObjArr[i]->ObjListSize == 0) {
				switch (CurrentObjArr[i]->TriggerGateNum) {
				case 1: //YES
					CurrentObjArr[i]->input(inputBool[Counter]);
					Counter++;
					break;
				case 2: //NOT
					CurrentObjArr[i]->input(inputBool[Counter]);
					Counter++;
					break;
				case 3: //AND
					CurrentObjArr[i]->input(inputBool[Counter], inputBool[Counter + 1]);
					Counter = Counter + 2;
					break;
				case 4: //OR
					CurrentObjArr[i]->input(inputBool[Counter], inputBool[Counter + 1]);
					Counter = Counter + 2;
					break;
				case 5: //DELAY
					CurrentObjArr[i]->input(inputBool[Counter]);
					Counter++;
					break;
				default:
					cout << "Wrong Gate Number (UseGate:LogicGate void inputAll)" << endl;
					break;
				}
			}
			if (Counter < inputBool.size()) {
				cout << "TOO MANY INPUTS PROVIDED (UseGate:LogicGate void inputAll)" << endl;
				cout << inputBool.size() - Counter << " number of input/s is/are neglected." << endl;
				checkNumOfInputs();
				return;
			}
		}
	}

	void printAll() {
		cout << "RESULT: ";
		for (int i = 0; i < Obj->CalculatedOutput.size(); i++) {
			cout << Obj->CalculatedOutput[i] << " | ";
		}cout << endl << "---------------" << endl;
		Obj->print();
		for (int i = 1; i < CurrentObjArr.size(); i++) {
			CurrentObjArr[i]->print();
		}
		cout << "=================================" << endl;
		cout << endl << "INPUT ORDER: " << endl;
		for (int i = 0; i < CurrentObjArr.size(); i++) {
			if (CurrentObjArr[i]->ObjListSize == 0) {
				cout << CurrentObjArr[i]->Name << "(" << CurrentObjArr[i]->FundamentalGateName << "), ";
			}
		}
		cout << endl << "=================================" << endl;
	}

	void printResult() {
		cout << "RESULT: ";
		for (int i = 0; i < Obj->CalculatedOutput.size(); i++) {
			cout << Obj->CalculatedOutput[i] << " | ";
		}
	}

	vector<int> getResult() {
		return Obj->CalculatedOutput;
	}

	void clear() {
		//Obj->clear();
		//Obj->clearMopUp();
		for (int i = 0; i < CurrentObjArr.size(); i++) {
			CurrentObjArr[i]->Output.clear();
			CurrentObjArr[i]->Input = { 2,2 };
			CurrentObjArr[i]->CalculatedInputPair = { 2,2 };
			CurrentObjArr[i]->Output.clear();
			CurrentObjArr[i]->ObjReceiverList.clear();
			
			if (DebugModeLevel > 3)cout << CurrentObjArr[i]->Name << " is cleared" << endl;
		}
	}

	string getName() {
		return this->Name;
	}
};