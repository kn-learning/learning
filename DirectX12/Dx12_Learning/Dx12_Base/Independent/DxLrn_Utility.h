#include <list>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include "Snct_Math.h"

using HashKey = std::string;

inline void ThrowError(std::runtime_error& e)
{
	_RPT0(_CRT_ASSERT, e.what());
}