#pragma once

class CircularDependencyTest { //called by a worker thread
private:

	int Input = 0;

	int CurrentTick = 0;

	int A = 0, B = 0, C = 0;

	vector<int> StoreArrA;
	vector<int> StoreArrB;
	vector<int> StoreArrC;

	void funcA() {
		A = B;
		StoreArrA.push_back(A);
	}

	void funcB() {
		B = C;
		StoreArrB.push_back(B);
	}

	void funcC() {

		//delay or wait for instruction to execute (ex. from the main thread)

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		C = A+2;

		StoreArrC.push_back(C);
	}

public:

	bool IsRunning = true;

	CircularDependencyTest(int input, int ticks) {
		Input = input;
		if (ticks == 0) {
			CurrentTick = -9;
		}
		else {
			CurrentTick = ticks;
		}
		
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