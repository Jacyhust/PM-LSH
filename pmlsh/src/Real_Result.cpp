#include "Real_Result.h"
#include <algorithm>
#include "DataMetric.h"
#include "MyFunc.h"
#include "Timer.h"
#include "Config.h"

void Real_Result::cal_knn_result(const string& result_Folder_path, DataMetric& raw_data_, DataMetric& query_data_, int knn_, int dim_)
{
	ifstream in(result_Folder_path);
	if (in) {
		in.close();
		return;
	}
	knn_ = 100;
	int qn = 100;
	cout << "COMPUTER BENCHMAKE...\n";
	Timer timer;
	vector<vector<pair<DATATYPE, int>>> sort_vec;
	sort_vec.resize(query_data_.size());
	progress_display pd(qn);
	for (int i = 0; i < qn; ++i){
		for (int j = 0; j < raw_data_.size(); ++j)	{ //compute the exact kNN
			DATATYPE dist = MyFunc::Cal_Euclidean_distance(raw_data_[j], query_data_[i]);
			sort_vec[i].emplace_back(make_pair(dist, j));
		}
		sort(sort_vec[i].begin(), sort_vec[i].end());
		if (sort_vec[i].size() > knn_)
			sort_vec[i].resize(knn_);
		else{
			cout << "sort_vec[i] error\n";
		}
		++pd;
	}
	/*Store the benchmark*/
	ofstream outfile(result_Folder_path);
	if (!outfile)
	{
		cout << "Fail to open the file cal_knn_result" << endl;
	}
	else
	{
		outfile << qn << ' ' << knn_ << endl;
		for (int i = 0; i < sort_vec.size(); i++)
		{
			for (int j = 0; j < sort_vec[i].size(); j++)				
			{
				outfile << sort_vec[i][j].second << '\t' << setprecision(16) << sort_vec[i][j].first << '\t';
			}
			outfile << endl;
		}
	}
	outfile.close();
	cout << "FINISH COMPUTERING WITH TIME: " << timer.elapsed() << " s.\n\n";
}

void Real_Result::load(const string & data_Folder_path)
{
	ifstream infile(data_Folder_path);
	if (!infile){
		cout << "Load_real_file error" << endl;
	}
	else{
		int read_q_num = 0, read_k_num = 0;
		infile >> read_q_num >> read_k_num;
		ground_truth.resize(read_q_num);
		for (int i = 0; i < read_q_num; ++i)
		{
			for (int j = 0; j < read_k_num; j++)
			{
				int key = 0;
				DATATYPE dis = 0;
				infile >> key;
				infile >> dis;
				ground_truth[i].emplace_back(make_pair(dis, key));
			}
		}
		infile.close();
	}
}


