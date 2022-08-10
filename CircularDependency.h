#pragma once

class CircularDependencyTest { //called by a worker thread
private:

	int A = -1, B = -1, C = -1;

	vector<int> StoreArrA;
	vector<int> StoreArrB;
	vector<int> StoreArrC;

	void funcA() {
		if (B == -1) {
			A = -1;
		}
		A = B;
		StoreArrA.push_back(A);
	}

	void funcB() {
		if (C == -1) {
			B = -1;
		}
		B = C;
		StoreArrB.push_back(B);
	}

	void funcC() {

		//delay or wait for instruction to execute (ex. from the main thread)

		std::this_thread::sleep_for(std::chrono::milliseconds(DelayMilliseconds));

		C = A+2;

		StoreArrC.push_back(C);
	}

public:

	int DelayMilliseconds = 100;

	int CurrentTick = 0;

	bool IsRunning = true;

	CircularDependencyTest(int input, int ticks, int DelayDuration) {
		B = input;
		if (ticks == 0) {
			CurrentTick = -9;
		}
		else {
			CurrentTick = ticks;
		}
		
		DelayMilliseconds = DelayDuration;
	}

	void run() {
		while (IsRunning && (CurrentTick > 0 || CurrentTick == -9)) { //wait for instruction to kill the process(ex.from the main thread)
			funcA();
			funcB();
			funcC();
			if (CurrentTick != -9) {
				CurrentTick--;
			}
		}
		IsRunning = false;
	}

	void printAllResults() { //you can call it from either threads
		cout << endl << "========= A =========" << endl;
		for (int i = 0; i < StoreArrA.size(); i++) {
			cout << "Tick " << i << ": " << StoreArrA[i] << endl;
		}
		cout << endl << "========= B =========" << endl;
		for (int i = 0; i < StoreArrB.size(); i++) {
			cout << "Tick " << i << ": " << StoreArrB[i] << endl;
		}
		cout << endl << "========= C =========" << endl;
		for (int i = 0; i < StoreArrC.size(); i++) {
			cout << "Tick " << i << ": " << StoreArrC[i] << endl;
		}
	}
};

/*//Driver Code:

	CircularDependencyTest test(10, 0);

    std::thread worker(&CircularDependencyTest::run, std::ref(test));

    while (test.IsRunning) {
        auto input = 1;
        cin >> input;
        if (input == 0) {
            test.IsRunning = false;
        }
    }

    worker.join();

    test.printAllResults();

*/

class CircularDependencyObj { //Use OBJ here! with Delay and TICK!
private:
	int CurrentTick = 0;
	int DelayTick = 0;
	vector<int> StoreArr;
	bool IsRunning = true;
	int DelayInMilliseconds = 100;

	CircularDependencyObj* obj;
public:

	string Name = "";

	int Value = 0;

	CircularDependencyObj(int input, int StartingTick, int delayInMilliseconds, string name) {
		Name = name;
		Value = input;
		if (StartingTick == 0) {
			CurrentTick = -9;
		}
		else {
			CurrentTick = StartingTick;
		}
		DelayInMilliseconds = delayInMilliseconds;
	}

	void inputObj(CircularDependencyObj* input) {
		obj = input;
	}

	void run() {
		if (CurrentTick == 0) {
			return;
		}
		else {
			Value++;
			cout << "Tick(" << CurrentTick << ")(" << Name << "): " << Value << endl;
			CurrentTick--;
			obj->run();
		}
	}
};