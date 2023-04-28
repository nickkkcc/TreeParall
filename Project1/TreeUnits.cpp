// TreeUnits.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "UnitTree.h"
#include "mutex"

void showHelp() {
		std::cerr << "Wrong using arguments!!!\n\tUsage: " << "-n <Node count> -t <thread count> -f1 <grow> -f2 <statistics>" << std::endl;
}

int main(int argc, char** argv)
{
	if (argc < 8 || (strcmp(argv[1], "-n") != 0 && strcmp(argv[3], "-t") != 0 && strcmp(argv[5], "-f1") != 0 && strcmp(argv[7], "-f2") != 0)
		|| atoll(argv[2]) == 0 || atoll(argv[4]) == 0) {
		showHelp();
	}
	unsigned int timeSteps = 10;
	
	
	std::ofstream file1;
	std::ofstream file2;
	file1.open(argv[6], std::ios::out | std::ios::app);
	file2.open(argv[8], std::ios::out | std::ios::app);

	if (!file1.is_open() || !file2.is_open()) {
		std::printf("File %s can't open, closing program...", argv[6]);
		std::system("pause");
	}
	file1 << "node_count,thread_count,thread_grow,openMP_grow, thread_variance, openMP_variance, thread_mean, openMP_mean,thread_c_interval, openMP_c_interval\n";
	file2 << "thread_count,seq_time,type_alg\n";
	int threads = atoll(argv[4]);
		UnitTree* newTree = new UnitTree();
		BigLong newNodeNumber = atoll(argv[2]);
		UnitTree::run(0, newNodeNumber, threads, file1, file2, newTree, timeSteps);
		delete newTree;
	std::cout << "Program finished :)\n\n";
	std::system("pause");
	return 0;
}


