#include "MyFunc.h"
#include <iostream>
#include<algorithm>
#define _USE_MATH_DEFINES
#include<math.h>

using namespace std;


 DATATYPE MyFunc::Cal_Euclidean_distance(const DATATYPE* a, const DATATYPE* b, int dim)
{
	DATATYPE sum = 0;
	for (int i = 0; i < dim; ++i) {
		sum += (a[i] - b[i])*(a[i] - b[i]);
	}
	return sqrt(sum);
}

DATATYPE MyFunc::Cal_Euclidean_distance(const vector<DATATYPE>& a, const vector<DATATYPE>& b)
{
	if (a.size() != b.size()) {
		cout << "the size don't match in Cal_Euclidean_distance" << endl; 
	}
	DATATYPE sum = 0;
	int m=a.size();
	for (int i = 0; i < m; ++i) {
		sum += (a[i] - b[i])*(a[i] - b[i]);
	}
	return sqrt(sum);
}


double MyFunc::Cal_Hypersphere_Volume(DATATYPE radius_, int dim_)
{
	double a = 2 * pow(M_PI, double(dim_) /2.0);
	double b = double(dim_) * tgamma(double(dim_) / 2.0);
	double c = pow((double)radius_, double(dim_));
	return (a*c)/b;
}
