#pragma once

namespace exprtk_practices {
	void exprtk_practice_1() { //wrong answer
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>   expression_t;
        typedef exprtk::parser<double>       parser_t;

        double x; // Variable x
        double y; // Variable y to store the result

        // Create symbol table and add variables to it
        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_constants(); // Add constants (optional)

        // Create an expression
        expression_t expression;
        expression.register_symbol_table(symbol_table);

        // Define the polynomial equation
        std::string equation = "3 * x^2 + 2 * x + 1";

        // Parse the equation
        parser_t parser;
        if (!parser.compile(equation, expression)) {
            std::cerr << "Error in parsing equation: " << equation << std::endl;
            return;
        }

        // Assign a value to x and evaluate the expression
        x = 5; // Example value for x
        y = expression.value(); // Evaluate the expression

        // Output the result
        std::cout << "For x = " << x << ", y = " << y << std::endl;

        return;
	}

    void exprtk_practice_2() {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>   expression_t;
        typedef exprtk::parser<double>       parser_t;

        double x; // Variable x
        double y; // Variable y
        double z; // Variable z to store the result

        // Create symbol table and add variables to it
        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("y", y);
        symbol_table.add_constants();

        // Create an expression

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        // Define the equation
        std::string equation = "x^2 + y - 3"; // z = x^2 + y - 3

        // Parse the equation
        parser_t parser;
        if (!parser.compile(equation, expression)) {
            std::cerr << "Error in parsing equation: " << equation << std::endl;
            return;
        }

        // Assign values to x and y and evaluate the expression
        x = 5; // Example value for x
        y = 2; // Example value for y
        z = expression.value(); // Evaluate the expression

        // Output the result
        std::cout << "For x = " << x << " and y = " << y << ", z = " << z << std::endl;

        return;
    }

};

class LimitApproximator {
public:

    /* EXAMPLE VALUES:
        std::string equation = "y^2 + x - 3"; // Define the equation
	    double x = 5; // Example value for x
	    double y_target = 3; // Value that y is approaching
	    double initial_step = 0.5; // Initial step size
	    double curve_factor = 1.0; // Factor to adjust the step size(exponential) in each iteration
	    double step_factor = 0.05; // Factor to adjust the step size(uniform) in each iteration
	    double stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)
	    int max_iterations = 40; // Maximum iterations(once reached it finish running)
    */
    LimitApproximator(const std::string& equation, double y_target, double initial_step, double curve_factor, double step_factor, double stop_accuracy, int max_iterations = 10) {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>   expression_t;
        typedef exprtk::parser<double>       parser_t;

        double y; // Variable y
        double z; // Variable z to store the result

        // Create symbol table and add variables to it
        symbol_table_t symbol_table;
        symbol_table.add_variable("y", y);
        symbol_table.add_constants();

        // Create an expression
        expression_t expression;
        expression.register_symbol_table(symbol_table);

        // Parse the equation
        parser_t parser;
        if (!parser.compile(equation, expression)) {
            std::cerr << "Error in parsing equation: " << equation << std::endl;
            return;
        }

        // Function to generate approach values using a curve
        auto generate_approach_values = [&](double target) {
            std::vector<double> values;
            double step = initial_step;
            for (int i = 0; i <= max_iterations; ++i) {
                values.push_back(target - step);
                values.push_back(target + step);
                step /= curve_factor; // Adjust the step size based on the curve factor
                step -= step_factor;
                final_iterations = i;
                if (target + step <= 3 + stop_accuracy && target - step >= 3 - stop_accuracy) {
                    stop_accuracy_value_reached = true;
                    return values;
                }
            }
            return values;
        };

        // Generate values for y
        std::vector<double> y_values = generate_approach_values(y_target);

        // Evaluate and store results
        for (const auto& val : y_values) {
            y = val;
            z = expression.value(); //this is where it calculates
            if (y < y_target) {
                From_Left.push_back(std::make_pair(y, z));
            }
            else {
                From_Right.push_back(std::make_pair(y, z));
            }
        }
    }

    /* EXAMPLE VALUES:
        std::string equation = "y^2 + x - 3"; // Define the equation

	    double x_target = 3; // Value that x is approaching
	    double x_initial_step = 0.5; // Initial step size
	    double x_curve_factor = 1.0; // Factor to adjust the step size(exponential) in each iteration
	    double x_step_factor = 0.04; // Factor to adjust the step size(uniform) in each iteration
	    double x_stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)
	
	    double y_target = 3; // Value that y is approaching
	    double y_initial_step = 0.5; // Initial step size
	    double y_curve_factor = 1.1; // Factor to adjust the step size(exponential) in each iteration
	    double y_step_factor = 0.00; // Factor to adjust the step size(uniform) in each iteration
	    double y_stop_accuracy = 0.0001; // Stop accuracy(so the value will stop at for example 3.0001)
	
	    int max_iterations = 40; // Maximum iterations(once reached it finish running)
    */
    LimitApproximator(const std::string& equation, double x_target, double x_initial_step, double x_curve_factor, double x_step_factor, double x_stop_accuracy, double y_target, double y_initial_step, double y_curve_factor, double y_step_factor, double y_stop_accuracy, int max_iterations = 10) {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>   expression_t;
        typedef exprtk::parser<double>       parser_t;

        double x; // Variable x
        double y; // Variable y
        double z; // Variable z to store the result

        // Create symbol table and add variables to it
        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("y", y);
        symbol_table.add_constants();

        // Create an expression
        expression_t expression;
        expression.register_symbol_table(symbol_table);

        // Parse the equation
        parser_t parser;
        if (!parser.compile(equation, expression)) {
            std::cerr << "Error in parsing equation: " << equation << std::endl;
            return;
        }

        // Function to generate approach values using a curve
        auto generate_approach_values = [&](double target, double initial_step, double curve_factor, double step_factor, double stop_accuracy, string variable_name) {
            std::vector<double> values;
            double step = initial_step;
            for (int i = 0; i <= max_iterations; ++i) {
                values.push_back(target - step);
                values.push_back(target + step);
                step /= curve_factor; // Adjust the step size based on the curve factor
                step -= step_factor;
                
                if (target + step <= 3 + stop_accuracy && target - step >= 3 - stop_accuracy) {
                    if (stop_accuracy_disabled == false) {
                        if (variable_name == "x") { x_stop_accuracy_value_reached = true; x_final_iterations = i; }
                        if (variable_name == "y") { y_stop_accuracy_value_reached = true; y_final_iterations = i; }
                        return values;
                    }
                }
            }
            if (variable_name == "x") {x_final_iterations = max_iterations;}
            if (variable_name == "y") {y_final_iterations = max_iterations;}
            return values;
        };

        // Generate values for x and y
        std::vector<double> y_values = generate_approach_values(y_target, y_initial_step, y_curve_factor, y_step_factor, y_stop_accuracy, "y");
        std::vector<double> x_values = generate_approach_values(x_target, x_initial_step, x_curve_factor, x_step_factor, x_stop_accuracy, "x");

        // Evaluate and store results
        if (x_values.size() > y_values.size()) {
            // x_values is longer
            for (size_t i = 0; i < x_values.size(); ++i) {
                x = x_values[i];
                y = (i < y_values.size()) ? y_values[i] : y_values.back(); // Use last value of y_values if it's shorter
                z = expression.value(); // Calculate the expression

                // Determine if it's from left or right
                if (x < x_target || y < y_target) {
                    From_Left_xy.push_back(std::make_tuple(x, y, z));
                }
                else {
                    From_Right_xy.push_back(std::make_tuple(x, y, z));
                }
            }
        }
        else {
            // y_values is longer or they are of the same length
            for (size_t i = 0; i < y_values.size(); ++i) {
                x = (i < x_values.size()) ? x_values[i] : x_values.back(); // Use last value of x_values if it's shorter
                y = y_values[i];
                z = expression.value(); // Calculate the expression

                // Determine if it's from left or right
                if (x < x_target || y < y_target) {
                    From_Left_xy.push_back(std::make_tuple(x, y, z));
                }
                else {
                    From_Right_xy.push_back(std::make_tuple(x, y, z));
                }
            }
        }
    }

    void print_result() const {
        std::cout << "Results approaching from left:" << std::endl;
        for (const auto& pair : From_Left) {
            std::cout << pair.first << "\t" << pair.second << std::endl;
        }

        std::cout << "\nResults approaching from right:" << std::endl;
        for (const auto& pair : From_Right) {
            std::cout << pair.first << "\t" << pair.second << std::endl;
        }

        if (stop_accuracy_value_reached == true) {
            std::cout << "\nstopped due to accuracy limit reached: " << final_iterations << std::endl;
        }else {
            std::cout << "\nstopped due to max iterations limit reached: " << final_iterations << std::endl;
        }
        
    }

    void print_result_xy() const {
        std::cout << "Results approaching from left:" << std::endl;
        for (const auto& tuple : From_Left_xy) {
            std::cout << "x: " << std::get<0>(tuple) << "\ty: " << std::get<1>(tuple) << "\tz: " << std::get<2>(tuple) << std::endl;
        }

        std::cout << "\nResults approaching from right:" << std::endl;
        for (const auto& tuple : From_Right_xy) {
            std::cout << "x: " << std::get<0>(tuple) << "\ty: " << std::get<1>(tuple) << "\tz: " << std::get<2>(tuple) << std::endl;
        }

        if (x_stop_accuracy_value_reached) {
            std::cout << "\nx stopped due to accuracy limit reached at iteration " << x_final_iterations << std::endl;
        }
        else {
            std::cout << "\nx stopped due to max iterations limit reached at iteration " << x_final_iterations << std::endl;
        }

        if (y_stop_accuracy_value_reached) {
            std::cout << "\ny stopped due to accuracy limit reached at iteration " << y_final_iterations << std::endl;
        }
        else {
            std::cout << "\ny stopped due to max iterations limit reached at iteration " << y_final_iterations << std::endl;
        }
    }

    void disable_stop_accuracy() {
        stop_accuracy_disabled = true;
    }

    std::vector<std::pair<double, double>> get_From_Left() {
        return From_Left;
    }
    std::vector<std::pair<double, double>> get_From_Right() {
        return From_Right;
    }
    std::vector<std::tuple<double, double, double>> get_From_Left_xy() {
        return From_Left_xy;
    }
    std::vector<std::tuple<double, double, double>> get_From_Right_xy() {
        return From_Right_xy;
    }

private:
    int final_iterations = 0;
    bool stop_accuracy_value_reached = false;
    std::vector<std::pair<double, double>> From_Left;
    std::vector<std::pair<double, double>> From_Right;
    bool stop_accuracy_disabled = false;

    int x_final_iterations = 0;
    int y_final_iterations = 0;
    bool x_stop_accuracy_value_reached = false;
    bool y_stop_accuracy_value_reached = false;
    std::vector<std::tuple<double, double, double>> From_Left_xy;
    std::vector<std::tuple<double, double, double>> From_Right_xy;
};

class Plot_Values {
public:
    Plot_Values(const std::string& equation,
        double x_min, double x_max,
        double x_interval = 0,
        int num_points = 100)
        : x_min(x_min), x_max(x_max) {

        if (x_interval == 0) {
            x_interval = (x_max - x_min) / num_points;
        }

        generateValues(equation, x_interval);
    }

    const std::vector<double>& getXValues() const {
        return x_values;
    }

    const std::vector<double>& getYValues() const {
        return y_values;
    }

private:
    double x_min, x_max;
    std::vector<double> x_values;
    std::vector<double> y_values;

    void generateValues(const std::string& equation, double x_interval) {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>   expression_t;
        typedef exprtk::parser<double>       parser_t;

        double x;
        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        if (!parser.compile(equation, expression)) {
            std::cerr << "Error in parsing equation: " << equation << std::endl;
            return;
        }

        for (x = x_min; x <= x_max; x += x_interval) {
            x_values.push_back(x);
            y_values.push_back(expression.value());
        }
    }
};

class Recursive_Plot_Values {
public:
    Recursive_Plot_Values(const std::string& equation,
        double r, double x_initial,
        int num_points = 100)
        : r(r), x(x_initial) {

        generateValues(equation, num_points);
    }

    const std::vector<double>& getXValues() const {
        return x_values;
    }

    const std::vector<double>& getYValues() const {
        return y_values;
    }

private:
    double r, x;
    std::vector<double> x_values;
    std::vector<double> y_values;

    void generateValues(const std::string& equation, int num_points) {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>   expression_t;
        typedef exprtk::parser<double>       parser_t;

        symbol_table_t symbol_table;
        symbol_table.add_variable("r", r);
        symbol_table.add_variable("x", x);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        if (!parser.compile(equation, expression)) {
            std::cerr << "Error in parsing equation: " << equation << std::endl;
            return;
        }

        for (int i = 0; i < num_points; ++i) {
            x_values.push_back(i);
            y_values.push_back(x);
            x = expression.value(); // Update x for the next iteration
        }
    }
};