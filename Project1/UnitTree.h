#pragma once
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include "Types.h"


class UnitTree
{
	BigLong val = 0;
	BigLong sum = 0;
	UnitTree* leftNode = nullptr;
	UnitTree* rightNode = nullptr;
	BigLong size = 0;

	static UnitTree* addNewNode(UnitTree* prevTree, BigLong val);
	static UnitTree* createTree(UnitTree* prevTree, BigLong val);
	static UnitTree* getRandomTree(BigLong oldNodeCount, BigLong newNodeCount, UnitTree* newTree);
	static BigLong getSumOfChild(UnitTree* tree);
	static BigLong getThreadSumOfChild(UnitTree* tree, int depth);
	static BigLong getOpenMPSumOfChild(UnitTree* tree);
	static BigLong sequencelySum(UnitTree* tree, BigLong nodeCount, std::ofstream& file, int threads);
	static BigLong threadsSum(UnitTree* tree, BigLong nodeCount, std::ofstream& file, int depth);
	static BigLong openMPSum(UnitTree* tree, BigLong nodeCount, std::ofstream& file, int threads);
	BigLong getSize();
	static UnitTree* generateTree(std::vector<BigLong> values);
	static UnitTree* getRandomTreeNEW(BigLong oldNodeCount, BigLong newNodeCount, UnitTree* newTree);
	void setVal(BigLong val);
	BigLong getVal();
	void setSum(BigLong sum);
	BigLong getSum();
	void setLeftNode(UnitTree* leftNode);
	UnitTree* getLeftNode();
	void setRightNode(UnitTree* rightNode);
	UnitTree* getRightNode();
	static double SKO(std::vector<BigLong> arr, double mean);
	static double mean(std::vector<BigLong> arr);
public:
	static int run(BigLong oldNodeCount, BigLong newNodeCount, int depth, std::ofstream& file1, std::ofstream& file2, UnitTree* newTree, unsigned int timeSteps);
	//~UnitTree();
};

