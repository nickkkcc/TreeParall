#include "Statistics.h"

Statistics::Statistics(ThreadCount threadsCount, TimePoints timePoints, NodeCount nodeCount)
{
	this->threadsCount = threadsCount;
	this->timePoints = timePoints;
	this->nodeCount = nodeCount;
}

void Statistics::addPointsPerThread(std::vector<ThreadTime>& threadPoints, std::vector<OpenMPTime> openMPPoints, SeqTime time, unsigned int threadNumber)
{
	StatisticUnit unit = StatisticUnit(threadPoints, openMPPoints, time);
	points[threadNumber] = unit;
}

void Statistics::writeResults(std::ofstream& stat, std::ofstream& point)
{
	for (auto& var : points) {
		double threadGrow = 100 - var.second.getMeanThread() * 100 / var.second.getSeqTime();
		double openMPGrow = 100 - var.second.getmeanOpenMP() * 100 / var.second.getSeqTime();

		stat << nodeCount << "," << var.first << "," << threadGrow << "," << openMPGrow << ","
			<< var.second.getVarianceThread() << "," << var.second.getVarianceOpenMP() << ","
			<< var.second.getMeanThread() << "," <<var.second.getmeanOpenMP() << ","
			<< "(" << var.second.getLowerLimitThread() << ";" << var.second.getUpperLimitThread() << "),"
			<< "(" << var.second.getLowerLimitOpenMP() << ";" << var.second.getUpperLimitOpenMP() << ")\n";

		point << var.first << "," << var.second.getSeqTime() << ",std::thread,";
		for (auto obj : var.second.getThreadPoints()) {
			point << obj << ",";
		}
			point <<"\n";
			point << var.first << "," << var.second.getSeqTime() << ",openMP,";
		for (auto obj : var.second.getOpenMPPoints()) {
			point << obj << ",";
		}
		point << "\n";
	}




}
