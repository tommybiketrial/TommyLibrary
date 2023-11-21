#pragma once

namespace exprtk_examples{

    template <typename T>
    void trig_function()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string expression_string =
            "clamp(-1.0,sin(2 * pi * x) + cos(x / 2 * pi),+1.0)";

        T x;

        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string, expression);

        for (x = T(-5); x <= T(+5); x += T(0.001))
        {
            const T y = expression.value();
            printf("%19.15f\t%19.15f\n", x, y);
        }
    }

    template <typename T>
    void square_wave()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string expr_string =
            "a*(4/pi)*"
            "((1 /1)*sin( 2*pi*f*t)+(1 /3)*sin( 6*pi*f*t)+"
            " (1 /5)*sin(10*pi*f*t)+(1 /7)*sin(14*pi*f*t)+"
            " (1 /9)*sin(18*pi*f*t)+(1/11)*sin(22*pi*f*t)+"
            " (1/13)*sin(26*pi*f*t)+(1/15)*sin(30*pi*f*t)+"
            " (1/17)*sin(34*pi*f*t)+(1/19)*sin(38*pi*f*t)+"
            " (1/21)*sin(42*pi*f*t)+(1/23)*sin(46*pi*f*t)+"
            " (1/25)*sin(50*pi*f*t)+(1/27)*sin(54*pi*f*t))";

        static const T pi = T(3.141592653589793238462643383279502);

        const T f = pi / T(10);
        const T a = T(10);
        T t = T(0);

        symbol_table_t symbol_table;
        symbol_table.add_variable("t", t);
        symbol_table.add_constant("f", f);
        symbol_table.add_constant("a", a);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expr_string, expression);

        const T delta = (T(4) * pi) / T(1000);

        for (t = (T(-2) * pi); t <= (T(+2) * pi); t += delta)
        {
            const T result = expression.value();
            printf("%19.15f\t%19.15f\n", t, result);
        }
    }

    template <typename T>
    void polynomial()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string expression_string =
            "25x^5 - 35x^4 - 15x^3 + 40x^2 - 15x + 1";

        const T r0 = T(0);
        const T r1 = T(1);
        T  x = T(0);

        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string, expression);

        const T delta = T(1.0 / 100.0);

        for (x = r0; x <= r1; x += delta)
        {
            printf("%19.15f\t%19.15f\n", x, expression.value());
        }
    }

    template <typename T>
    void fibonacci()
    {
        typedef exprtk::symbol_table<T>         symbol_table_t;
        typedef exprtk::expression<T>           expression_t;
        typedef exprtk::parser<T>               parser_t;
        typedef exprtk::function_compositor<T>  compositor_t;
        typedef typename compositor_t::function function_t;

        T x = T(0);

        compositor_t compositor;

        compositor.add(
            function_t("fibonacci")
            .var("x")
            .expression
            (
                " switch                      "
                " {                           "
                "    case x == 0 : 0;         "
                "    case x == 1 : 1;         "
                "    default     :            "
                "    {                        "
                "       var prev := 0;        "
                "       var curr := 1;        "
                "       while ((x -= 1) > 0)  "
                "       {                     "
                "          var temp := prev;  "
                "          prev := curr;      "
                "          curr += temp;      "
                "       };                    "
                "    };                       "
                " }                           "
            ));

        symbol_table_t& symbol_table = compositor.symbol_table();
        symbol_table.add_constants();
        symbol_table.add_variable("x", x);

        std::string expression_str = "fibonacci(x)";

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_str, expression);

        for (std::size_t i = 0; i < 40; ++i)
        {
            x = static_cast<T>(i);

            const T result = expression.value();

            printf("fibonacci(%3d) = %10.0f\n",
                static_cast<int>(i),
                result);
        }
    }

    template <typename T>
    struct myfunc : public exprtk::ifunction<T>
    {
        using exprtk::ifunction<T>::operator();

        myfunc()
            : exprtk::ifunction<T>(2)
        {
            exprtk::disable_has_side_effects(*this);
        }

        inline T operator()(const T& v1, const T& v2)
        {
            return T(1) + (v1 * v2) / T(3);
        }
    };

    template <typename T>
    inline T myotherfunc(T v0, T v1, T v2)
    {
        return std::abs(v0 - v1) * v2;
    }

    template <typename T>
    void custom_function()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string expression_string =
            "myfunc(sin(x / pi), otherfunc(3 * y, x / 2, x * y))";

        T x = T(1);
        T y = T(2);
        myfunc<T> mf;

        symbol_table_t symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("y", y);
        symbol_table.add_function("myfunc", mf);
        symbol_table.add_function("otherfunc", myotherfunc);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string, expression);

        const T result = expression.value();
        printf("Result: %10.5f\n", result);
    }

    template <typename T>
    void vector_function()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string expression_string =
            " for (var i := 0; i < min(x[], y[], z[]); i += 1) "
            " {                                                "
            "    z[i] := 3sin(x[i]) + 2log(y[i]);              "
            " }                                                ";

        T x[] = { T(1.1), T(2.2), T(3.3), T(4.4), T(5.5) };
        T y[] = { T(1.1), T(2.2), T(3.3), T(4.4), T(5.5) };
        T z[] = { T(0.0), T(0.0), T(0.0), T(0.0), T(0.0) };

        symbol_table_t symbol_table;
        symbol_table.add_vector("x", x);
        symbol_table.add_vector("y", y);
        symbol_table.add_vector("z", z);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string, expression);

        expression.value();
    }

    template <typename T>
    void logic()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string expression_string = "not(A and B) or C";

        symbol_table_t symbol_table;
        symbol_table.create_variable("A");
        symbol_table.create_variable("B");
        symbol_table.create_variable("C");

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string, expression);

        printf(" # | A | B | C | %s\n"
            "---+---+---+---+-%s\n",
            expression_string.c_str(),
            std::string(expression_string.size(), '-').c_str());

        for (int i = 0; i < 8; ++i)
        {
            symbol_table.get_variable("A")->ref() = T((i & 0x01) ? 1 : 0);
            symbol_table.get_variable("B")->ref() = T((i & 0x02) ? 1 : 0);
            symbol_table.get_variable("C")->ref() = T((i & 0x04) ? 1 : 0);

            const int result = static_cast<int>(expression.value());

            printf(" %d | %d | %d | %d | %d \n",
                i,
                static_cast<int>(symbol_table.get_variable("A")->value()),
                static_cast<int>(symbol_table.get_variable("B")->value()),
                static_cast<int>(symbol_table.get_variable("C")->value()),
                result);
        }
    }

    template <typename T>
    void composite()
    {
        typedef exprtk::symbol_table<T>         symbol_table_t;
        typedef exprtk::expression<T>           expression_t;
        typedef exprtk::parser<T>               parser_t;
        typedef exprtk::parser_error::type      error_t;
        typedef exprtk::function_compositor<T>  compositor_t;
        typedef typename compositor_t::function function_t;

        T x = T(1);
        T y = T(2);

        compositor_t compositor;

        symbol_table_t& symbol_table = compositor.symbol_table();
        symbol_table.add_constants();
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("y", y);

        compositor
            .add(
                function_t("f", "sin(x / pi)", "x"));          // f(x) = sin(x / pi)

        compositor
            .add(
                function_t("g", "3*[f(x) + f(y)]", "x", "y"));  // g(x,y) = 3[f(x) + f(y)]

        std::string expression_string = "g(1 + f(x), f(y) / 2)";

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;

        if (!parser.compile(expression_string, expression))
        {
            printf("Error: %s\tExpression: %s\n",
                parser.error().c_str(),
                expression_string.c_str());

            for (std::size_t i = 0; i < parser.error_count(); ++i)
            {
                const error_t error = parser.get_error(i);

                printf("Error: %02d  Position: %02d Type: [%14s] Msg: %s\tExpression: %s\n",
                    static_cast<unsigned int>(i),
                    static_cast<unsigned int>(error.token.position),
                    exprtk::parser_error::to_str(error.mode).c_str(),
                    error.diagnostic.c_str(),
                    expression_string.c_str());
            }

            return;
        }

        const T result = expression.value();

        printf("%s = %e\n", expression_string.c_str(), result);
    }

    template <typename T>
    void primes()
    {
        typedef exprtk::symbol_table<T>         symbol_table_t;
        typedef exprtk::expression<T>           expression_t;
        typedef exprtk::parser<T>               parser_t;
        typedef exprtk::function_compositor<T>  compositor_t;
        typedef typename compositor_t::function function_t;

        T x = T(0);

        symbol_table_t symbol_table;

        symbol_table.add_constants();
        symbol_table.add_variable("x", x);

        compositor_t compositor(symbol_table);

        //Mode 1 - if statement based
        compositor.add(
            function_t("is_prime_impl1")
            .vars("x", "y")
            .expression
            (
                " if (y == 1,true,                "
                "    if (0 == (x % y),false,      "
                "       is_prime_impl1(x,y - 1))) "
            ));

        compositor.add(
            function_t("is_prime1")
            .var("x")
            .expression
            (
                " if (frac(x) != 0, false,                                "
                "    if (x <= 0, false,                                   "
                "       is_prime_impl1(x,min(x - 1,trunc(sqrt(x)) + 1)))) "
            ));

        //Mode 2 - switch statement based
        compositor.add(
            function_t("is_prime_impl2")
            .vars("x", "y")
            .expression
            (
                " switch                                         "
                " {                                              "
                "   case y == 1       : true;                    "
                "   case (x % y) == 0 : false;                   "
                "   default           : is_prime_impl2(x,y - 1); "
                " }                                              "
            ));

        compositor.add(
            function_t("is_prime2")
            .var("x")
            .expression
            (
                " switch                                                                 "
                " {                                                                      "
                "   case x <= 0       : false;                                           "
                "   case frac(x) != 0 : false;                                           "
                "   default           : is_prime_impl2(x,min(x - 1,trunc(sqrt(x)) + 1)); "
                " }                                                                      "
            ));

        //Mode 3 - switch statement and while-loop based
        compositor.add(
            function_t("is_prime_impl3")
            .vars("x", "y")
            .expression
            (
                " while (y > 0)                            "
                " {                                        "
                "   switch                                 "
                "   {                                      "
                "     case y == 1       : ~(y := 0,true);  "
                "     case (x % y) == 0 : ~(y := 0,false); "
                "     default           : y := y - 1;      "
                "   }                                      "
                " }                                        "
            ));

        compositor.add(
            function_t("is_prime3")
            .var("x")
            .expression
            (
                " switch                                                                 "
                " {                                                                      "
                "   case x <= 0       : false;                                           "
                "   case frac(x) != 0 : false;                                           "
                "   default           : is_prime_impl3(x,min(x - 1,trunc(sqrt(x)) + 1)); "
                " }                                                                      "
            ));

        std::string expression_str1 = "is_prime1(x)";
        std::string expression_str2 = "is_prime2(x)";
        std::string expression_str3 = "is_prime3(x)";

        expression_t expression1;
        expression_t expression2;
        expression_t expression3;
        expression1.register_symbol_table(symbol_table);
        expression2.register_symbol_table(symbol_table);
        expression3.register_symbol_table(symbol_table);

        parser_t parser;

        parser.compile(expression_str1, expression1);
        parser.compile(expression_str2, expression2);
        parser.compile(expression_str3, expression3);

        for (std::size_t i = 0; i < 100; ++i)
        {
            x = static_cast<T>(i);

            const T result1 = expression1.value();
            const T result2 = expression2.value();
            const T result3 = expression3.value();

            printf("%03d  Result1: %c  Result2: %c  Result3: %c\n",
                static_cast<unsigned int>(i),
                (result1 == T(1)) ? 'T' : 'F',
                (result2 == T(1)) ? 'T' : 'F',
                (result3 == T(1)) ? 'T' : 'F');
        }
    }

    template <typename T>
    void newton_sqrt()
    {
        typedef exprtk::symbol_table<T>         symbol_table_t;
        typedef exprtk::expression<T>           expression_t;
        typedef exprtk::parser<T>               parser_t;
        typedef exprtk::function_compositor<T>  compositor_t;
        typedef typename compositor_t::function function_t;

        T x = T(0);

        symbol_table_t symbol_table;

        symbol_table.add_constants();
        symbol_table.add_variable("x", x);

        compositor_t compositor(symbol_table);

        compositor.add(
            function_t("newton_sqrt")
            .var("x")
            .expression
            (
                " switch                                                  "
                " {                                                       "
                "   case x < 0  : null;                                   "
                "   case x == 0 : 0;                                      "
                "   case x == 1 : 1;                                      "
                "   default:                                              "
                "   ~{                                                    "
                "      var z := 100;                                      "
                "      var sqrt_x := x / 2;                               "
                "      repeat                                             "
                "         if (equal(sqrt_x^2, x))                         "
                "            break[sqrt_x];                               "
                "         else                                            "
                "            sqrt_x := (1 / 2) * (sqrt_x + (x / sqrt_x)); "
                "      until ((z -= 1) <= 0);                             "
                "    };                                                   "
                " }                                                       "
            ));

        const std::string expression_str = "newton_sqrt(x)";

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_str, expression);

        for (std::size_t i = 0; i < 100; ++i)
        {
            x = static_cast<T>(i);

            const T result = expression.value();

            printf("sqrt(%03d) - Result: %15.13f\tReal: %15.13f\n",
                static_cast<unsigned int>(i),
                result,
                std::sqrt(x));
        }
    }

    template <typename T>
    void square_wave2()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string wave_program =
            " var r := 0;                                          "
            "                                                      "
            " for (var i := 0; i < 1000; i += 1)                   "
            " {                                                    "
            "    r += (1 / (2i + 1)) * sin((4i + 2) * pi * f * t); "
            " };                                                   "
            "                                                      "
            " r *= a * (4 / pi);                                   ";

        static const T pi = T(3.141592653589793238462643383279502);

        T f = pi / T(10);
        T t = T(0);
        T a = T(10);

        symbol_table_t symbol_table;
        symbol_table.add_variable("f", f);
        symbol_table.add_variable("t", t);
        symbol_table.add_variable("a", a);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(wave_program, expression);

        const T delta = (T(4) * pi) / T(1000);

        for (t = (T(-2) * pi); t <= (T(+2) * pi); t += delta)
        {
            const T result = expression.value();
            printf("%19.15f\t%19.15f\n", t, result);
        }
    }

    template <typename T>
    void bubble_sort()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string bubblesort_program =
            " var upper_bound := v[];                      "
            "                                              "
            " repeat                                       "
            "    var new_upper_bound := 0;                 "
            "                                              "
            "    for (var i := 1; i < upper_bound; i += 1) "
            "    {                                         "
            "       if (v[i - 1] > v[i])                   "
            "       {                                      "
            "          v[i - 1] <=> v[i];                  "
            "          new_upper_bound := i;               "
            "       };                                     "
            "    };                                        "
            "                                              "
            "    upper_bound := new_upper_bound;           "
            "                                              "
            " until (upper_bound <= 1);                    ";

        T v[] = { T(9.1), T(2.2), T(1.3), T(5.4), T(7.5), T(4.6), T(3.7) };

        symbol_table_t symbol_table;
        symbol_table.add_vector("v", v);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(bubblesort_program, expression);

        expression.value();
    }

    template <typename T>
    void savitzky_golay_filter()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string sgfilter_program =
            " var weight[9] :=                                            "
            "     {                                                       "
            "        -21, 14,  39,                                        "
            "         54, 59,  54,                                        "
            "         39, 14, -21                                         "
            "     };                                                      "
            "                                                             "
            " if (v_in[] >= weight[])                                     "
            " {                                                           "
            "    var lower_bound := trunc(weight[] / 2);                  "
            "    var upper_bound := v_in[] - lower_bound;                 "
            "                                                             "
            "    v_out := 0;                                              "
            "                                                             "
            "    for (var i := lower_bound; i < upper_bound; i += 1)      "
            "    {                                                        "
            "       for (var j := -lower_bound; j <= lower_bound; j += 1) "
            "       {                                                     "
            "          v_out[i] += weight[j + lower_bound] * v_in[i + j]; "
            "       };                                                    "
            "    };                                                       "
            "                                                             "
            "    v_out /= sum(weight);                                    "
            " }                                                           ";

        const std::size_t n = 1024;

        std::vector<T> v_in;
        std::vector<T> v_out;

        const T pi = T(3.141592653589793238462643383279502);

        srand(static_cast<unsigned int>(time(0)));

        // Generate a signal with noise.
        for (T t = T(-5); t <= T(+5); t += T(10.0 / n))
        {
            const T noise = T(0.5 * (rand() / (RAND_MAX + 1.0) - 0.5));
            v_in.push_back(sin(2.0 * pi * t) + noise);
        }

        v_out.resize(v_in.size());

        symbol_table_t symbol_table;
        symbol_table.add_vector("v_in", v_in);
        symbol_table.add_vector("v_out", v_out);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(sgfilter_program, expression);

        expression.value();

        for (std::size_t i = 0; i < v_out.size(); ++i)
        {
            printf("%10.6f\t%10.6f\n", v_in[i], v_out[i]);
        }
    }

    template <typename T>
    void stddev_example()
    {
        typedef exprtk::expression<T> expression_t;
        typedef exprtk::parser<T>     parser_t;

        const std::string stddev_program =
            " var x[25] := {                     "
            "                 1,  2,  3,  4,  5, "
            "                 6,  7,  8,  9, 10, "
            "                11, 12, 13, 14, 15, "
            "                16, 17, 18, 19, 20, "
            "                21, 22, 23, 24, 25  "
            "              };                    "
            "                                    "
            " sqrt(sum([x - avg(x)]^2) / x[])    ";

        expression_t expression;

        parser_t parser;
        parser.compile(stddev_program, expression);

        const T stddev = expression.value();

        printf("stddev(1..25) = %10.6f\n", stddev);
    }

    template <typename T>
    void black_scholes_merton_model()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string bsm_model_program =
            " var d1 := (log(s / x) + (r + v^2 / 2) * t) / (v * sqrt(t)); "
            " var d2 := d1 - v * sqrt(t);                                 "
            "                                                             "
            " if (callput_flag == 'call')                                 "
            "    s * ncdf(d1) - x * e^(-r * t) * ncdf(d2);                "
            " else if (callput_flag == 'put')                             "
            "    x * e^(-r * t) * ncdf(-d2) - s * ncdf(-d1);              "
            "                                                             ";

        T s = T(60.00); // Stock / Underlying / Base price
        T x = T(65.00); // Strike price
        T v = T(0.30); // Volatility
        T t = T(0.25); // Years to maturity
        T r = T(0.08); // Risk free rate

        std::string callput_flag;

        static const T e = exprtk::details::numeric::constant::e;

        symbol_table_t symbol_table;
        symbol_table.add_variable("s", s);
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("t", t);
        symbol_table.add_variable("r", r);
        symbol_table.add_variable("v", v);
        symbol_table.add_constant("e", e);
        symbol_table.add_stringvar("callput_flag", callput_flag);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(bsm_model_program, expression);

        {
            callput_flag = "call";

            const T bsm_option_price = expression.value();

            printf("BSM(%4s, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f) = %10.6f\n",
                callput_flag.c_str(),
                s, x, t, r, v,
                bsm_option_price);
        }

        {
            callput_flag = "put";

            const T bsm_option_price = expression.value();

            printf("BSM(%4s, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f) = %10.6f\n",
                callput_flag.c_str(),
                s, x, t, r, v,
                bsm_option_price);
        }
    }

    template <typename T>
    void linear_least_squares()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string linear_least_squares_program =
            " if (x[] == y[])                                        "
            " {                                                      "
            "    beta  := (sum(x * y) - sum(x) * sum(y) / x[]) /     "
            "             (sum(x^2) - sum(x)^2 / x[]);               "
            "                                                        "
            "    alpha := avg(y) - beta * avg(x);                    "
            "                                                        "
            "    rmse  := sqrt(sum((beta * x + alpha - y)^2) / y[]); "
            " }                                                      "
            " else                                                   "
            " {                                                      "
            "    alpha := null;                                      "
            "    beta  := null;                                      "
            "    rmse  := null;                                      "
            " }                                                      ";

        T x[] = { T(1), T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10) };
        T y[] = { T(8.7), T(6.8), T(6), T(5.6), T(3.8), T(3), T(2.4), T(1.7), T(0.4), T(-1) };

        T alpha = T(0);
        T beta = T(0);
        T rmse = T(0);

        symbol_table_t symbol_table;
        symbol_table.add_variable("alpha", alpha);
        symbol_table.add_variable("beta", beta);
        symbol_table.add_variable("rmse", rmse);
        symbol_table.add_vector("x", x);
        symbol_table.add_vector("y", y);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(linear_least_squares_program, expression);

        expression.value();

        printf("alpha: %15.12f\n", alpha);
        printf("beta:  %15.12f\n", beta);
        printf("rmse:  %15.12f\n", rmse);
        printf("y = %15.12fx + %15.12f\n", beta, alpha);
    }

    template <typename T>
    struct rnd_01 : public exprtk::ifunction<T>
    {
        using exprtk::ifunction<T>::operator();

        rnd_01() : exprtk::ifunction<T>(0)
        {
            ::srand(static_cast<unsigned int>(time(NULL)));
        }

        inline T operator()()
        {
            // Note: Do not use this in production
            // Result is in the interval [0,1)
            return T(::rand() / T(RAND_MAX + 1.0));
        }
    };

    template <typename T>
    void monte_carlo_pi()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string monte_carlo_pi_program =
            " var samples[5 * 10^7] := [(rnd_01^2 + rnd_01^2) <= 1]; "
            " 4 * sum(samples) / samples[];                          ";

        rnd_01<T> rnd01;

        symbol_table_t symbol_table;
        symbol_table.add_function("rnd_01", rnd01);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(monte_carlo_pi_program, expression);

        const T approximate_pi = expression.value();

        const T real_pi = T(3.141592653589793238462643383279502); // or close enough...

        printf("pi ~ %20.17f\terror: %20.17f\n",
            approximate_pi,
            std::abs(real_pi - approximate_pi));
    }

    template <typename T>
    void file_io()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string fileio_program =
            " var file_name := 'file.txt';                          "
            " var stream    := null;                                "
            "                                                       "
            " if (stream := open(file_name,'w'))                    "
            "    println('Successfully opened file: ' + file_name); "
            " else                                                  "
            " {                                                     "
            "    println('Failed to open file: ' + file_name);      "
            "    return [false];                                    "
            " };                                                    "
            "                                                       "
            " var s := 'Hello world...\n';                          "
            "                                                       "
            " for (var i := 0; i < 10; i += 1)                      "
            " {                                                     "
            "    write(stream,s);                                   "
            " };                                                    "
            "                                                       "
            " if (close(stream))                                    "
            "    println('Sucessfully closed file: ' + file_name);  "
            " else                                                  "
            " {                                                     "
            "    println('Failed to close file: ' + file_name);     "
            "    return [false];                                    "
            " }                                                     ";

        exprtk::rtl::io::file::package<T> fileio_package;
        exprtk::rtl::io::println<T>       println;

        symbol_table_t symbol_table;
        symbol_table.add_function("println", println);
        symbol_table.add_package(fileio_package);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(fileio_program, expression);

        printf("Result %10.3f\n", expression.value());
    }

    template <typename T>
    class randu : public exprtk::igeneric_function<T>
    {
    public:

        typedef typename exprtk::igeneric_function<T> igfun_t;
        typedef typename igfun_t::parameter_list_t    parameter_list_t;
        typedef typename igfun_t::generic_type        generic_type;
        typedef typename generic_type::vector_view    vector_t;

        using exprtk::igeneric_function<T>::operator();

        randu()
            : exprtk::igeneric_function<T>("V|VTT")
            /*
               Overloads:
               0. V   - vector
               1. VTT - vector, r0, r1
            */
        {
            ::srand(static_cast<unsigned int>(time(NULL)));
        }

        inline T operator()(const std::size_t& ps_index, parameter_list_t parameters)
        {
            vector_t v(parameters[0]);

            std::size_t r0 = 0;
            std::size_t r1 = v.size() - 1;

            if (
                (1 == ps_index) &&
                !exprtk::rtl::vecops::helper::
                load_vector_range<T>::process(parameters, r0, r1, 1, 2, 0)
                )
                return T(0);

            for (std::size_t i = r0; i <= r1; ++i)
            {
                v[i] = rnd();
            }

            return T(1);
        }

    private:

        inline T rnd()
        {
            // Note: Do not use this in production
            // Result is in the interval [0,1)
            return T(::rand() / T(RAND_MAX + 1.0));
        }
    };

    template <typename T>
    void vector_randu()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string vecrandu_program =
            " var noise[6] := [0];                     "
            "                                          "
            " if (randu(noise,0,5) == false)           "
            " {                                        "
            "    println('Failed to generate noise');  "
            "    return [false];                       "
            " };                                       "
            "                                          "
            " var noisy[6] := signal + (noise - 1/2);  "
            "                                          "
            " for (var i := 0; i < noisy[]; i += 1)    "
            " {                                        "
            "    println('noisy[',i,'] = ', noisy[i]); "
            " };                                       "
            "                                          "
            " println('avg: ', avg(noisy));            "
            "                                          ";

        T signal[] = { T(1.1), T(2.2), T(3.3), T(4.4), T(5.5), T(6.6), T(7.7) };

        exprtk::rtl::io::println<T> println;
        randu<T>                    randu;

        symbol_table_t symbol_table;
        symbol_table.add_vector("signal", signal);
        symbol_table.add_function("println", println);
        symbol_table.add_function("randu", randu);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(vecrandu_program, expression);

        expression.value();
    }

    struct vector_access_rtc : public exprtk::vector_access_runtime_check
    {
        typedef std::map<void*, std::string> map_t;
        map_t vector_map;

        bool handle_runtime_violation(violation_context& context)
        {
            const map_t::iterator itr = vector_map.find(static_cast<void*>(context.base_ptr));
            std::string vector_name = (itr != vector_map.end()) ?
                itr->second : "Unknown";

            printf("Runtime vector access violation\n"
                "Vector: %s base: %p end: %p access: %p typesize: %lu\n",
                vector_name.c_str(),
                context.base_ptr,
                context.end_ptr,
                context.access_ptr,
                context.type_size);

            throw std::runtime_error("Runtime vector access violation. Vector: " + vector_name);
            return false;
        }
    };

    template <typename T>
    void vector_overflow_example()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string& expression_str =
            " for (var i := 0; i < max(v0[],v1[]); i += 1) "
            " {                                            "
            "   v0[i] := (2 * v0[i]) + (v1[i] / 3);        "
            " }                                            ";

        T v0[5] = { 0, 1, 2, 3, 4 };
        T v1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        vector_access_rtc vec_rtc;

        vec_rtc.vector_map[v0] = "v0";
        vec_rtc.vector_map[v1] = "v1";

        symbol_table_t symbol_table;
        expression_t   expression;
        parser_t       parser;

        symbol_table.add_vector("v0", v0);
        symbol_table.add_vector("v1", v1);

        expression.register_symbol_table(symbol_table);

        parser.register_vector_access_runtime_check(vec_rtc);

        try
        {
            if (!parser.compile(expression_str, expression))
            {
                printf("Error: %s\tExpression: %s\n",
                    parser.error().c_str(),
                    expression_str.c_str());

                return;
            }

            expression.value();
        }
        catch (std::runtime_error& exception)
        {
            printf("Exception: %s\n", exception.what());
        }
    }

    template <typename T>
    void binomial_option_pricing_model()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string binomial_model_program =
            " var dt     := t / n;                                              "
            " var z      := exp(r * dt);                                        "
            " var z_inv  := 1 / z;                                              "
            " var u      := exp(v * sqrt(dt));                                  "
            " var u_inv  := 1 / u;                                              "
            " var p_up   := (z - u_inv) / (u - u_inv);                          "
            " var p_down := 1 - p_up;                                           "
            "                                                                   "
            " var base_price  [n + 1] := [0];                                   "
            " var option_price[n + 1] := [0];                                   "
            "                                                                   "
            " for (var i := 0; i <= n; i += 1)                                  "
            " {                                                                 "
            "    base_price[i]   := s * u^(n - 2i);                             "
            "    option_price[i] :=                                             "
            "       switch                                                      "
            "       {                                                           "
            "          case callput_flag == 'call' : max(base_price[i] - x, 0); "
            "          case callput_flag == 'put'  : max(x - base_price[i], 0); "
            "       };                                                          "
            " };                                                                "
            "                                                                   "
            " for (var j := n - 1; j >= 0; j -= 1)                              "
            " {                                                                 "
            "    for (var i := 0; i <= j; i += 1)                               "
            "    {                                                              "
            "       option_price[i] := z_inv *                                  "
            "          (p_up * option_price[i] + p_down * option_price[i + 1]); "
            "    }                                                              "
            " };                                                                "
            "                                                                   "
            " option_price[0];                                                  ";

        T s = T(100.00); // Stock / Underlying / Base price
        T x = T(110.00); // Strike price
        T v = T(0.30); // Volatility
        T t = T(2.22); // Years to maturity
        T r = T(0.05); // Risk free rate
        T n = T(1000.00); // Number of time steps

        std::string callput_flag;

        symbol_table_t symbol_table;
        symbol_table.add_variable("s", s);
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("t", t);
        symbol_table.add_variable("r", r);
        symbol_table.add_variable("v", v);
        symbol_table.add_constant("n", n);
        symbol_table.add_stringvar("callput_flag", callput_flag);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(binomial_model_program, expression);

        callput_flag = "call";

        const T binomial_call_option_price = expression.value();

        printf("BinomialPrice(%4s, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f) = %10.6f\n",
            callput_flag.c_str(),
            s, x, t, r, v,
            binomial_call_option_price);

        callput_flag = "put";

        const T binomial_put_option_price = expression.value();

        printf("BinomialPrice(%4s, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f) = %10.6f\n",
            callput_flag.c_str(),
            s, x, t, r, v,
            binomial_put_option_price);

        const double put_call_parity_diff =
            (binomial_call_option_price - binomial_put_option_price) -
            (s - x * std::exp(-r * t));

        printf("Put-Call parity difference: %20.17f\n", put_call_parity_diff);
    }

    //compute_implied_volatility
    template <typename T>
    void compute_implied_volatility()
    {
        typedef exprtk::symbol_table<T>         symbol_table_t;
        typedef exprtk::expression<T>           expression_t;
        typedef exprtk::parser<T>               parser_t;
        typedef exprtk::function_compositor<T>  compositor_t;
        typedef typename compositor_t::function function_t;

        const std::string implied_volatility_program =
            " var v         := 0.5; /* Initial volatility guess */            "
            " var epsilon   := 0.0000001;                                     "
            " var max_iters := 1000;                                          "
            " var itr       := 0;                                             "
            "                                                                 "
            " while ((itr += 1) <= max_iters)                                 "
            " {                                                               "
            "    var price :=                                                 "
            "       switch                                                    "
            "       {                                                         "
            "          case callput_flag == 'call' : bsm_call(s, x, r, t, v); "
            "          case callput_flag == 'put'  : bsm_put (s, x, r, t, v); "
            "       };                                                        "
            "                                                                 "
            "    var price_diff := price - target_price;                      "
            "                                                                 "
            "    if (abs(price_diff) <= epsilon)                              "
            "    {                                                            "
            "       break;                                                    "
            "    };                                                           "
            "                                                                 "
            "    v -= price_diff / vega(s, x, r, t, v);                       "
            " };                                                              "
            "                                                                 "
            " itr <= max_iters ? v : null;                                    ";

        T s = T(100.00); // Stock / Underlying / Base price
        T x = T(110.00); // Strike price
        T t = T(2.22); // Years to maturity
        T r = T(0.05); // Risk free rate
        T n = T(1000.00); // Number of time steps
        T target_price = T(0.00);

        std::string callput_flag;

        symbol_table_t symbol_table(symbol_table_t::e_immutable);
        symbol_table.add_variable("s", s);
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("t", t);
        symbol_table.add_variable("r", r);
        symbol_table.add_constant("n", n);
        symbol_table.add_stringvar("callput_flag", callput_flag);
        symbol_table.add_variable("target_price", target_price);
        symbol_table.add_pi();

        compositor_t compositor(symbol_table);

        compositor.add(
            function_t("bsm_call")
            .vars("s", "x", "r", "t", "v")
            .expression
            (
                " var d1 := (log(s / x) + (r + v^2 / 2) * T) / (v * sqrt(t)); "
                " var d2 := d1 - v * sqrt(t);                                 "
                " s * ncdf(d1) - x * exp(-r * T) * ncdf(d2);                  "
            ));

        compositor.add(
            function_t("bsm_put")
            .vars("s", "x", "r", "t", "v")
            .expression
            (
                " var d1 := (log(s / x) + (r + v^2 / 2) * t) / (v * sqrt(t)); "
                " var d2 := d1 - v * sqrt(t);                                 "
                " x * exp(-r * T) * ncdf(-d2) - s * ncdf(-d1);                "
            ));

        compositor.add(
            function_t("vega")
            .vars("s", "x", "r", "t", "v")
            .expression
            (
                " var d1 := (log(s / x) + (r + v^2 / 2) * t) / (v * sqrt(t)); "
                " s * sqrt(T) * exp(-d1^2 / 2) / sqrt(2pi);                   "
            ));

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(implied_volatility_program, expression);

        {
            callput_flag = "call";
            target_price = T(18.339502);

            const T implied_vol = expression.value();

            printf("Call Option(s: %5.3f, x: %5.3f, t: %5.3f, r: %5.3f) @ $%8.6f Implied volatility = %10.8f\n",
                s, x, t, r, target_price, implied_vol);
        }

        {
            callput_flag = "put";
            target_price = T(16.782764);

            const T implied_vol = expression.value();

            printf("Put  Option(s: %5.3f, x: %5.3f, t: %5.3f, r: %5.3f) @ $%8.6f Implied volatility = %10.8f\n",
                s, x, t, r, target_price, implied_vol);
        }
    }
    
//    template <typename T>
//    void real_1d_discrete_fourier_transform()
//    {
//        typedef exprtk::symbol_table<T>         symbol_table_t;
//        typedef exprtk::expression<T>           expression_t;
//        typedef exprtk::parser<T>               parser_t;
//        typedef exprtk::function_compositor<T>  compositor_t;
//        typedef typename compositor_t::function function_t;
//
//        const double sampling_rate = 1024.0;            // ~1KHz
//        const std::size_t N = 8 * sampling_rate; // 8 seconds worth of samples
//
//        std::vector<double> input(N, 0.0);
//        std::vector<double> output(N, 0.0);
//
//        exprtk::rtl::io::println<T> println;
//
//        symbol_table_t symbol_table;
//        symbol_table.add_vector("input", input);
//        symbol_table.add_vector("output", output);
//        symbol_table.add_function("println", println);
//        symbol_table.add_constant("N", N);
//        symbol_table.add_constant("sampling_rate", sampling_rate);
//        symbol_table.add_pi();
//
//        compositor_t compositor(symbol_table);
//        compositor.load_vectors(true);
//
//        compositor.add(
//            function_t("dft_1d_real")
//            .var("N")
//            .expression
//            (
//                " for (var k := 0; k < N; k += 1)          "
//                " {                                        "
//                "    var k_real := 0.0;                    "
//                "    var k_imag := 0.0;                    "
//                "                                          "
//                "    for (var i := 0; i < N; i += 1)       "
//                "    {                                     "
//                "       var theta := 2pi * k * i / N;      "
//                "       k_real += input[i] * cos(theta);   "
//                "       k_imag -= input[i] * sin(theta);   "
//                "    };                                    "
//                "                                          "
//                "    output[k] := hypot(k_real,k_imag);    "
//                " };                                       "
//                "                                          "
//                " output;                                  "
//            ));
//
//        const std::string dft_program =
//            "                                                                       "
//            " /*                                                                    "
//            "    Generate an aggregate waveform comprised of three                  "
//            "    sine waves of varying frequencies and amplitudes.                  "
//            " */                                                                    "
//            " var frequencies[3] := { 100.0, 200.0, 300.0 }; /* Hz    */            "
//            " var amplitudes [3] := {  10.0,  20.0,  30.0 }; /* Power */            "
//            "                                                                       "
//            " for (var i := 0; i < N; i += 1)                                       "
//            " {                                                                     "
//            "    var time := i / sampling_rate;                                     "
//            "                                                                       "
//            "    for (var j := 0; j < frequencies[]; j += 1)                        "
//            "    {                                                                  "
//            "       var frequency := frequencies[j];                                "
//            "       var amplitude := amplitudes [j];                                "
//            "       var theta     := 2 * pi * frequency * time;                     "
//            "                                                                       "
//            "       input[i] += amplitude * sin(theta);                             "
//            "    }                                                                  "
//            " };                                                                    "
//            "                                                                       "
//            " dft_1d_real(input[]);                                                 "
//            "                                                                       "
//            " var freq_bin_size   := sampling_rate / N;                             "
//            " var max_bin         := ceil(N / 2);                                   "
//            " var max_noise_level := 1e-5;                                          "
//            "                                                                       "
//            " /* Normalise amplitudes */                                            "
//            " output /= max_bin;                                                    "
//            "                                                                       "
//            " println('1D Real DFT Frequencies');                                   "
//            "                                                                       "
//            " for (var k := 0; k < max_bin; k += 1)                                 "
//            " {                                                                     "
//            "    if (output[k] > max_noise_level)                                   "
//            "    {                                                                  "
//            "       var freq_begin := k * freq_bin_size;                            "
//            "       var freq_end   := freq_begin + freq_bin_size;                   "
//            "                                                                       "
//            "       println('Index: ', k,' ',                                       "
//            "               'Freq. range: [', freq_begin, 'Hz, ', freq_end, 'Hz) ', "
//            "               'Amplitude: ', output[k]);                              "
//            "    }                                                                  "
//            " }                                                                     "
//            "                                                                       ";
//
//        expression_t expression;
//        expression.register_symbol_table(symbol_table);
//
//        parser_t parser;
//        parser.compile(dft_program, expression);
//
//        expression.value();
//    }
    
    //rpn_example

    template <typename T, T Process(unsigned char)>
    struct char_process : public exprtk::igeneric_function<T>
    {
        typedef typename exprtk::igeneric_function<T> igfun_t;
        typedef typename igfun_t::parameter_list_t    parameter_list_t;
        typedef typename igfun_t::generic_type        generic_type;
        typedef typename generic_type::string_view    string_t;

        char_process()
            : exprtk::igeneric_function<T>("S")
        {}

        inline T operator()(parameter_list_t parameters)
        {
            const unsigned char c = string_t(parameters[0])[0];
            return Process(c);
        }
    };

    template <typename T>
    T is_digit_func(unsigned char c)
    {
        return (('0' <= c) && (c <= '9')) ? T(1) : T(0);
    }

    template <typename T>
    T to_num_func(unsigned char c)
    {
        return static_cast<T>(c - '0');
    }

    template <typename T>
    void rpn_example()
    {
        typedef exprtk::symbol_table<T> symbol_table_t;
        typedef exprtk::expression<T>   expression_t;
        typedef exprtk::parser<T>       parser_t;

        const std::string rpn_program =
            " var stack[1000] := [0];                                                   "
            " var stack_size  := 0;                                                     "
            "                                                                           "
            " for (var i := 0; i < rpn_expression[]; i += 1)                            "
            " {                                                                         "
            "    var c := rpn_expression[i : i + 1];                                    "
            "                                                                           "
            "    if (c == ' ')                                                          "
            "    {                                                                      "
            "       continue;                                                           "
            "    }                                                                      "
            "    else if (is_digit(c))                                                  "
            "    {                                                                      "
            "       stack[stack_size] := to_num(c);                                     "
            "       stack_size        += 1;                                             "
            "    }                                                                      "
            "    else                                                                   "
            "    {                                                                      "
            "       var operator := c;                                                  "
            "       var operand1 := stack[stack_size - 2];                              "
            "       var operand2 := stack[stack_size - 1];                              "
            "       stack_size   -= 2;                                                  "
            "                                                                           "
            "       switch                                                              "
            "       {                                                                   "
            "          case operator == '+' : stack[stack_size] := operand1 + operand2; "
            "          case operator == '-' : stack[stack_size] := operand1 - operand2; "
            "          case operator == '*' : stack[stack_size] := operand1 * operand2; "
            "          case operator == '/' : stack[stack_size] := operand1 / operand2; "
            "          case operator == '^' : stack[stack_size] := operand1 ^ operand2; "
            "       };                                                                  "
            "                                                                           "
            "       stack_size += 1;                                                    "
            "    }                                                                      "
            " };                                                                        "
            "                                                                           "
            " println(stack[0], ' = ', rpn_expression);                                 "
            "                                                                           ";

        std::string rpn_expression;

        char_process<T, is_digit_func<T>> isdigit;
        char_process<T, to_num_func<T>>   tonum;
        exprtk::rtl::io::println<T>      println;

        symbol_table_t symbol_table;
        symbol_table.add_stringvar("rpn_expression", rpn_expression);
        symbol_table.add_function("println", println);
        symbol_table.add_function("is_digit", isdigit);
        symbol_table.add_function("to_num", tonum);

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(rpn_program, expression);

        const std::string expressions[] =
        {
           "2 3 8 / ^ 4 6 * + 3 9 / -",
           "1 2 / 6 5 2 - / * 7 +",
           "1 2 * 3 / 4 * 5 / 6 *",
           "8 6 4 + * 2 /"
        };

        for (std::size_t i = 0; i < sizeof(expressions) / sizeof(std::string); ++i)
        {
            rpn_expression = expressions[i];
            expression.value();
        }
    }

//Numerical Derivative
    template <typename T>
    T get_numerical_derivative(exprtk::expression<T>& expression, T& x, T h = T(0.00001))
    {
        T original_x = x;
        T x0 = x - h;
        T x1 = x + h;

        x = x0;
        T f_x0 = expression.value();

        x = x1;
        T f_x1 = expression.value();

        // Restore the original value of x
        x = original_x;

        return (f_x1 - f_x0) / (2 * h);
    }
    template <typename T>
    T numerical_derivative()
    {
        std::string expression_string = "sin(x)";  // Example function

        exprtk::symbol_table<T> symbol_table;
        exprtk::expression<T> expression;
        exprtk::parser<T> parser;

        T x;
        symbol_table.add_variable("x", x);
        symbol_table.add_constants();

        expression.register_symbol_table(symbol_table);

        if (!parser.compile(expression_string, expression))
        {
            std::cerr << "Error in expression: " << expression_string << std::endl;
            return 1;
        }

        // Evaluate derivative at a specific point, e.g., x = 1
        x = 1;
        T derivative = get_numerical_derivative(expression, x);

        std::cout << "The derivative of " << expression_string << " at x = " << x << " is approximately " << derivative << std::endl;

        return 0;
    }

}