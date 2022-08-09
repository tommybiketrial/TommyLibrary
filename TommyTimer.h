#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <ctime>
#include <iostream>

class TommyTimer
{
public:
	TommyTimer(bool TimePoint);
	void checkTime();
	void PrintTimeStampOfTheEnd();

	double long ElapsedTime;

private:
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;

	std::time_t end_time;

	bool isTimePoint;
};

TommyTimer::TommyTimer(bool TimePoint)
{
	start = std::chrono::system_clock::now();
	isTimePoint = TimePoint;
}

void TommyTimer::checkTime() {
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	ElapsedTime = elapsed_seconds.count();
	if (isTimePoint){ 
		end_time = std::chrono::system_clock::to_time_t(end);
	}
}

void TommyTimer::PrintTimeStampOfTheEnd() {
	printf(std::ctime(&end_time));
}

//=======================================================================

class TommyTimer2 {
private:
	std::chrono::steady_clock::time_point begin;
	int time = 0;
	bool print = 1;
public:
	/*Boolean: Whether you want time printed from checkTime() or not*/
	TommyTimer2(bool input);

	int checkTime();
};

TommyTimer2::TommyTimer2(bool input) {
	begin = std::chrono::high_resolution_clock::now();
	print = input;
}

int TommyTimer2::checkTime() {
	auto end = std::chrono::high_resolution_clock::now();
	time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

	if (print == 1) {
		std::cout << "ns: " << time << std::endl;
		std::cout << "µs: " << time / 1000 << std::endl;
		std::cout << "ms: " << time / 1000000 << std::endl;
		std::cout << "s: " << time / 1000000000 << std::endl;
	}

	return time;
}

//====================================================================

class DelayTimer {
public:

	bool IsRunning = 1;

	int TotalTicks = 50;

	int TickType = 1;
	int TickInterval = 500;

	int Input1 = 0, Input2 = 0;

	int Result = 0;

	vector<int> ResultArr;

	DelayTimer(int Max_Ticks, int Tick_Type, int Tick_Interval) {
		TotalTicks = Max_Ticks;
		TickType = Tick_Type;
		TickInterval = Tick_Interval;

	}

	void input(int input1, int input2) {
		Input1 = input1;
		Input2 = input2;
	}

	void foo()
	{
		std::cout << "Hello !\n";
	}
	/*
	auto chooseTickType() {
		switch (TickType) {
		case 0:
			auto TickTypeFunc = std::chrono::microseconds();
			return TickTypeFunc;
		case 1:
			auto TickTypeFunc = std::chrono::milliseconds();
			return TickTypeFunc;
		case 2:
			auto TickTypeFunc = std::chrono::seconds();
			return TickTypeFunc;
		}
	}
	*/
	void test()
	{
		std::thread([&]
		{
			while (IsRunning && TotalTicks > 0)
			{
				if (TickType == 0) {
					std::this_thread::sleep_for(std::chrono::microseconds(TickInterval));
				}else if (TickType == 1) {
					std::this_thread::sleep_for(std::chrono::milliseconds(TickInterval));
				}
				else {
					std::this_thread::sleep_for(std::chrono::seconds(TickInterval));
				}
				// Call our method
				foo();
				TotalTicks--;
			}
		}).detach();

		// Execution continues ...
		//std::cin.get();
	}

	void invoke(unsigned long long (*func)(unsigned long long)) {
		std::thread([&]
		{
			while (IsRunning && TotalTicks > 0)
			{

				ResultArr.push_back(func(Input1));

				foo();

				std::this_thread::sleep_for(std::chrono::milliseconds(TickInterval));

				Input1++;

				TotalTicks--;
			}
		}).detach();

		

		IsRunning = false;
	}

	void invoke(int (*func)(int, int)) {
		std::thread([&]
		{
			while (IsRunning && TotalTicks > 0)
			{
					
				Result = func(Input1,Input2);

				foo();
				TotalTicks--;
			}
		}).detach();
	}
	
	vector<int> getResultArr() {
		return ResultArr;
	}

	int getResult() {
		return Result;
	}

	void stop() {
		IsRunning = 0;
	}


};

/* //DelayTimer Class
	DelayTimer obj(20);

	obj.input(20, 0);
	obj.invoke(&Fibonacci::fibRecur);
	cout << obj.getResult() << endl;

	std::cin.get();
	std::cin.get();
*/

/* //Thread Timer

	int input = 0;

    DelayTimer testing(50,1,500);
    testing.test();

    while (input != 1) {
        cin >> input;
        if (input == 1) {
            testing.stop();
        }
    }

    std::cin.get();
    std::cin.get();

*/