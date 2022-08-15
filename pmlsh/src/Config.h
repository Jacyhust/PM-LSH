#pragma once
#include<string>
#include<assert.h>
#include<iostream>

#define DATATYPE float

using namespace std;

class Config
{
public:
	Config() {}
	~Config() {}
	static int lowDim;
	static int highDim;
	static int N;
	static string dataSetName;
	static string highDataFilePath;
	static string highQueryFilePath;
	static string knnResultFilePath;
	
	static string lowDataFilePath;
	static string lowQueryFilePath;
	//pivot
	static int pivotNum;
	static int pivotRandomCount;

	//pm-lsh
    static DATATYPE c_appro;
	static DATATYPE T;
	static DATATYPE t;
	static DATATYPE alpha1;
	static DATATYPE search_Radius;
	static int KNN;

	//pm-tree
	static int M_NUM;
	static int MLeaf;

private:

};

