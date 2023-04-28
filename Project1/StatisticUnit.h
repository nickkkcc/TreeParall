#pragma once
#include "Types.h"
#include <vector>

class StatisticUnit
{
	SeqTime seqTime;
	double meanThread;
	double meanOpenMP;
	double varianceThread;
	double varianceOpenMP;
	double lowerLimitThread;
	double lowerLimitOpenMP;
	double upperLimitThread;
	double upperLimitOpenMP;
	std::vector<ThreadTime> threadPoints;
	std::vector<OpenMPTime> openMPPoints;
	

public:
	StatisticUnit();
	StatisticUnit(std::vector<ThreadTime>& threadPoints, std::vector<OpenMPTime> openMPPoints, SeqTime time);

	const std::vector<ThreadTime>& getThreadPoints();
	
	const std::vector<OpenMPTime>& getOpenMPPoints();
	double getVarianceThread();
	double getVarianceOpenMP();
	double getMeanThread();
	double getmeanOpenMP();
	double getLowerLimitOpenMP();
	double getLowerLimitThread();
	double getUpperLimitThread();
	double getUpperLimitOpenMP();
	SeqTime getSeqTime();

private:
	void calculateStudentInterval();

};

