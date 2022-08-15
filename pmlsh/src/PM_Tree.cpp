#include "PM_Tree.h"
#include <queue>
Timer time_buff[10];
DATATYPE time_res[10] = {0};
using namespace std;


/*给定*/
void PM_Tree::Split(M_Node_St ** cur_node_ptr_address_, M_Node_St ** insert_node_address_)
{
	M_Node_St * cur_node_ = *cur_node_ptr_address_;
	M_Node_St * insert_node_ = *insert_node_address_;
	/*spilt two new nodes */
	M_Node_St* new_Mnode_1 = new M_Node_St(nullptr, 0, -1, -1);
	M_Node_St* new_Mnode_2 = new M_Node_St(nullptr, 0, -1, -1);

	vector<M_Node_St*>  entries = cur_node_->ptr_sub_tree;
	entries.emplace_back(insert_node_);
	
	/*decide featuer value of node_1 and node_2  basing on entries*/
	Promote(entries, new_Mnode_1, new_Mnode_2);
	
	/*divide ertries into node_1 and node_2  */
	Partition(entries, new_Mnode_1, new_Mnode_2);

	if (Is_Root_Node(cur_node_)) {
		M_Node_St* new_root = new M_Node_St(nullptr,0,-1,-1);
		new_root->feature_val = new_Mnode_1->feature_val;/*update root feature value*/

		/*update information*/
		new_Mnode_1->patent_node = new_root;
		new_Mnode_1->dist_to_parent = Cal_Dist_To_Parent(new_Mnode_1);
		new_Mnode_2->patent_node = new_root;
		new_Mnode_2->dist_to_parent = Cal_Dist_To_Parent(new_Mnode_2);
		Merge_subNode_HR(new_Mnode_1);
		Merge_subNode_HR(new_Mnode_2);


		new_root->ptr_sub_tree.emplace_back(new_Mnode_1);
		new_root->ptr_sub_tree.emplace_back(new_Mnode_2);
		new_root->range = Cal_Cover_Radius(new_root);
		Merge_subNode_HR(new_root);
		root = new_root;
	}
	else {
		/*update information*/
		new_Mnode_1->patent_node = cur_node_->patent_node;
		new_Mnode_1->dist_to_parent = Cal_Dist_To_Parent(new_Mnode_1);
		new_Mnode_2->patent_node = cur_node_->patent_node;
		new_Mnode_2->dist_to_parent = Cal_Dist_To_Parent(new_Mnode_2);
		/*将new_Mnode_1中元素替换cur_node_元素，并更新父节点*/
		assign_Node_All_Value(cur_node_, new_Mnode_1);
		Merge_subNode_HR(cur_node_);
		Merge_subNode_HR(new_Mnode_2);
		/*delete memory*/
		delete(new_Mnode_1);

		if (Is_Full(new_Mnode_2->patent_node)) {
			Split(&(new_Mnode_2->patent_node), &new_Mnode_2);
		}
		else {		
			new_Mnode_2->patent_node->ptr_sub_tree.emplace_back(new_Mnode_2);
		}
	}
}

void PM_Tree::Insert(vector<DATATYPE> feature_val_, int id_)
{
	M_Node_St* new_node = new M_Node_St(nullptr, 2, -1, id_);
	new_node->feature_val = feature_val_;
	new_node->range = -1;
	Update_pivot_distance(new_node->pivot_distance, feature_val_);

	if (root == nullptr) {	//empty tree
		M_Node_St* new_root = new M_Node_St(nullptr, 1, -1, -1);		//空树第一个根节点设为第一个数据特征值
		new_root->feature_val = feature_val_;
		new_root->ptr_sub_tree.emplace_back(new_node);
		new_root->range = Cal_Cover_Radius(new_root);
		Update_hyper_rings(new_root->hyper_rings,feature_val_);
		root = new_root;									//update root node
	
		new_node->patent_node = new_root;
		new_node->dist_to_parent = Cal_Dist_To_Parent(new_node);
	}
	else {
		Insert(&root, &new_node);
	}
}

void PM_Tree::rangeSearch(DataMetric& highData, vector<DATATYPE>& highQuery, vector<DATATYPE>& lowQuery, DATATYPE search_range_, vector<pair<DATATYPE, int>>& res_vec_)
{
	this->visitHighDataFlag.resize(highData.size());
	this->Via_Node_Num = 0;
	this->Via_subNode_Num = 0;
	this->Pivot_Filter_Num = 0;
	this->Cal_Distance_Num = 0;
	
	/*produce dist_query_pivot*/
	vector<DATATYPE> dist_q_pivot(max(PD, HR));
	for (int i = 0; i < pivot_vec.size(); ++i) {
		dist_q_pivot[i] = MyFunc::Cal_Euclidean_distance(lowQuery, pivot_vec[i]);

	}
	DATATYPE dist = MyFunc::Cal_Euclidean_distance(root->feature_val, lowQuery);
	
	subRange_Search(highData, highQuery, &(this->root), lowQuery, search_range_, res_vec_,dist, dist_q_pivot);

}

DATATYPE PM_Tree::checkRange(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData)
{
	DATATYPE averRecall = 0;
	for (int i = 0; i < lowQueryData.size(); ++i) {
		vector<pair<DATATYPE, int>> rangeResult;
		rangeSearch(highData, highQueryData[i], lowQueryData[i], Config::search_Radius, rangeResult);

		vector<pair<DATATYPE, int>> realResult;
		for (int j = 0; j < highData.size(); ++j) {
			DATATYPE dist = MyFunc::Cal_Euclidean_distance(highData[j], highQueryData[i]);
			if (dist < Config::search_Radius) {
				realResult.emplace_back(make_pair(dist, j));
			}
		}
		sort(rangeResult.begin(), rangeResult.end());
		sort(realResult.begin(), realResult.end());
		DATATYPE recall = Metric::calRecall(realResult, rangeResult);
		if (recall < 1.0)
			cout << "error in cal recall" << endl;
		averRecall += recall;
	}
	return averRecall / lowQueryData.size();
}

void PM_Tree::incrementalRangeSearch(vector<DATATYPE>& lowQuery, DATATYPE searchRange, vector<pair<DATATYPE,int>>& candidateResult)
{
	if (!rangeSearchStartFlag) {
		satisfyCountArray.resize(100, 0);
		rangeSearchStartFlag = true;
		unVisitNodeQueue = initUnVisitNodeQueue();
		unVisitNodeQueue->push(make_pair(0,root));
		/*computing distance between query and pivot*/
		distQueryAndPivot.resize(max(PD, HR));
		for (int i = 0; i < pivot_vec.size(); ++i) {
			distQueryAndPivot[i] = MyFunc::Cal_Euclidean_distance(lowQuery, pivot_vec[i]);
		}
	}
	satisfyCount = 0;
	DATATYPE accumulateC = 1;
	int satisfyCountArrayPos = 0;
	while (searchRange >= Config::search_Radius * accumulateC) {
		satisfyCount += satisfyCountArray[satisfyCountArrayPos];
		satisfyCountArrayPos++;
		accumulateC *= Config::c_appro;
	}
	while (!unVisitNodeQueue->empty()) {
		M_Node_St* curNode = unVisitNodeQueue->top().second;
		DATATYPE curLB = unVisitNodeQueue->top().first;
		if (curLB < searchRange && !isCompleteIncrementalRangeSearch()) {
			unVisitNodeQueue->pop();
			if (Is_leaf_Node(curNode)) {
				DATATYPE dist = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* dataNode = curNode->ptr_sub_tree[i];
					dist = MyFunc::Cal_Euclidean_distance(dataNode->feature_val, lowQuery);

					candidateResult.emplace_back(make_pair(dist, dataNode->id));

					int pos = max((int)floor(log(dist / Config::search_Radius) / log(Config::c_appro)), 0)+1;
					satisfyCountArray[pos] += 1;
					if (dist < searchRange) {
						satisfyCount++;
					}
					if (isCompleteIncrementalRangeSearch()) {
						break;
					}
				}
			}
			else {
				DATATYPE nextLB = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* nextNode = curNode->ptr_sub_tree[i];
					nextLB = calLowerBound(lowQuery, nextNode);
					unVisitNodeQueue->push(make_pair(nextLB, nextNode));
				}
			}
		}
		else
			break;
	}
}


void PM_Tree::incrementalRangeSearchWithKth(vector<DATATYPE>& lowQuery, DATATYPE searchRange, vector<pair<DATATYPE, int>>& candidateResult, std::priority_queue<knnPair>& res)
{
	if (!rangeSearchStartFlag) {
		satisfyCountArray.resize(100, 0);
		rangeSearchStartFlag = true;
		unVisitNodeQueue = initUnVisitNodeQueue();
		unVisitNodeQueue->push(make_pair(0, root));
		/*computing distance between query and pivot*/
		distQueryAndPivot.resize(max(PD, HR));
		for (int i = 0; i < pivot_vec.size(); ++i) {
			distQueryAndPivot[i] = MyFunc::Cal_Euclidean_distance(lowQuery, pivot_vec[i]);
		}
	}
	int num_res = res.size();
	satisfyCount = 0;
	for (int i = 0; i < traverseLeafPointArray.size(); ++i) {
		if (traverseLeafPointArray[i].first <= searchRange) {
			satisfyCount++;
			candidateResult.emplace_back(traverseLeafPointArray[i]);
		}
	}

	while (!unVisitNodeQueue->empty()) {
		M_Node_St* curNode = unVisitNodeQueue->top().second;
		DATATYPE curLB = unVisitNodeQueue->top().first;
		if (curLB < searchRange && !isCompleteIncrementalRangeSearch(searchRange)) {
			unVisitNodeQueue->pop();
			if (Is_leaf_Node(curNode)) {
				DATATYPE dist = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* dataNode = curNode->ptr_sub_tree[i];
					dist = MyFunc::Cal_Euclidean_distance(dataNode->feature_val, lowQuery);
					traverseLeafPointArray.emplace_back(make_pair(dist, dataNode->id));
					if (dist < searchRange) {
						candidateResult.emplace_back(make_pair(dist, dataNode->id));
						satisfyCount++;
					}
				}
			}
			else {
				DATATYPE nextLB = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* nextNode = curNode->ptr_sub_tree[i];
					nextLB = calLowerBound(lowQuery, nextNode);
					unVisitNodeQueue->push(make_pair(nextLB, nextNode));
				}
			}
		}
		else break;
	}
}

void PM_Tree::improvedRangeSearch(vector<DATATYPE>& highQuery, DataMetric& highData, vector<DATATYPE>& lowQuery, 
	DATATYPE searchRange, vector<pair<DATATYPE, int>>& candidateResult, std::priority_queue<knnPair>& res, DATATYPE& all_cost, std::vector<bool>& visitFlag)
{
	if (!rangeSearchStartFlag) {
		num_verified = 0;
		satisfyCountArray.resize(100, 0);
		rangeSearchStartFlag = true;
		unVisitNodeQueue = initUnVisitNodeQueue();
		unVisitNodeQueue->push(make_pair(0, root));
		/*computing distance between query and pivot*/
		distQueryAndPivot.resize(max(PD, HR));
		for (int i = 0; i < pivot_vec.size(); ++i) {
			distQueryAndPivot[i] = MyFunc::Cal_Euclidean_distance(lowQuery, pivot_vec[i]);
		}
	}
	DATATYPE dist = 0.0f;
	int num_res = res.size();
	knnPair pair0;
	satisfyCount = 0;
	for (int i = 0; i < traverseLeafPointArray.size(); ++i) {
		if (traverseLeafPointArray[i].first <= searchRange) {
			satisfyCount++;
			candidateResult.emplace_back(traverseLeafPointArray[i]);

			if (!visitFlag[traverseLeafPointArray[i].second]) {
				visitFlag[traverseLeafPointArray[i].second] = true;
				all_cost++;
				pair0.id = traverseLeafPointArray[i].second;
				pair0.dist = MyFunc::Cal_Euclidean_distance(highData[traverseLeafPointArray[i].second], highQuery);
				res.push(pair0);
				++num_res;
				if (num_res > Config::KNN) {
					--num_res;
					res.pop();
					minDistKth = res.top().dist;
				}
				//queryResult[i].emplace_back(make_pair(dist, singleQueryResult[j].second));
			}
		}
	}

	while (!unVisitNodeQueue->empty()) {
		M_Node_St* curNode = unVisitNodeQueue->top().second;
		DATATYPE curLB = unVisitNodeQueue->top().first;

		//if (all_cost != satisfyCount || satisfyCount > Config::T) {
		//	cout << all_cost << "," << satisfyCount << endl;
		//}

		if (curLB < searchRange && !isCompleteIncrementalRangeSearch(searchRange)) {
			unVisitNodeQueue->pop();
			if (Is_leaf_Node(curNode)) {
				DATATYPE dist = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* dataNode = curNode->ptr_sub_tree[i];
					dist = MyFunc::Cal_Euclidean_distance(dataNode->feature_val, lowQuery);

					traverseLeafPointArray.emplace_back(make_pair(dist, dataNode->id));
					if (dist < searchRange) {
						candidateResult.emplace_back(make_pair(dist, dataNode->id));
						satisfyCount++;

						visitFlag[dataNode->id] = true;
						all_cost++;
						pair0.id = dataNode->id;
						pair0.dist = MyFunc::Cal_Euclidean_distance(highData[dataNode->id], highQuery);
						res.push(pair0);
						++num_res;
						if (num_res > Config::KNN) {
							--num_res;
							res.pop();
							minDistKth = res.top().dist;
						}
					}
				}
			}
			else {
				DATATYPE nextLB = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* nextNode = curNode->ptr_sub_tree[i];
					nextLB = calLowerBound(lowQuery, nextNode);
					unVisitNodeQueue->push(make_pair(nextLB, nextNode));
				}
			}
		}
		else
			break;
	}
}

void PM_Tree::improvedRangeSearch1(vector<DATATYPE>& highQuery, DataMetric& highData, vector<DATATYPE>& lowQuery,
	DATATYPE searchRange, std::priority_queue<knnPair,std::vector<knnPair>,greater<knnPair>>& buffer, std::priority_queue<knnPair>& res, DATATYPE& all_cost, std::vector<bool>& visitFlag)
{
	if (!rangeSearchStartFlag) {
		rangeSearchStartFlag = true;
		unVisitNodeQueue = initUnVisitNodeQueue();
		unVisitNodeQueue->push(make_pair(0, root));
		/*computing distance between query and pivot*/
		distQueryAndPivot.resize(max(PD, HR));
		for (int i = 0; i < pivot_vec.size(); ++i) {
			distQueryAndPivot[i] = MyFunc::Cal_Euclidean_distance(lowQuery, pivot_vec[i]);
		}
	}
	DATATYPE dist;
	int num_res = res.size();
	knnPair pair0;
	satisfyCount= 0;

	while (!buffer.empty() && buffer.top().dist <= searchRange) {
		visitFlag[buffer.top().id] = true;
		all_cost++;
		pair0 = buffer.top();
		pair0.dist = MyFunc::Cal_Euclidean_distance(highData[pair0.id], highQuery);
		res.push(pair0);
		buffer.pop();
		++num_res;
		if (num_res > Config::KNN) {
			--num_res;
			res.pop();
			minDistKth = res.top().dist;
		}
		if (all_cost > Config::T 
			//|| (minDistKth * Config::t < searchRange)
			) {
			return;
		}
	}

	while (!unVisitNodeQueue->empty()) {
		M_Node_St* curNode = unVisitNodeQueue->top().second;
		DATATYPE curLB = unVisitNodeQueue->top().first;

		if (curLB < searchRange 
			&& all_cost <= Config::T&& minDistKth * Config::t > searchRange) {
			unVisitNodeQueue->pop();
			if (Is_leaf_Node(curNode)) {
				DATATYPE dist = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* dataNode = curNode->ptr_sub_tree[i];
					dist = MyFunc::Cal_Euclidean_distance(dataNode->feature_val, lowQuery);
					pair0.id = dataNode->id;

					if (dist < searchRange) {
						pair0.dist = MyFunc::Cal_Euclidean_distance(highData[dataNode->id], highQuery);
						visitFlag[dataNode->id] = true;
						all_cost++;
						res.push(pair0);
						++num_res;
						if (num_res > Config::KNN) {
							--num_res;
							res.pop();
							minDistKth = res.top().dist;
						}
					}
					else {
						pair0.dist = dist;
						buffer.push(pair0);
					}
				}
			}
			else {
				DATATYPE nextLB = 0;
				for (int i = 0; i < curNode->ptr_sub_tree.size(); ++i) {
					M_Node_St* nextNode = curNode->ptr_sub_tree[i];
					nextLB = calLowerBound(lowQuery, nextNode);
					unVisitNodeQueue->push(make_pair(nextLB, nextNode));
				}
			}
		}
		else {
			return;
		}
	}
}


void PM_Tree::subRange_Search(DataMetric& highData, vector<DATATYPE>& highQuery, M_Node_St ** cur_node_address_, vector<DATATYPE>& q_feature_val_, DATATYPE search_range_, vector<pair<DATATYPE, int>>& res_vec_, DATATYPE &dist_parent_q_, vector<DATATYPE>& dist_q_pivot_)
{
	if (res_vec_.size() > Config::T) return;
	DATATYPE dist = 0;
	M_Node_St* cur_node_ = *cur_node_address_;
	if (Is_leaf_Node(cur_node_)) {
		for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {	
			if (res_vec_.size() > Config::T) return;
			int id = cur_node_->ptr_sub_tree[i]->id;
			if (visitHighDataFlag[id] == 1){
				continue;
			}
			if (fabs(dist_parent_q_ - cur_node_->ptr_sub_tree[i]->dist_to_parent) <= search_range_ && Is_Pivot_Filter(dist_q_pivot_, search_range_, cur_node_->ptr_sub_tree[i])) {
				dist = MyFunc::Cal_Euclidean_distance(cur_node_->ptr_sub_tree[i]->feature_val, q_feature_val_);
				if (dist <= search_range_)
				{
					visitHighDataFlag[id] = 1;
					dist = MyFunc::Cal_Euclidean_distance(highData[id], highQuery, highData[0].size());
					res_vec_.emplace_back(make_pair(dist, id));
				}
			}
		}
	}
	else {
		for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {
			if (res_vec_.size() > Config::T) return;
			if (fabs(dist_parent_q_ - cur_node_->ptr_sub_tree[i]->dist_to_parent) <= search_range_ + cur_node_->ptr_sub_tree[i]->range) {				
				dist = MyFunc::Cal_Euclidean_distance(cur_node_->ptr_sub_tree[i]->feature_val, q_feature_val_);
				if (dist <= search_range_ + cur_node_->ptr_sub_tree[i]->range&&Is_Pivot_Filter(dist_q_pivot_, search_range_, cur_node_->ptr_sub_tree[i])){		
					subRange_Search(highData, highQuery, &(cur_node_->ptr_sub_tree[i]), q_feature_val_, search_range_, res_vec_, dist, dist_q_pivot_);
				}
			}
		}
	}
}

void PM_Tree::Set_Pivot(int pivot_num_, int HR_, int PD_, const vector<vector<DATATYPE>>& pivot_vec_)
{
	this->Piovt_Num = pivot_num_;
	this->HR = HR_;
	this->PD = PD_;
	this->pivot_vec = pivot_vec_;
}

double PM_Tree::Traverse_Get_Volume()
{
	double volume =MyFunc::Cal_Hypersphere_Volume(this->root->range, this->root->feature_val.size());
	Sub_Traverse_Get_Volumem(this->root, volume);
	return volume;
}

void PM_Tree::Update_Level(M_Node_St* cur_node_,int level_)
{
	if (level_ > this->Max_Level)
		this->Max_Level = level_;
	for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {
		cur_node_->ptr_sub_tree[i]->level = level_;
		if(!(Is_Data_Node(cur_node_->ptr_sub_tree[i])))
			Update_Level(cur_node_->ptr_sub_tree[i], level_ + 1);
	}
}

void PM_Tree::Traverse_Bread_Tree(string out_file_path_)
{
	ofstream outfile(out_file_path_);
	queue< M_Node_St *> my_queue;
	my_queue.push(root);
	int cur_level = 0;
	if (outfile) {
		while (!my_queue.empty()) {
			M_Node_St * cur_node_ = my_queue.front();
			my_queue.pop();
			if (cur_node_->level - cur_level >= 2) 
				cout << "error in Traverse_Bread_Tree\n";
			if (cur_node_->level != cur_level) {
				cur_level = cur_node_->level;
			}
			Output_Node_Info(outfile,cur_node_);
			for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i)
				my_queue.push(cur_node_->ptr_sub_tree[i]);
		}
	}
	else {
		cout << "error in Traverse_Bread_Tree\n";
	}
}

void PM_Tree::Output_Node_Info(ofstream & out_, const M_Node_St * cur_node_)
{
	out_ << cur_node_->level<<"  ";
	out_ << cur_node_->feature_val.size() << " ";
	for (int i = 0; i < cur_node_->feature_val.size(); ++i) {
		out_ << cur_node_->feature_val[i] << " ";
	}

	if (this->Piovt_Num == 0) {
		if(Is_Data_Node(cur_node_))
			out_ << 0 << " " << 1 << "     ";
		else
			out_ << cur_node_->range << " " << cur_node_->ptr_sub_tree.size() << "     ";
	}
		
	else {
		if (Is_Data_Node(cur_node_)) {
			out_ << 0 << " " << 1 << "     ";
			for (int i = 0; i < cur_node_->pivot_distance.size(); ++i)
			{
				out_ << cur_node_->pivot_distance[i] << "  " << cur_node_->pivot_distance[i] << "  ";

			}
		}
		else {
			out_ << cur_node_->range << " " << cur_node_->ptr_sub_tree.size() << "     ";
			for (int i = 0; i < cur_node_->hyper_rings.size(); ++i)
			{
				out_ << cur_node_->hyper_rings[i].first << "  " << cur_node_->hyper_rings[i].second << "  ";

			}
		}	
	}
	out_ << endl;
		
}

DATATYPE PM_Tree::calLowerBound(vector<DATATYPE>& lowQuery, M_Node_St* node)
{
	DATATYPE lb_o = max(MyFunc::Cal_Euclidean_distance(lowQuery, node->feature_val) - node->range, (DATATYPE)0.0);
	DATATYPE lb_p = -1;
	for (int i = 0; i < node->hyper_rings.size(); ++i) {
		lb_p = max(max(node->hyper_rings[i].first - distQueryAndPivot[i], (DATATYPE)0.0), distQueryAndPivot[i] - node->hyper_rings[i].second);
	}
	return max(lb_o, lb_p);
}

void PM_Tree::Sub_Traverse_Get_Volumem(M_Node_St* cur_node_,double& volume_)
{
	if (Is_leaf_Node(cur_node_)) return;
	for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {
		double v = MyFunc::Cal_Hypersphere_Volume(cur_node_->ptr_sub_tree[i]->range, cur_node_->ptr_sub_tree[i]->feature_val.size());
		volume_ += v;
		Sub_Traverse_Get_Volumem(cur_node_->ptr_sub_tree[i], volume_);
	}	
}

void PM_Tree::Update_hyper_rings(vector<pair<DATATYPE, DATATYPE>>& hyper_rings_, vector<DATATYPE>& data_feature_val_)
{
	if (hyper_rings_.size() == 0) hyper_rings_.resize(HR, make_pair(INT16_MAX, INT16_MIN));
	for (int i = 0; i <(this->HR); ++i) {
		DATATYPE dist=MyFunc::Cal_Euclidean_distance(data_feature_val_, this->pivot_vec[i]);
		if (dist < hyper_rings_[i].first)hyper_rings_[i].first = dist;
		if (dist > hyper_rings_[i].second)hyper_rings_[i].second = dist;
	}
}

void PM_Tree::Update_pivot_distance(vector<DATATYPE>& pivot_distance_, vector<DATATYPE>& data_feature_val_)
{
	if (pivot_distance_.size() == 0)
		pivot_distance_.resize(PD, 0);
	for (int i = 0; i < (this->PD); ++i) {
		DATATYPE dist = MyFunc::Cal_Euclidean_distance(data_feature_val_, this->pivot_vec[i]);
		pivot_distance_[i] = dist;
	}
}

inline bool PM_Tree::Is_Pivot_Filter(vector<DATATYPE>& dist_q_pivot_, DATATYPE range_, M_Node_St * cur_node_)
{	
	if (Is_Data_Node(cur_node_)) {
		DATATYPE PD_i = 0;
		for (int i = 0; i < PD; ++i) {
			PD_i = cur_node_->pivot_distance[i];
			if (fabs(dist_q_pivot_[i] - PD_i) > range_) {
				Pivot_Filter_Num++;
				return false;
			}	
		}
	}
	else {
		for (int i = 0; i < HR; ++i) {
			if ((dist_q_pivot_[i] - range_ > cur_node_->hyper_rings[i].second) || (dist_q_pivot_[i] + range_) < cur_node_->hyper_rings[i].first) {
				Pivot_Filter_Num++;
				return false;
			}
		}
	}
	return true;
}

void PM_Tree::Insert(M_Node_St ** cur_node_address_, M_Node_St ** insert_node_address_)
{
	M_Node_St *cur_node_ = *cur_node_address_;
	M_Node_St * insert_node_ = *insert_node_address_;
	if (Is_leaf_Node(cur_node_)) {
		if (Is_Full(cur_node_)) {
			Split(&cur_node_, &insert_node_);
		}
		else {
			cur_node_->range = Cal_Cover_Radius(cur_node_, insert_node_);
			cur_node_->ptr_sub_tree.emplace_back(insert_node_);	

			insert_node_->patent_node = cur_node_;
			insert_node_->dist_to_parent = Cal_Dist_To_Parent(insert_node_);
			Update_hyper_rings(cur_node_->hyper_rings, insert_node_->feature_val);
		}
	}
	else {
		M_Node_St** next_node=Get_Next_Node_And_Update_Range(&cur_node_, &insert_node_);
		Update_hyper_rings((*next_node)->hyper_rings, insert_node_->feature_val);
		Insert(next_node, &insert_node_);
	}
}

/*repalce all attributtion of cur_node_ with new_node_'s*/
void PM_Tree::assign_Node_All_Value(M_Node_St * cur_node_, M_Node_St * new_node_)
{
	cur_node_->dist_to_parent = new_node_->dist_to_parent;
	cur_node_->feature_val = new_node_->feature_val;
	cur_node_->id = new_node_->id;
	cur_node_->node_category = new_node_->node_category;
	cur_node_->patent_node = new_node_->patent_node;
	cur_node_->ptr_sub_tree = new_node_->ptr_sub_tree;
	cur_node_->range = new_node_->range;
	cur_node_->pivot_distance = new_node_->pivot_distance;
	cur_node_->hyper_rings = cur_node_->hyper_rings;
	for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {
		cur_node_->ptr_sub_tree[i]->patent_node = cur_node_;
	}
}

/*traverse subnode of cur_node_ and decide the hyper_rings ofcur_node_*/
void PM_Tree::Merge_subNode_HR(M_Node_St * cur_node_)
{
	if (Is_leaf_Node(cur_node_)) {
		for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {
			Update_hyper_rings(cur_node_->hyper_rings, cur_node_->ptr_sub_tree[i]->feature_val);
		}
	}
	else {
		if (cur_node_->hyper_rings.size() == 0)
			cur_node_->hyper_rings.resize(HR, make_pair(INT16_MAX, INT16_MIN));
		for (int i = 0; i < HR; ++i) {
			for (int j = 0; j < cur_node_->ptr_sub_tree.size(); ++j) {
				DATATYPE sub_min;
				DATATYPE sub_max;
				if (cur_node_->ptr_sub_tree[j]->ptr_sub_tree.size() != 0)
				{
					sub_min = cur_node_->ptr_sub_tree[j]->hyper_rings[i].first;
					sub_max = cur_node_->ptr_sub_tree[j]->hyper_rings[i].second;
				}
				else
					continue;
				DATATYPE cur_min = cur_node_->hyper_rings[i].first;
				DATATYPE cur_max = cur_node_->hyper_rings[i].second;
				if (cur_min > sub_min)
					cur_node_->hyper_rings[i].first = cur_node_->ptr_sub_tree[j]->hyper_rings[i].first;
				if (cur_max < sub_max)
					cur_node_->hyper_rings[i].second = cur_node_->ptr_sub_tree[j]->hyper_rings[i].second;
			}
		}
	}
}

M_Node_St ** PM_Tree::Get_Next_Node_And_Update_Range(M_Node_St ** cur_node_address_, M_Node_St ** insert_node_address_)
{
	M_Node_St * cur_node_ = *cur_node_address_;
	M_Node_St * insert_node_ = *insert_node_address_;
	vector<pair<DATATYPE, M_Node_St**>> data_vec;
	vector<pair<DATATYPE, M_Node_St**>> data2_vec;
	for (int i = 0; i < cur_node_->ptr_sub_tree.size(); ++i) {
		/*minmum d(O_r,O_ins)<= r(O_r)*/
		DATATYPE dist = MyFunc::Cal_Euclidean_distance(cur_node_->ptr_sub_tree[i]->feature_val, insert_node_->feature_val);
		if (dist <= cur_node_->ptr_sub_tree[i]->range) {
			data_vec.emplace_back(make_pair(dist, &(cur_node_->ptr_sub_tree[i])));
		}
		/*minmum d(O_r,O_ins)- r(O_r)*/
		if (data_vec.size() == 0) {
			dist -= cur_node_->ptr_sub_tree[i]->range;
			data2_vec.emplace_back(make_pair(dist, &(cur_node_->ptr_sub_tree[i])));
		}
	}

	if (data_vec.size() != 0) {
		sort(data_vec.begin(), data_vec.end());
		return data_vec[0].second;
	}
	sort(data2_vec.begin(), data2_vec.end());
	(*(data2_vec[0].second))->range = data2_vec[0].first+ (*(data2_vec[0].second))->range;//update range
	return data2_vec[0].second;
}


void PM_Tree::Promote(vector<M_Node_St*>& entries_, M_Node_St * node_1_, M_Node_St * node_2_)
{
	switch (promotefunc_e) {
		case RANDOM_e: {
			Promote_Random(entries_, node_1_, node_2_);
		}break;
		case m_RAD_2: {
			Promote_M_RAD_2(entries_, node_1_, node_2_);
		}break;
		default: {

		}
	}

}

/*
	randomly choose two feature value from entries,and let them act as the feature value of node_1 and node_2
	only change the feature information of node_1 and node_2
	two feature value are different
*/
void PM_Tree::Promote_Random(vector<M_Node_St*>& entries_, M_Node_St * node_1_, M_Node_St * node_2_)
{
	default_random_engine generator;
	uniform_int_distribution<int> u_random_int(0, entries_.size() - 1);
	int a=u_random_int(generator);
	int b = -1;
	do {
		b= u_random_int(generator);
	} while (a == b);
	node_1_->feature_val = entries_[a]->feature_val;
	node_2_->feature_val = entries_[b]->feature_val;
}

void PM_Tree::Promote_M_RAD_2(vector<M_Node_St*>& entries_, M_Node_St * node_1_, M_Node_St * node_2_)
{
	DATATYPE minimum = INT32_MAX;
	int a=-1,b=-1;
	for (int i = 0; i < entries_.size(); ++i) {
		for (int j = i+1; j < entries_.size(); ++j) {
			DATATYPE range = minimum_Rad(entries_, i, j);
			if (range < minimum) {
				a = i;
				b = j;
				minimum = range;
			}
		}
	}
	node_1_->feature_val = entries_[a]->feature_val;
	node_2_->feature_val = entries_[b]->feature_val;
}

DATATYPE PM_Tree::minimum_Rad(vector<M_Node_St*>& entries_, int pos1_, int pos2_)
{
	M_Node_St* p_1 = new M_Node_St();
	M_Node_St* p_2 = new M_Node_St();
	p_1->node_category = entries_[0]->node_category;
	p_2->node_category = entries_[0]->node_category;
	p_1->feature_val = entries_[pos1_]->feature_val;
	p_2->feature_val = entries_[pos2_]->feature_val;

	for (int i = 0; i < entries_.size(); ++i) {
		if (i == pos1_ || i == pos2_) 
			continue;

		if (MyFunc::Cal_Euclidean_distance(entries_[i]->feature_val, p_1->feature_val)
			<= MyFunc::Cal_Euclidean_distance(entries_[i]->feature_val, p_2->feature_val))
		{
			p_1->ptr_sub_tree.emplace_back(entries_[i]);
		}
		else {
			p_2->ptr_sub_tree.emplace_back(entries_[i]);
		}
	}
	DATATYPE res_range = 0;
	res_range +=Cal_Cover_Radius(p_1);
	res_range += Cal_Cover_Radius(p_2);
	delete(p_1);
	delete(p_2);
	return res_range;
}

/*
	Dividing entries into node_1 and node_2
	changing all relevant information
*/
void PM_Tree::Partition(vector<M_Node_St*>& entries_, M_Node_St * node_1_, M_Node_St * node_2_)
{
	if (entries_[0]->node_category == 1)
		node_1_->node_category = node_2_->node_category = 0;
	else if (entries_[0]->node_category == 2)
		node_1_->node_category = node_2_->node_category = 1;
	else if (entries_[0]->node_category == 0)
		node_1_->node_category = node_2_->node_category = 0;
	else
		cout << "error in Partition\n";

	int cnt1 = 0, cnt2 = 0;
	int i = 0;
	while (i < entries_.size()&&cnt1 <= M - MIN_CAPACITY && cnt2 <= M - MIN_CAPACITY) {
		if (MyFunc::Cal_Euclidean_distance(entries_[i]->feature_val, node_1_->feature_val)
			<= MyFunc::Cal_Euclidean_distance(entries_[i]->feature_val, node_2_->feature_val))
		{
			node_1_->ptr_sub_tree.emplace_back(entries_[i]);
			entries_[i]->patent_node = node_1_;
			++cnt1;
		}
		else {
			node_2_->ptr_sub_tree.emplace_back(entries_[i]);
			entries_[i]->patent_node = node_2_;
			++cnt2;
		}
		entries_[i]->dist_to_parent = Cal_Dist_To_Parent(entries_[i]);/*update distance*/

		++i;
	}

	if (cnt1 > M - MIN_CAPACITY) {
		for (; i < entries_.size(); ++i) {
			node_2_->ptr_sub_tree.emplace_back(entries_[i]);
			entries_[i]->patent_node = node_2_;
			entries_[i]->dist_to_parent = Cal_Dist_To_Parent(entries_[i]);/*update distance*/
		}
	}
	else {
		for (; i < entries_.size(); ++i) {
			node_1_->ptr_sub_tree.emplace_back(entries_[i]);
			entries_[i]->patent_node = node_1_;
			entries_[i]->dist_to_parent = Cal_Dist_To_Parent(entries_[i]);/*update distance*/
		}
	}

	node_1_->range = Cal_Cover_Radius(node_1_);
	node_2_->range = Cal_Cover_Radius(node_2_);


}

DATATYPE PM_Tree::Cal_Cover_Radius(const M_Node_St * node_)
{
	if (node_->ptr_sub_tree.size() == 0)
		return 0;
	DATATYPE max_range = INT16_MIN;
	if (Is_leaf_Node(node_)) {
		for (int i = 0; i < node_->ptr_sub_tree.size(); ++i) {
			DATATYPE dist = MyFunc::Cal_Euclidean_distance(node_->ptr_sub_tree[i]->feature_val, node_->feature_val);
			if (dist > max_range) max_range = dist;
		}	
	}
	else {
		for (int i = 0; i < node_->ptr_sub_tree.size(); ++i) {
			DATATYPE dist = MyFunc::Cal_Euclidean_distance(node_->ptr_sub_tree[i]->feature_val, node_->feature_val);
			dist += node_->ptr_sub_tree[i]->range;
			if (dist > max_range) max_range = dist;
		}
	}
	return max_range;
}

DATATYPE PM_Tree::Cal_Cover_Radius(const M_Node_St * node_, const M_Node_St * cmp_node_)
{
	DATATYPE dist;
	if (Is_leaf_Node(node_)) {	
		dist = MyFunc::Cal_Euclidean_distance(cmp_node_->feature_val, node_->feature_val);
	}
	else {
		dist = MyFunc::Cal_Euclidean_distance(cmp_node_->feature_val, node_->feature_val);
		dist += cmp_node_->range;
	}	
	if (node_->range < dist)
		return dist;
	else
		return node_->range;
}

DATATYPE PM_Tree::Cal_Dist_To_Parent(const M_Node_St * node_)
{
	DATATYPE dist;
	if(!(node_==NULL || node_->patent_node==NULL))
		dist=MyFunc::Cal_Euclidean_distance(node_->feature_val, node_->patent_node->feature_val);
	else {
		cout << "error In Cal_Dist_To_Parent \n";
			return -1;
	}
	return dist;
}

