#pragma once

class RegexWrapper {
private:

    std::regex The_Regex;

    char* Input_Char = {};

    std::cmatch match;

public:

    RegexWrapper(){}
    RegexWrapper(std::regex input) { //enter regex here
        The_Regex = input;
    }
    RegexWrapper(char* input_Char) { //enter regex here
        Input_Char = input_Char;
    }
    RegexWrapper(std::regex input, const char* input_Char) { //enter regex here
        The_Regex = input;
        Input_Char = (char*)input_Char;
    }

    void match_regex() {
        std::regex_match(Input_Char, match, The_Regex);
    }
    void match_regex(const char* Input_Char) {
        std::regex_match(Input_Char, match, The_Regex);
    }

    void print_regex_matches()
    {
        std::cout << "Matches:\n";

        for (std::size_t i = 0; i < match.size(); ++i)
        {
            std::cout << i << ": " << match.str(i) << '\n';
        }
    }

    std::cmatch& getMatch() {
        return match;
    }

};

/*// RegexWrapper

    std::regex input_regex(R"((\d+): \[([^\]]+)\] (.*))");

    RegexWrapper test(input_regex, "123: [error] Hello World");
    test.match_regex();
    test.print_regex_matches();
*/

/*
    while (true) {
        string inputregex, inputstr;
        cout << "input regex: ";
        getline(cin, inputregex);
        cout << endl << inputregex << endl;
        std::regex input_regex(inputregex);
        cout << endl;
        cout << "input the string: ";
        getline(cin, inputstr);
        const char* input = inputstr.c_str();
        RegexWrapper test(input_regex, input);
        cout << endl;
        for (int i = 0; i < inputstr.size(); i++) {
            cout << input[i];
        }cout << endl;
        test.match_regex();
        test.print_regex_matches();
        cout << endl << "==========" << endl;
    }
*/