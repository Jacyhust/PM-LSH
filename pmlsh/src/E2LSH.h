#pragma once
#include<vector>
#include<assert.h>
#include<map>
#include <random>
#include <iostream>
#include <functional>
#include <ctime>
#include "Config.h"
#include <iomanip>
using namespace std;

class E2LSH
{
public:
	E2LSH() {}
	~E2LSH(){}
	void constructTable(Config & config);
	vector<DATATYPE> getHashVal(const vector<DATATYPE>& domin, Config& config) const;
private:
	void constructParam(Config& config);
	vector<vector<DATATYPE>>hash_a;  //a*o

	DATATYPE calInnerProduct(vector<DATATYPE>& a, vector<DATATYPE>& b) {
		int dim = a.size();
		DATATYPE res = 0.0;
		for (int i = 0; i < dim; ++i) {
			res += a[i] * b[i];
		}
		return res;
	}

public:
	void showInfo(){
		int m = hash_a.size();
		int d = hash_a[0].size();
		vector<vector<double>> res(m, vector<double>(m));
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < m; ++j)
				res[i][j] = calInnerProduct(hash_a[i], hash_a[j]);
		}

		cout << "The inner products are:\n";
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < m; ++j)
				cout << setw(12) << res[i][j] << "\t";
			cout << "\n";
		}
		exit(0);
	}
};

