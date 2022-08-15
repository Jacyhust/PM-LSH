#pragma once
#include<vector>
#include"DataMetric.h"

using namespace std;
class Pivot
{
public:
	Pivot() {}
	~Pivot() {}
	static vector<vector<DATATYPE>> Random_pivot(DataMetric& raw_data_, int pivot_num_, int N_);
private:
	static DATATYPE distance_Between_Piovt_Vector(vector<vector<DATATYPE>>& pivot_vector_);
};

