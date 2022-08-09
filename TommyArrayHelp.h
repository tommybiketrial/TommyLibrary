#pragma once

using std::rand;

namespace RandomArray {
	vector<int> generateRandomVectorArray(int min, int max, int NumberOfNumbers) {
		vector<int> Output;
		for (int i = 0; i < NumberOfNumbers; i++) {
			Output.push_back((rand()%(max-min)+min));
		}

		return Output;
	}

	int* generateRandomIntArray(int min, int max, int NumberOfNumbers) {
		
		int* Output = new int[NumberOfNumbers];
		
		for (int i = 0; i < NumberOfNumbers; i++) {
			Output[i] = ((rand() % (max - min) + min));
		}

		return Output;
	}
}

class TommyArray {
private:

	void delete2DArrayInt(int** Input, int sizeX) {
		for (int i = 0; i < sizeX; ++i) {
			delete[] Input[i];
		}
		delete[] Input;
	}

	int** dimension1to2(int* Input, int InputSize, int RowSize) {

		int** Output = new int* [InputSize / RowSize];
		for (int i = 0; i < InputSize / RowSize; ++i) {
			Output[i] = new int[RowSize];
		}

		for (int i = 0; i < RowSize; i++) {

		}

		return 0;
	}

public:

	int Size1D;
	int Size2DX;
	int Size2DY;

	int* Arr1D;
	int** Arr2D;

	TommyArray(int Size) {
		int* Array1D = new int[Size];
	}

	void printVectorInt(vector<int> Input) {
		for (int i = 0; i < Input.size(); i++) {
			cout << Input[i] << ", ";
		}
		cout << endl;
	}

	void printArrayInt(int* Input, int Size) {
		for (int i = 0; i < Size; i++) {
			cout << Input[i] << ", ";
		}
		cout << endl;
	}
};

/* //Random Array Int

	TommyArray test(500);

	test.printArrayInt(RandomArray::generateRandomIntArray(10, 100, 500),500);
*/