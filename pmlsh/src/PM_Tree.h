#pragma once
#include<vector>
#include<set>
#include<iostream>
#include <stdint.h>
#include"MyFunc.h"
#include <stdint.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<random>
#include<queue>
#include<time.h>
#include"Timer.h"
#include"DataMetric.h"
#include"Metric.h"
#include"Config.h"

using namespace std;

enum PromoteFunc_E { RANDOM_e,m_RAD_2 };
struct M_Node_St
{
	/*common*/
	M_Node_St* patent_node = NULL;
	int node_category = 0;  //0 represent routing node , 1:leaf node , 2: data entry
	vector<DATATYPE> feature_val;
	DATATYPE dist_to_parent = 0;	//the distance between the node and parent node
	int level = 0;//level of root is 0
	/*data node*/
	int id = 0;
	vector<DATATYPE> pivot_distance;

	/*routing or leaf node */
	DATATYPE range = 0;
	vector<M_Node_St*> ptr_sub_tree;
	vector<pair<DATATYPE,DATATYPE>> hyper_rings;//pair<min,max>

	M_Node_St() {		
	}
	M_Node_St(M_Node_St* patent_node_, int node_category_,
			DATATYPE dist_to_parent_,int id_) {
		patent_node = patent_node_;
		node_category = node_category_;
		dist_to_parent = dist_to_parent_;
		id = id_;
	}
};

struct unVisitNodeQueueCmp
{
	bool operator()(const pair<DATATYPE, M_Node_St*> p1, const pair<DATATYPE, M_Node_St*> p2)
	{
		return p1.first > p2.first;
	}
};



class PM_Tree
{
public:
	PM_Tree(int m_):M(m_){
		//cout << "Build an PM-Tree with M \n\n";
		//promotefunc_e = m_RAD_2;
		promotefunc_e = RANDOM_e;
		root = nullptr;
		Max_Level=0;
		MIN_CAPACITY = M / 2;
	};

private:
	int num_verified = 0;
	
public:
	PM_Tree() {
		//cout << "Build an PM-Tree\n\n";
	};
	~PM_Tree() { delete unVisitNodeQueue; };
	
	void Insert(vector<DATATYPE> feature_val_, int id_);
	void rangeSearch(DataMetric& highData, vector<DATATYPE>& highQuery, vector<DATATYPE>& q_feature_val_,DATATYPE search_range_,vector<pair<DATATYPE, int>>& res_vec_);
	DATATYPE checkRange(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData);
	void incrementalRangeSearch(vector<DATATYPE>& lowQuery, DATATYPE searchRange, vector<pair<DATATYPE, int>>& candidateResult);
	void incrementalRangeSearchWithKth(vector<DATATYPE>& lowQuery, DATATYPE searchRange, vector<pair<DATATYPE, int>>& candidateResult, std::priority_queue<knnPair>& res);
	//void improvedRangeSearch(vector<DATATYPE>& lowQuery, DATATYPE searchRange, vector<pair<DATATYPE, int>>& candidateResult, std::priority_queue<knnPair>& res);
	void improvedRangeSearch(vector<DATATYPE>& highQuery, DataMetric& highData, vector<DATATYPE>& lowQuery,
		DATATYPE searchRange, vector<pair<DATATYPE, int>>& candidateResult, std::priority_queue<knnPair>& res, DATATYPE& all_cost, std::vector<bool>& visitFlag);

	void improvedRangeSearch1(vector<DATATYPE>& highQuery, DataMetric& highData, vector<DATATYPE>& lowQuery,
		DATATYPE searchRange, std::priority_queue<knnPair, std::vector<knnPair>, greater<knnPair>>& buffer, std::priority_queue<knnPair>& res, DATATYPE& all_cost, std::vector<bool>& visitFlag);

	void updateMinDistKth(DATATYPE minDistKth) { this->minDistKth = minDistKth; }
	bool isCompleteIncrementalRangeSearch() {
		return satisfyCount >= Config::T;
		//d_k in original space
		// t*r
		//
	}
	bool isCompleteIncrementalRangeSearch(DATATYPE radius) {
		return satisfyCount >= Config::T || (minDistKth * Config::t < radius );
	//	return satisfyCount >= Config::T;
	}
	void Set_Pivot(int pivot_num_,int HR_,int PD_, const vector<vector<DATATYPE>>& pivot_vec_);
	int Get_Pivot_Filter_Num() { return Pivot_Filter_Num; }
	int Get_Cal_Distance_Num() { return Cal_Distance_Num; }
	int Get_Via_subNode_Num() { return Via_subNode_Num; }
	int Get_Via_Node_Num() { return Via_Node_Num; }
	void reset() {
		rangeSearchStartFlag = false;
		satisfyCount = 0;
		delete unVisitNodeQueue;
		distQueryAndPivot.clear();
		satisfyCountArray.clear();
		visitHighDataFlag.clear();
		traverseLeafPointArray.clear();
		minDistKth = DBL_MAX;
	}
	void reset(DATATYPE capacity) {
		reset();
		traverseLeafPointArray.reserve(capacity);
	}
	double Traverse_Get_Volume();
	void Update_Level(M_Node_St * cur_node_, int level_);
	void Traverse_Bread_Tree(string out_file_path_);
	void Output_Node_Info(ofstream& out_,const M_Node_St* cur_node_);
	M_Node_St* Get_Root() { return root; }
	static bool Is_Rounting_Node(const M_Node_St* node_) {
		return node_->node_category == 0;
	}
	static bool Is_leaf_Node(const M_Node_St* node_) {
		return node_->node_category == 1;
	}
	static bool Is_Root_Node(const M_Node_St* node_) {
		return node_->patent_node == nullptr;
	}
	static bool Is_Data_Node(const M_Node_St* node_) {
		return node_->node_category == 2;
	}
protected:

	DATATYPE calLowerBound(vector<DATATYPE>& lowQuery,M_Node_St* node);
	priority_queue<pair<DATATYPE, M_Node_St*>, vector<pair<DATATYPE, M_Node_St*>>, unVisitNodeQueueCmp >* initUnVisitNodeQueue() {
		return new priority_queue<pair<DATATYPE, M_Node_St*>, vector<pair<DATATYPE, M_Node_St*>>, unVisitNodeQueueCmp >;
	}

	virtual bool Is_Full(M_Node_St* node_) { return node_->ptr_sub_tree.size() >= M; }
	void Sub_Traverse_Get_Volumem(M_Node_St* cur_node_, double& volume_);
	void Update_hyper_rings(vector<pair<DATATYPE, DATATYPE>>& hyper_rings_, vector<DATATYPE>& data_feature_val_);
	void Update_pivot_distance(vector<DATATYPE>& pivot_distance_, vector<DATATYPE>& data_feature_val_);
	bool Is_Pivot_Filter(vector<DATATYPE>& dist_q_pivot_, DATATYPE range_, M_Node_St* cur_node_);
	void subRange_Search(DataMetric& highData, vector<DATATYPE>& highQuery, M_Node_St** cur_node_address_, vector<DATATYPE>& q_feature_val_, DATATYPE search_range_, vector<pair<DATATYPE, int>>& res_vec_, DATATYPE& dist_parent_q_, vector<DATATYPE>& dist_q_pivot_);
	void Split(M_Node_St** cur_node_ptr_address_, M_Node_St** insert_node_address_);
	void Insert(M_Node_St** cur_node_address_, M_Node_St** insert_node_address_);
	void assign_Node_All_Value(M_Node_St* cur_node_, M_Node_St* new_node_);
	void Merge_subNode_HR(M_Node_St* cur_node_);
	M_Node_St** Get_Next_Node_And_Update_Range(M_Node_St** cur_node_address_, M_Node_St** insert_node_address_);


	void Promote(vector<M_Node_St*>& entries_, M_Node_St* node_1_, M_Node_St* node_2_);
	void Promote_Random(vector<M_Node_St*>& entries_, M_Node_St* node_1_, M_Node_St* node_2_);
	void Promote_M_RAD_2(vector<M_Node_St*>& entries_, M_Node_St* node_1_, M_Node_St* node_2_);
	DATATYPE minimum_Rad(vector<M_Node_St*>& entries_, int pos1_, int pos2_);
	void Partition(vector<M_Node_St*>& entries_, M_Node_St* node_1_, M_Node_St* node_2_);
	DATATYPE Cal_Cover_Radius(const M_Node_St* node_);
	DATATYPE Cal_Cover_Radius(const M_Node_St* node_, const M_Node_St* cmp_node_);
	DATATYPE Cal_Dist_To_Parent(const M_Node_St* node_);

	int M = 0; //node capacity
	int MIN_CAPACITY = 0;
	int HR = 0;
	int PD = 0;
	int Piovt_Num = 0;
	int Max_Level = 0;
	vector<vector<DATATYPE>> pivot_vec;
	M_Node_St* root = NULL;
	PromoteFunc_E promotefunc_e = RANDOM_e;
	int Pivot_Filter_Num = 0;
	int Cal_Distance_Num = 0;
	int Via_subNode_Num = 0;
	int Via_Node_Num = 0;
	vector<int> visitHighDataFlag;
	vector<DATATYPE> distQueryAndPivot;

	priority_queue<pair<DATATYPE, M_Node_St*>, vector<pair<DATATYPE, M_Node_St*>>, unVisitNodeQueueCmp >* unVisitNodeQueue = NULL;
	vector<int> satisfyCountArray;
	vector<pair<DATATYPE,int>> traverseLeafPointArray;
	bool rangeSearchStartFlag = false;
	int satisfyCount = 0;
	DATATYPE minDistKth = 0;
private:

};

