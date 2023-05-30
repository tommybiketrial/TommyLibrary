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
#include "InfInt.h"
#include "Profiny.h"
#include "LitreGallonFuelCalculator.h"
#include "SudokuSolver.h"

#include "TommyFunctions.cpp"
