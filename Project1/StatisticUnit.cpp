#include "StatisticUnit.h"
#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics.hpp"
#include <boost/math/distributions/students_t.hpp>
using namespace boost::accumulators;
using namespace boost::math;

StatisticUnit::StatisticUnit()
{
	this->threadPoints = std::vector<ThreadTime>(0);
	this->openMPPoints = std::vector<OpenMPTime>(0);
	seqTime = 0;
	varianceThread = 0;
	varianceOpenMP = 0;
	meanThread = 0;
	meanOpenMP = 0;
}

StatisticUnit::StatisticUnit(std::vector<ThreadTime>& threadPoints, std::vector<OpenMPTime> openMPPoints, SeqTime time)
{
	accumulator_set<double, features<tag::mean, tag::variance>> accThread;
	accumulator_set<double, features<tag::mean, tag::variance>> accOpenMP;

	this->threadPoints = std::move(threadPoints);
	this->openMPPoints = std::move(openMPPoints);

	for (auto var : this->threadPoints) {
		accThread(var);
	}

	for (auto var : this->openMPPoints) {
		accOpenMP(var);
	}

	seqTime = time;

	meanThread = boost::accumulators::mean(accThread);
	meanOpenMP = boost::accumulators::mean(accOpenMP);

	varianceThread = boost::accumulators::variance(accThread);
	varianceOpenMP = boost::accumulators::variance(accOpenMP);
	calculateStudentInterval();

}

const std::vector<ThreadTime>& StatisticUnit::getThreadPoints()
{
	return threadPoints;
}

const std::vector<OpenMPTime>& StatisticUnit::getOpenMPPoints()
{
	return openMPPoints;
}

double StatisticUnit::getVarianceThread()
{
	return varianceThread;
}

double StatisticUnit::getVarianceOpenMP()
{
	return varianceOpenMP;
}

double StatisticUnit::getMeanThread()
{
	return meanThread;
}

double StatisticUnit::getmeanOpenMP()
{
	return meanOpenMP;
}

double StatisticUnit::getLowerLimitOpenMP()
{
	return lowerLimitOpenMP;
}

double StatisticUnit::getLowerLimitThread()
{
	return lowerLimitThread;
}

double StatisticUnit::getUpperLimitThread()
{
	return upperLimitThread;
}

double StatisticUnit::getUpperLimitOpenMP()
{
	return upperLimitOpenMP;
}

SeqTime StatisticUnit::getSeqTime()
{
	return seqTime;
}

void StatisticUnit::calculateStudentInterval()
{
	students_t dist(threadPoints.size() - 1);
	double T = boost::math::quantile(complement(dist, 0.05 / 2));
	double wThread = T * varianceThread / std::sqrt(double(threadPoints.size()));
	double wOpenMP = T * varianceOpenMP / std::sqrt(double(threadPoints.size()));

	lowerLimitThread = meanThread - wThread;
	upperLimitThread = meanThread + wThread;
	
	lowerLimitOpenMP = meanOpenMP - wOpenMP;
	upperLimitOpenMP = meanOpenMP + wOpenMP;
}


