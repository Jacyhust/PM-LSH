#include "DataMetric.h"
#include <algorithm>
#include <fstream>
#include "Timer.h"

void DataMetric::loadData(const string & data_file_,int dim_)
{
	this->dimension = dim_;

	std::ifstream in(data_file_, std::ios::binary); // open data file 
	this->dataset.clear();
	if (!in.is_open()) {
		std::cout << "open file error: " << data_file_ << std::endl;
		printf("Fatal error in file: %s, func: %s, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}
	in.seekg(0, std::ios::beg);
	int cnt = 0;
	float* buf = new float[this->dimension];

	while (in) {
		if (cnt % 10000000 == 10000000 - 1)
			printf("cnt = %d\n", cnt + 1);
		vector<DATATYPE> tmp(this->dimension);
		in.seekg(4, std::ios::cur);
		in.read((char*)(&tmp[0]), 4 * this->dimension);
		if (!in) break;
		dataset.emplace_back(tmp);
		cnt++;
	}
	delete[] buf;
	
	in.close();
}

void DataMetric::lowerDimFromAnother(const DataMetric& another, E2LSH* e2lsh, Config& config)
{
	dataset.resize(another.size());
	for (int i = 0; i < dataset.size(); ++i) {
		dataset[i] = e2lsh->getHashVal(another[i],config);
	}
	this->dimension = dataset[0].size();
}



