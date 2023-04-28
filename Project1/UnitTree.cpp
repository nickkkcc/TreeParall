#include "UnitTree.h"
#include "queue"
#include <math.h>
#include <algorithm>
#include <numeric>
#include "omp.h"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "Statistics.h"

int t = 1;
using namespace boost::accumulators;

UnitTree* UnitTree::addNewNode(UnitTree* prevTree, BigLong val)
{
	if (prevTree == nullptr) {
		prevTree = createTree(prevTree, val);
	}
	else if (prevTree->getVal() > val) {
		prevTree->setLeftNode(addNewNode(prevTree->getLeftNode(), val));
	}
	else if (prevTree->getVal() <= val) {
		prevTree->setRightNode(addNewNode(prevTree->getRightNode(), val));
		
	}
	return prevTree;
}

UnitTree* UnitTree::createTree(UnitTree* prevTree, BigLong val)
{
	prevTree = new UnitTree();
	prevTree->setVal(val);
	return prevTree;
}

UnitTree* UnitTree::getRandomTree(BigLong oldNodeCount, BigLong newNodeCount, UnitTree* newTree)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 10);
	newTree->setVal(dist(gen));

	for (size_t i = 0; i < newNodeCount - oldNodeCount - 1; i++)
	{
		if (i % 10000 == 0) {
			std::cout << i << std::endl;
		}
		newTree->addNewNode(newTree, dist(gen));
	}
	//std::printf("Tree generated with %llu nodes\n\n\n", nodeCount);
	return newTree;
}

BigLong UnitTree::getSumOfChild(UnitTree* tree)
{
	if (tree != nullptr)
	{
		BigLong leftSum = 0;
		BigLong rightSum = 0;

		if (tree->getLeftNode()) {
			std::this_thread::sleep_for(std::chrono::microseconds(t));
			tree->getLeftNode()->setSum(getSumOfChild(tree->getLeftNode()));
			leftSum = tree->getLeftNode()->getSum() + tree->getLeftNode()->getVal();
		}
		

	if (tree->getRightNode()) {
			std::this_thread::sleep_for(std::chrono::microseconds(t));
			tree->getRightNode()->setSum(getSumOfChild(tree->getRightNode()));
			rightSum = tree->getRightNode()->getSum() + tree->getRightNode()->getVal();
	}
		tree->setSum(leftSum + rightSum);
		return tree->getSum();
	}
	return tree->getSum();
}

BigLong UnitTree::getThreadSumOfChild(UnitTree* tree, int depth)
{	
	
	if (tree != nullptr) {
		BigLong leftSum = 0;
		BigLong rightSum = 0;

		if (depth <= 1) {
			tree->setSum(getSumOfChild(tree));
			return 0;
		}

		std::thread leftThread;
		UnitTree leftTree;
		if (tree->getLeftNode() != nullptr) {
			leftTree = *(tree->getLeftNode());
			int levels = (depth - 1) / 2;
			leftThread = std::thread(getThreadSumOfChild, &leftTree, levels);
		}
		std::thread rightThread;
		UnitTree rightTree;

		if (tree->getRightNode() != nullptr) {
			rightTree = *(tree->getRightNode());
			int levels = (depth - 1) / 2;
			rightThread = std::thread(getThreadSumOfChild, &rightTree, levels);
		}

		if (leftThread.joinable()) {
			leftThread.join();
		}
		if (tree->getLeftNode() != nullptr) {
			std::this_thread::sleep_for(std::chrono::microseconds(t));
			tree->getLeftNode()->setSum(leftTree.getSum());
			leftSum = tree->getLeftNode()->getSum() + tree->getLeftNode()->getVal();

		}
		if (rightThread.joinable()) {
			rightThread.join();
		}
		if (tree->getRightNode() != nullptr) {
			std::this_thread::sleep_for(std::chrono::microseconds(t));
			tree->getRightNode()->setSum(rightTree.getSum());
			rightSum = tree->getRightNode()->getSum() + tree->getRightNode()->getVal();
			
		}

		tree->setSum(leftSum + rightSum);
		return tree->getSum();
	}
	return tree->getSum();
}

BigLong UnitTree::getOpenMPSumOfChild(UnitTree* tree)
{
	if (tree) {
		BigLong leftSum = 0;
		BigLong rightSum = 0;
		if (omp_get_active_level() >= omp_get_max_active_levels()) {
			return getSumOfChild(tree);
		}
		#pragma omp parallel sections
		{
			#pragma	omp section
				{
					if (tree->getLeftNode()) {
						//std::cout << "1 in " << omp_get_thread_num() << std::endl;
						std::this_thread::sleep_for(std::chrono::microseconds(t));
						tree->getLeftNode()->setSum(getOpenMPSumOfChild(tree->getLeftNode()));
						leftSum = tree->getLeftNode()->getSum() + tree->getLeftNode()->getVal();

					}
				}
			#pragma	omp section
				{
					if (tree->getRightNode()) {
						//std::cout << "2 in " << omp_get_thread_num() << std::endl;
						std::this_thread::sleep_for(std::chrono::microseconds(t));
						tree->getRightNode()->setSum(getOpenMPSumOfChild(tree->getRightNode()));
						rightSum = tree->getRightNode()->getSum() + tree->getRightNode()->getVal();

					}
				}
		}
		tree->setSum(leftSum + rightSum);
		return tree->getSum();
	}
	return tree->getSum();

	return 0;
}

BigLong UnitTree::sequencelySum(UnitTree* tree, BigLong nodeCount, std::ofstream& file, int threads)
{
	TimePoint start = Time::now();
	BigLong sum = getSumOfChild(tree);
	TimePoint end = Time::now();
	BigLong time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//std::printf("\tSequencely sum Time is %llu [ns]\n\tSum is %llu\n\n", time, sum);
	//file << "defaultSum" << "," << nodeCount << "," << time <<","<<threads<< "\n";
	return time;
}

BigLong UnitTree::threadsSum(UnitTree* tree, BigLong nodeCount, std::ofstream& file, int depth)
{
	TimePoint start = Time::now();
	BigLong sum = getThreadSumOfChild(tree, depth);
	TimePoint end = Time::now();
	BigLong time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//std::printf("\tSequencely sum Time is %llu [ns]\n\tSum is %llu\n\n", time, sum);
	//file << "threadSum" << "," << nodeCount << "," << time <<"," << depth<< "\n";
	return time;
}

BigLong UnitTree::openMPSum(UnitTree* tree, BigLong nodeCount, std::ofstream& file, int threads)
{
	omp_set_nested(true);
	omp_set_max_active_levels((threads - 1) / 2);
	TimePoint start = Time::now();
	BigLong sum = getOpenMPSumOfChild(tree);
	TimePoint end = Time::now();
	BigLong time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//std::printf("\tSequencely sum Time is %llu [ns]\n\tSum is %llu\n\n", time, sum);
	//file << "OpenMPSum" << "," << nodeCount << "," << time << "\n";
	return time;
}

BigLong UnitTree::getSize()
{
	return size;
}

UnitTree* UnitTree::generateTree(std::vector<BigLong> values)
{
	UnitTree* newTree = new UnitTree();
	newTree->setVal(values[0]);

	for (size_t i = 1; i < values.size(); i++)
	{
		newTree->addNewNode(newTree, values.at(i));
	}
	//std::printf("Tree generated with %llu nodes\n\n\n", nodeCount);
	return newTree;
}


UnitTree* UnitTree::getRandomTreeNEW(BigLong oldNodeCount, BigLong newNodeCount, UnitTree* newTree)
{
	std::queue<UnitTree*> que;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 10);
	newTree->setVal(dist(gen));
	que.push(newTree);

	for (size_t i = 0; i < newNodeCount - oldNodeCount - 1; i++)
	{
		/*if (i % 10000 == 0) {
		std::cout << i << std::endl;
		}*/
		BigLong val = dist(gen);
		if (que.front()->getLeftNode() == nullptr) {
			que.front()->setLeftNode(new UnitTree());
			que.front()->getLeftNode()->setVal(val);
			que.push(que.front()->getLeftNode());
		}
		else if (que.front()->getRightNode() == nullptr) {
			que.front()->setRightNode(new UnitTree());
			que.front()->getRightNode()->setVal(val);
			que.push(que.front()->getRightNode());
		}
		else {
			que.pop();
		}
		if (que.front()->getRightNode() != nullptr && que.front()->getLeftNode() != nullptr) {
			que.pop();
		}
	}
	//std::printf("Tree generated with %llu nodes\n\n\n", nodeCount);
	return newTree;
}


int UnitTree::run(BigLong oldNodeCount, BigLong newNodeCount, int depth, std::ofstream& file1, std::ofstream& file2, UnitTree* newTree, unsigned int timeSteps)
{
	newTree = getRandomTreeNEW(oldNodeCount, newNodeCount, newTree);
	std::vector<BigLong> threadVector(0);
	std::vector<BigLong> openMPMeanVector(0);
	Statistics stats = Statistics(depth, timeSteps, newNodeCount);

	for (int i = 3; i <= depth; i++)
	{
		std::printf("\n\n\t[Thread number]: %d\n\n", i);
		BigLong seqTime = sequencelySum(newTree, newNodeCount, file1, depth);
		for (size_t j = 0; j < timeSteps; j++)
		{
			BigLong thrTime = threadsSum(newTree, newNodeCount, file1, i);
			BigLong openMPTime = openMPSum(newTree, newNodeCount, file1, i);
			threadVector.push_back(thrTime);
			openMPMeanVector.push_back(openMPTime);
		}

		stats.addPointsPerThread(threadVector, openMPMeanVector, seqTime, i);
		threadVector.clear();
		openMPMeanVector.clear();
	}
	stats.writeResults(file1, file2);
	return 0;
}



double UnitTree::SKO(std::vector<BigLong> arr, double mean)
{
	int size = arr.size();
	if (size == 1) { return 0.0; };
	double SKO = 0;
	for (size_t i = 0; i < size; i++)
	{
		SKO += (((arr.at(i) - mean) * (arr.at(i) - mean)) / size);
	}
	return SKO;
}

double UnitTree::mean(std::vector<BigLong> arr)
{
	int size = arr.size();
	if (size == 0) { return 0; };
	return std::accumulate(arr.begin(), arr.end(), 0.0) / size;
}


//UnitTree::~UnitTree()
//{
//	if (this!= nullptr && this->leftNode != nullptr) {
//		delete this->leftNode;
//	}
//
//	if (this != nullptr && this->rightNode != nullptr) {
//		delete this->rightNode;
//	}
//}


void UnitTree::setVal(BigLong val)
{
	this->val = val;
}

BigLong UnitTree::getVal()
{
	return this->val;
}

void UnitTree::setSum(BigLong summary)
{
	this->sum = summary;
}

BigLong UnitTree::getSum()
{
	return this->sum;
}



void UnitTree::setLeftNode(UnitTree* leftNode)
{
	this->leftNode = leftNode;
}

UnitTree* UnitTree::getLeftNode()
{
	return this->leftNode;
}

void UnitTree::setRightNode(UnitTree* rightNode)
{
	this->rightNode = rightNode;
}

UnitTree* UnitTree::getRightNode()
{
	return this->rightNode;
}


