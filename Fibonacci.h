#pragma once
#include <map>

namespace Fibonacci {
	//Function(n) = Function(n-1) + Function(n-2)
	unsigned long long fibLoop(int iteration) {
		unsigned long long a = 1, b = 1, tmp = 0; 
		for (int i = 1; i <= iteration; i++) {
			b = a;
			a = b + tmp;
			tmp = b;
		}
		return a;
	}

	unsigned long long fibLoopFromInternet(int n)
	{
		int i; unsigned long long one = 0, two = 1, three;
		if (n == 0)
			return one;
		for (i = 2; i <= n; i++)
		{
			three = one + two;
			one = two;
			two = three;
		}
		return two;
	}

	unsigned long long fibRecur(unsigned long long iteration) {
		if (iteration <= 2) return 1;
		return fibRecur(iteration - 1) + fibRecur(iteration - 2);
	}

	unsigned long long fibRecurMemo(int n)
	{
		static std::map<unsigned long long, unsigned long long> values;
		if (n == 0 || n == 1) //BASE CASE
			return n;
		std::map<unsigned long long, unsigned long long>::iterator iter = values.find(n);
		
		if (iter == values.end())
		{
			return values[n] = fibRecurMemo(n - 1) + fibRecurMemo(n - 2);
		}
		else
		{
			return iter->second;
		}
	}
}

/*
	TommyTimer2 timer(0);
	unsigned long long result = Fibonacci::fibRecur(25);
	int time = timer.checkTime();
	std::cout << result << std::endl;
	std::cout << "Elapsed Time (ns): " << time << std::endl;
*/

/*
	TommyTimer Timer(1);
	
	unsigned long long result1 = Fibonacci::fibRecur(25);
	
	Timer.checkTime();

	std::cout << result1 << std::endl;

	std::cout << "Elapsed Time:" << Timer.ElapsedTime << std::endl;
	Timer.PrintTimeStampOfTheEnd();
*/