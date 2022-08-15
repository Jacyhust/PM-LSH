#pragma once
#include"DataMetric.h"
#include"Real_Result.h"
class Metric
{
public:
	Metric();
	~Metric();
	static vector<DATATYPE> calRecallAll(Real_Result& real_result_, vector <vector<pair<DATATYPE, int>>>& query_res_);
	static DATATYPE Cal_Recall(int targetKnn_, Real_Result & real_result_, vector<vector<pair<DATATYPE, int>>> query_res_);
	static vector<DATATYPE> calOverRatioAll(Real_Result & real_result_, vector<vector<pair<DATATYPE, int>>>& query_res_);
	static DATATYPE Cal_Ratio(int target_knn_, Real_Result & real_result_, vector<vector<pair<DATATYPE, int>>> query_res_);
	static DATATYPE calRecall(vector<pair<DATATYPE, int>>& realResult, vector<pair<DATATYPE, int>>& approResult);
};

