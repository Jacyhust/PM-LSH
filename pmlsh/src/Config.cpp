#include"Config.h"
#include "MyFunc.h"
string Config::dataSetName = "mnist";
int Config::lowDim = 15;
int Config::highDim = 59800;
int Config::KNN = 100;
int Config::N = 10000;

string Config::highDataFilePath = "E:\\Dataset_for_c\\" + dataSetName + ".data_all";
string Config::highQueryFilePath = "E:\\Dataset_for_c\\" + dataSetName + ".data_all_query";
string Config::knnResultFilePath = "E:\\Dataset_for_c\\wlg\\" + dataSetName + "_result.txt";

//pivot
int Config::pivotNum = 5;
int Config::pivotRandomCount = 50;

//pm-lsh
DATATYPE Config::c_appro = 1.4;
DATATYPE Config::T = 0.1;
DATATYPE Config::alpha1 = 0.01;
DATATYPE Config::t = MyFunc::Ccal_thresh(Config::lowDim, Config::alpha1);
DATATYPE Config::search_Radius = 3.0;

//PM-Tree
int Config::M_NUM = 16;
int Config::MLeaf = 16;

