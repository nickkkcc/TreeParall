#pragma once
#include <chrono>

using BigLong = unsigned long long;
using ThreadCount = unsigned int;
using TimePoints = unsigned int;
using NodeCount = BigLong;

using SeqTime = BigLong;
using ThreadTime = BigLong;
using OpenMPTime = BigLong;

using Time = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::steady_clock::time_point;
