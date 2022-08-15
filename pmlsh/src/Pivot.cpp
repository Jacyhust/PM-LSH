#include "Pivot.h"
#include<random>
#include<vector>
#include<set>
#include"MyFunc.h"
using namespace std;

vector<vector<DATATYPE>> Pivot::Random_pivot(DataMetric& raw_data_, int pivot_num_, int N_)
{
	default_random_engine rng(0);
	uniform_int_distribution<int> u_random_int(0, raw_data_.size() - 1);
	vector<vector<vector<DATATYPE>>> N_pivot_label(N_);
	for (int i = 0; i < N_; ++i) {
		set<int> repeat_flag_set;
		vector<vector<DATATYPE>> pivot_label(pivot_num_);
		for (int j = 0; j < pivot_num_; ++j) {
			int label = u_random_int(rng);
			if (repeat_flag_set.count(label) == 0)
			{
				pivot_label[j] = raw_data_[label];
				repeat_flag_set.insert(label);
			}
			else
				--j;
		}
		N_pivot_label.emplace_back(pivot_label);
	}
	/*maximum distantce*/
	pair<DATATYPE, int> res_label(INT16_MIN, -1);
	for (int i = 0; i < N_pivot_label.size(); ++i) {
		DATATYPE dist = distance_Between_Piovt_Vector(N_pivot_label[i]);
		if (dist > res_label.first)
			res_label.second = i;
	}
	return N_pivot_label[res_label.second];
}

DATATYPE Pivot::distance_Between_Piovt_Vector(vector<vector<DATATYPE>>& pivot_vector_)
{
	DATATYPE all_dist = 0;
	for (int i = 0; i < pivot_vector_.size(); ++i) {
		for (int j = i + 1; j < pivot_vector_.size(); ++j) {
			all_dist += MyFunc::Cal_Euclidean_distance(pivot_vector_[i], pivot_vector_[j]);
		}
	}
	return all_dist;
}