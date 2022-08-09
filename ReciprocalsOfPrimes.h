#pragma once

//https://www.youtube.com/watch?v=DmfxIhmGPP4&t=105s

class Reciprocals_Of_Prime {
private:

    int The_Divisor = 0;

    int NumOfInputDigits = 0;

    int Dividend = 0;

    vector<int> Remainders;

    vector<int> Quotient;

    int countDigitsInInteger(int n)
    {
        int count = 0;
        while (n > 0)
        {
            count++;
            n = n / 10;
        }
        return count;
    }

public:

    Reciprocals_Of_Prime(int Input) {
        The_Divisor = Input;
    }

    void calculate() {

        int tmp_quotient = 0;

        NumOfInputDigits = countDigitsInInteger(The_Divisor);

        Dividend = pow(10, NumOfInputDigits);
        for (int i = 1; i < NumOfInputDigits; i++) {
            Quotient.push_back(0);
        }

        tmp_quotient = Dividend / The_Divisor;

        //cout << " - " << tmp_quotient;

        Remainders.push_back(Dividend - (tmp_quotient * The_Divisor));
        Quotient.push_back(tmp_quotient);

        //cout << " - " << Remainders[0] << endl;


        //================
        int i = 0;
        int tmp_remainder;
        bool isUnique = true;
        while(isUnique) {

            tmp_quotient = Remainders[i] * 10 / The_Divisor;
            //cout << Remainders[i] * 10 << "(Remainder*10) / " << The_Divisor << "(Divisor)" << endl;

            tmp_remainder = (Remainders[i] * 10) - (tmp_quotient * The_Divisor);

            if (std::find(Remainders.begin(), Remainders.end(), tmp_remainder) != Remainders.end()) {
                isUnique = false;
            }

            Remainders.push_back(tmp_remainder);
            Quotient.push_back(tmp_quotient);
            //cout << Remainders[i] * 10 << "(Remainder*10) - (" << tmp_quotient << " * " << The_Divisor << ")" << endl;

            //cout << " -quotient digit - " << tmp_quotient;
            //cout << " -   remainder   - " << Remainders[i+1] << endl;
            
            i++;
        }
    }

    void printCalculations() {
        cout << "Quotient:" << endl;
        cout << "0.";
        for (int i = 0; i < Quotient.size(); i++) {
            cout << Quotient[i];
        }cout << endl << endl;
        for (int i = 0; i < Remainders.size(); i++) {
            cout << "Remainder(" << i << ") = " << Remainders[i] << endl;
        }

    }

    void printDetails() {
        cout << "First Repeated Remainder: " << Remainders[Remainders.size()-1] << endl;
        cout << "Range of Recurring Decimals: " << Remainders.size()-1 << endl;

    }
};

/*// Driver Code for Reciprocals_Of_Prime
* 
    Reciprocals_Of_Prime test = 23;
	test.calculate();
	test.printDetails();
	cout << endl;
	test.printCalculations();
*/

class Integer_To_Digit_Array {
private:

    vector<int> The_Number;


    int countDigitsInInteger(int n)
    {
        int count = 0;
        while (n > 0)
        {
            count++;
            n = n / 10;
        }
        return count;
    }

public:
    
    Integer_To_Digit_Array(int Input) {
        int intLength = countDigitsInInteger(Input);
        //break apart the integer into digits
        int digit;
        vector<int> tmp;
        while (Input > 0)
        {
            digit = Input % 10;
            Input = Input / 10;
            tmp.push_back(digit);
        }
        for (int i = tmp.size() - 1; i >= 0; i--) {
            The_Number.push_back(tmp[i]);
        }
    }

    int getSumOfDigits() {
        int tmp = 0;
        for (int i = 0; i < The_Number.size(); i++) {
            tmp += The_Number[i];
        }
        return tmp;
    }

    void printInput() {
        for (int i = 0; i < The_Number.size(); i++) {
            cout << The_Number[i];
        }cout << endl;
    }
};