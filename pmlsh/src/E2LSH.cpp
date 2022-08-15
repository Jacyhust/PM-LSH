#include "E2LSH.h"

void E2LSH::constructTable(Config& config)
{
	constructParam(config);
}

vector<DATATYPE> E2LSH::getHashVal(const vector<DATATYPE>& domin, Config& config) const
{
	vector<DATATYPE> sum;
	vector<DATATYPE> hashVal;
	sum.resize(config.lowDim);
	for (unsigned i = 0; i < config.lowDim; ++i){
		for (unsigned j = 0; j < domin.size(); ++j){
			sum[i] += domin[j] * hash_a[i][j];
		}
		hashVal.emplace_back(sum[i]);
	}
	return hashVal;
}

void E2LSH::constructParam(Config& config)
{
	mt19937 rng(unsigned(time(0)));
	hash_a.resize(config.lowDim);

	std::normal_distribution<float> nd;
	int dim = config.highDim;

	for (vector<vector<DATATYPE> >::iterator iter = hash_a.begin(); iter != hash_a.end(); ++iter){
		for (unsigned i = 0; i < dim; ++i){
			iter->emplace_back(nd(rng));
		}
	}	
}