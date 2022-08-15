#include "pmLsh.h"
#include "MyFunc.h"
#include <ctime>
#include <queue>


void pmLsh::improvedSearchWithKth(DataMetric& highData, DataMetric& highQueryData, DataMetric& lowQueryData, Real_Result& real_result)
{
	/*ËÑË÷*/
#if defined(unix) || defined(__unix__)
	std::string file = "./experiment/result.csv", file_all = "./experiment/result_all.csv";
#else
	std::string file = "experiment/result.csv", file_all = "experiment/result_all.csv";
#endif
	ofstream outfile(file.c_str(), ios::app);
	ofstream outall(file_all.c_str(), ios::app);
	if (outfile && outall) {
		cout << "\nRUNNING PM-LSH ...\n\n";
		queryResult.clear();
		queryResult.resize(lowQueryData.size());
		vector<DATATYPE> all_recall;
		vector<DATATYPE> all_overRatio;
		DATATYPE all_cost = 0;
		DATATYPE query_time = 0;
		DATATYPE sortTime = 0;
		Timer time;
		time.restart();
		float averageRangeCount = 0;

		progress_display pd(lowQueryData.size());
		for (int i = 0; i < lowQueryData.size(); ++i) {
			DATATYPE cost = 0;
			std::priority_queue<knnPair> cand;
			vector<bool> visitFlag(highData.size(), false);
			queryResult[i].reserve(highData.size());
			vector<pair<DATATYPE, int>> singleQueryResult;
			singleQueryResult.reserve(highData.size());
			DATATYPE range = Config::search_Radius;
			std::priority_queue<knnPair, std::vector<knnPair>, greater<knnPair>> buffer;

			myIndex->reset(highData.size());
			while (!myIndex->isCompleteIncrementalRangeSearch(range)) {
				myIndex->improvedRangeSearch1(highQueryData[i], highData, lowQueryData[i], range, buffer, cand, cost, visitFlag);
				range *= sqrt(Config::c_appro);
				averageRangeCount++;
			}
			queryResult[i].resize(Config::KNN);
			int t = Config::KNN - 1;
			while (!cand.empty()) {
				queryResult[i][t--] = make_pair(cand.top().dist, cand.top().id);
				cand.pop();
			}
			all_cost += cost;

			++pd;
		}
		averageRangeCount = averageRangeCount / lowQueryData.size();
		query_time = time.elapsed();
		all_recall = Metric::calRecallAll(real_result, queryResult);
		all_overRatio = Metric::calOverRatioAll(real_result, queryResult);

		cout << "\nFINISH QUERY!\n\n";

		int id = Config::KNN / 10;

		std::cout << "AVG QUERY TIME:    " << query_time / lowQueryData.size() * 1000 << "ms." << std::endl << std::endl;
		std::cout << "AVG RECALL:        " << all_recall[id] << std::endl;
		std::cout << "AVG RATIO:         " << all_overRatio[id] << std::endl;
		std::cout << "AVG COST:          " << all_cost / highData.size() / lowQueryData.size() << std::endl;
		std::cout << "AVG ROUNDS:        " << averageRangeCount << std::endl;

		time_t now = std::time(0);
		time_t zero_point = 1635153971;//Let me set the time 2021.10.25. 17:27 as the zero point
		float date = ((float)(now - zero_point)) / 86400;
		outfile.seekp(0, std::ios_base::end); 
		int tmp = (int)outfile.tellp();
		if (tmp == 0) {
			outfile << "Dataset,c,k,m,M_NUM,PIVOT,Q_NUM,R_MIN,RATIO,RECALL,AVG_TIME,COST,DATE" << std::endl;
		}
		outall.seekp(0, std::ios_base::end);
		tmp = (int)outall.tellp();
		if (tmp == 0) {
			outall << "Dataset,c,k,m,M_NUM,PIVOT,Q_NUM,R_MIN,RATIO,RECALL,AVG_TIME,COST,DATE" << std::endl;
		}

		int knn[] = { 1,10,20,30,40,50,60,70,80,90,100 };
		for (int t = 0; t < 11; t++)
		{
			
			vector<DATATYPE> outData;
			
			outData.emplace_back(Config::c_appro);
			outData.emplace_back(knn[t]);
			outData.emplace_back(Config::lowDim);
			outData.emplace_back(Config::M_NUM);
			outData.emplace_back(Config::pivotNum);
			outData.emplace_back(lowQueryData.size());
			outData.emplace_back(Config::search_Radius);

			outData.emplace_back(all_overRatio[t]);
			outData.emplace_back(all_recall[t]);
			outData.emplace_back(query_time / lowQueryData.size() * 1000);
			outData.emplace_back(DATATYPE(all_cost) / highData.size() / highQueryData.size());
			outData.emplace_back(date);
			
			if (knn[t] == 50) {
				MyFunc::Printf_Result_Csv(outfile, outData);
			}
			MyFunc::Printf_Result_Csv(outall, outData);
		}
	}
	else {
		cout << "error in ofstream outfile(experiment/result.txt) \n";
	}
}


void pmLsh::constructIndex(DataMetric& lowData)
{
	cout << "BUILDING INDEX...\n";
	Timer timer;
	vector<vector<DATATYPE>> pivot_vec = Pivot::Random_pivot(lowData, Config::pivotNum, Config::pivotRandomCount);
	myIndex->Set_Pivot(Config::pivotNum, Config::pivotNum, Config::pivotNum, pivot_vec);
	progress_display pd(lowData.size());
	for (int i = 0; i < lowData.size(); ++i) {
		myIndex->Insert(lowData[i], i);
		++pd;
	}
	cout << "FINISH BUILDING WITH TIME: " << timer.elapsed() << " s.\n";
	//printf("The whole volume is: %f\n", myIndex.Traverse_Get_Volume());
}

