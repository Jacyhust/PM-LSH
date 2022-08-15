#pragma once

#include<string>
#include<iostream>
#include<vector>
#include<fstream>
#include<assert.h>
//#include <io.h>
#include <iomanip>
#include "E2LSH.h"

using namespace std;

class DataMetric
{
public:
	DataMetric() {};
	~DataMetric() {};

	virtual void loadData(const string & data_file_, int dim_);
	void lowerDimFromAnother(const DataMetric& another, E2LSH* e2lsh, Config& config);

	virtual string type() { return "DataMetric"; };
	virtual int size()const { return dataset.size(); }
	const vector<vector<DATATYPE>>& getData() const { return dataset; }
	const vector<DATATYPE>& operator[](int i) const { return dataset[i]; }
	vector<DATATYPE>& operator[](int i) { return dataset[i]; }
	void resize(int n) { dataset.resize(n); }

protected:
	int dimension = 0;
	vector<vector<DATATYPE>> dataset;
};






