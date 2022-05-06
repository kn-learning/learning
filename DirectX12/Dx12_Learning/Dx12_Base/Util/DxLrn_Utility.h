#include <list>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <assert.h>
#include "Snct_Math.h"

using HashKey = std::string;

inline void RuntimeError(std::runtime_error& e)
{
	_RPT0(_CRT_ASSERT, e.what());
}

inline void RuntimeLog(std::string log)
{
	_RPT0(_CRT_WARN, log.c_str());
}