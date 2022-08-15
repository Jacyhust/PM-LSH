#pragma once
#include<algorithm>
#include"PM_Tree.h"
#include"DataMetric.h"
#include "Pivot.h"
#include "Config.h"
#include "Timer.h"
#include"Metric.h"
class pmLsh
{
public:
	pmLsh() { myIndex = new PM_Tree(Config::M_NUM); }
	~pmLsh() { delete myIndex; }
	//void Search(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData, Real_Result& real_result);

	//void incrementalSearch(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData, Real_Result& real_result);
	//void incrementalSearchWithKth(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData, Real_Result& real_result);
	void improvedSearchWithKth(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData, Real_Result& real_result);
	void constructIndex(DataMetric& lowData);

private:
	PM_Tree* myIndex = NULL;
	vector<vector<pair<DATATYPE, int>>> queryResult;
};

