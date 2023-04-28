#pragma once
#include "Types.h"
#include <unordered_map>
#include "StatisticUnit.h"
#include <fstream>

class Statistics
{
	ThreadCount threadsCount = 0;
	TimePoints timePoints = 0;
	NodeCount nodeCount = 0;
	std::unordered_map<unsigned int, StatisticUnit> points;


public:
	Statistics(ThreadCount threadsCount, TimePoints timePoints, NodeCount nodeCount);
	void addPointsPerThread(std::vector<ThreadTime>& threadPoints, std::vector<OpenMPTime> openMPPoints, SeqTime time, unsigned int threadNumber);
	void writeResults(std::ofstream& stat, std::ofstream& point);
};

