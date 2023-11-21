#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <stdexcept>
#include <set>
#include <string>
#include <sstream>
#include <iomanip>
#include <climits>
#include <fstream>
#include <map>
#include <queue>
#include <unordered_set>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <deque>
#include <cstddef>
#include <numeric>


#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define _CRT_SECURE_NO_WARNINGS

using std::size_t;
using std::vector;
using std::cin; using std::cout; using std::endl;
using std::string;
using std::pair;
using std::set;

#include <SFML/Graphics.hpp>
#include "InfInt.h"
#include "Profiny.h"
#include "exprtk.hpp"

#include "SFML_graph_wrapper.h"
#include "TommyTools.h"
#include "TommyCounter.h"
#include "Fibonacci.h"
#include "TommyTimer.h"
#include "Multidimensional_Core.h"
#include "TommyArrayHelp.h"
#include "LogicGate.h"
#include "ReciprocalsOfPrimes.h"
#include "RegexWrapper.h"
#include "CircularDependency.h"
#include "TommyLogic.h"
#include "FunctionalGate.h"
#include "FunctionalGate_Helper.h"
#include "LitreGallonFuelCalculator.h"
#include "SudokuSolver.h"
#include "Calculus_1.h"
#include "exprtk_examples.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "TommyFunctions.cpp"
