#pragma once
#include "DataMetric.h"
class Real_Result
{
public:
	Real_Result() {}
	~Real_Result() {}
	void cal_knn_result(const string& result_Folder_path, DataMetric& raw_data_, DataMetric& query_data_, int knn_, int dim_);
	
	void load(const string & data_Folder_path);
	int getSize()const { return ground_truth.size(); }
	const vector<vector<pair<DATATYPE, int>>>& getData() const { return ground_truth; }
	const vector<pair<DATATYPE, int>>& operator[](int i) const { return ground_truth[i]; }
	vector<pair<DATATYPE, int>>& operator[](int i) { return ground_truth[i]; }
	vector<vector<pair<DATATYPE, int>>> Get_Result() { return ground_truth; }
private:
	vector<vector<pair<DATATYPE, int>>> ground_truth;
};

