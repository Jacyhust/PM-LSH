#pragma once
#include <vector>
#include <algorithm>
#include <fstream>
#include <boost/math/distributions/chi_squared.hpp>
#include "Config.h"

#if defined(unix) || defined(__unix__)

#else
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif


using namespace std;
class MyFunc
{
public:
	static DATATYPE  Ccal_thresh(int lowDim, DATATYPE alpha1)
	{
		boost::math::chi_squared chi(lowDim);
		return sqrt(boost::math::quantile(chi, 1 - alpha1));

	}
	static DATATYPE Cal_Euclidean_distance(const DATATYPE * a, const DATATYPE * b, int dim);
	static DATATYPE Cal_Euclidean_distance(const vector<DATATYPE>& a,const  vector<DATATYPE>& b);
	static void Printf_Result_Csv(ofstream& out, vector<DATATYPE>& outData)
	{
		out << Config::dataSetName << ",";
		for (int i = 0; i < outData.size(); ++i) {
			out << outData[i] << ",";
		}
		out << endl;
	}

	static void Printf_Result_Csv(ofstream& out, vector<string>& outData)
	{
		for (int i = 0; i < outData.size(); ++i) {
			out << outData[i] << ",";
		}
		out << endl;
	}

	static bool IsBoxCircleIntersec(vector<DATATYPE>&rec_center_, const vector<DATATYPE>&ball_center_, vector<DATATYPE>& right_top_pos_, DATATYPE radius_)
	{
		int dim = rec_center_.size();
		DATATYPE dist = 0;
		vector<DATATYPE> v;
		vector<DATATYPE> h;
		vector<DATATYPE> u;
		for (int i = 0; i < dim; ++i) {
			v.emplace_back(fabs(rec_center_[i] - ball_center_[i]));
		}
		for (int i = 0; i < dim; ++i) {
			h.emplace_back(right_top_pos_[i] - rec_center_[i]);
		}
		for (int i = 0; i < dim; ++i) {
			u.emplace_back(max(v[i] - h[i], (DATATYPE)0.0));
		}
		for (int i = 0; i < dim; ++i) {
			dist += u[i] * u[i];
		}

		return dist <= radius_ * radius_;
	}
	static inline DATATYPE Cal_Euclidean_distance(const vector<DATATYPE>& a, const  vector<DATATYPE>& b, int dim_) {
		int d = dim_;
		DATATYPE dis;
		switch (d) {
		case 15:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]) + (a[9] - b[9]) * (a[9] - b[9])
				+ (a[10] - b[10]) * (a[10] - b[10]) + (a[11] - b[11]) * (a[11] - b[11])
				+ (a[12] - b[12]) * (a[12] - b[12]) + (a[13] - b[13]) * (a[13] - b[13])
				+ (a[14] - b[14]) * (a[14] - b[14]);
			return sqrt(dis);
			break;
		case 6:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5]);
			return sqrt(dis);
			break;
		case 7:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]);
			return sqrt(dis);
			break;
		case 8:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7]);
			return sqrt(dis);
			break;
		case 9:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]);
			return sqrt(dis);
			break;
		case 10:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]) + (a[9] - b[9]) * (a[9] - b[9]);
			return sqrt(dis);
			break;
		case 11:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]) + (a[9] - b[9]) * (a[9] - b[9])
				+ (a[10] - b[10]) * (a[10] - b[10]);
			return sqrt(dis);
			break;
		case 12:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]) + (a[9] - b[9]) * (a[9] - b[9])
				+ (a[10] - b[10]) * (a[10] - b[10]) + (a[11] - b[11]) * (a[11] - b[11]);
			return sqrt(dis);
			break;
		case 13:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]) + (a[9] - b[9]) * (a[9] - b[9])
				+ (a[10] - b[10]) * (a[10] - b[10]) + (a[11] - b[11]) * (a[11] - b[11])
				+ (a[12] - b[12]) * (a[12] - b[12]);
			return sqrt(dis);
			break;
		case 14:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]) + (a[5] - b[5]) * (a[5] - b[5])
				+ (a[6] - b[6]) * (a[6] - b[6]) + (a[7] - b[7]) * (a[7] - b[7])
				+ (a[8] - b[8]) * (a[8] - b[8]) + (a[9] - b[9]) * (a[9] - b[9])
				+ (a[10] - b[10]) * (a[10] - b[10]) + (a[11] - b[11]) * (a[11] - b[11])
				+ (a[12] - b[12]) * (a[12] - b[12]) + (a[13] - b[13]) * (a[13] - b[13]);
			return sqrt(dis);
			break;
		case 1:
			dis = (a[0] - b[0]) * (a[0] - b[0]);
			return sqrt(dis);
			break;
		case 2:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]);
			return sqrt(dis);
			break;
		case 3:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]);
			return sqrt(dis);
			break;
		case 4:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]);
			return sqrt(dis);
			break;
		case 5:
			dis = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1])
				+ (a[2] - b[2]) * (a[2] - b[2]) + (a[3] - b[3]) * (a[3] - b[3])
				+ (a[4] - b[4]) * (a[4] - b[4]);
			return sqrt(dis);
			break;
		default:
			break;
		}
		int len = 10;
		int iter = d / len, last = d % len;
		DATATYPE res = 0;
		for (int i = 0; i < iter; ++i) {
			res += (a[0 + i * len] - b[0 + i * len]) * (a[0 + i * len] - b[0 + i * len])
				+ (a[1 + i * len] - b[1 + i * len]) * (a[1 + i * len] - b[1 + i * len])
				+ (a[2 + i * len] - b[2 + i * len]) * (a[2 + i * len] - b[2 + i * len])
				+ (a[3 + i * len] - b[3 + i * len]) * (a[3 + i * len] - b[3 + i * len])
				+ (a[4 + i * len] - b[4 + i * len]) * (a[4 + i * len] - b[4 + i * len])
				+ (a[5 + i * len] - b[5 + i * len]) * (a[5 + i * len] - b[5 + i * len])
				+ (a[6 + i * len] - b[6 + i * len]) * (a[6 + i * len] - b[6 + i * len])
				+ (a[7 + i * len] - b[7 + i * len]) * (a[7 + i * len] - b[7 + i * len])
				+ (a[8 + i * len] - b[8 + i * len]) * (a[8 + i * len] - b[8 + i * len])
				+ (a[9 + i * len] - b[9 + i * len]) * (a[9 + i * len] - b[9 + i * len]);
		}
		for (int i = 0; i < last; ++i) {
			res += (a[i + len * iter] - b[i + len * iter]) * (a[i + len * iter] - b[i + len * iter]);
		}
		return sqrt(res);
	}
	static double Cal_Hypersphere_Volume(DATATYPE radius_, int dim_);


	MyFunc() {};
	~MyFunc() {};
	
	
};

struct knnPair
{
	DATATYPE dist;
	int id;
	bool operator < (const knnPair& rhs) const { return dist < rhs.dist; }
	bool operator > (const knnPair& rhs) const { return dist > rhs.dist; }
};





