#pragma once
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<random>
#include<fstream>
#include<string>
#include<sstream>

#define DATANUM 800	//入力データ数
#define DIM 2	//入力データ次元
#define CLASSNUM 4	//クラス数
#define COMPONENTNUM 2	//コンポーネント数
#define STUDYRATE 0.01	//学習率
#define COUNTMAX 5000	//最大試行回数
#define Threshold 0.001	//しきい値

using namespace std;

void LLGMN(vector<vector<double>> Tx, vector<vector<double>> Ty, vector<vector<vector<double>>>& w, int dataN, int classNum, int componentNum, int H, int dimention);
void forward(vector<vector<double>> x, vector<vector<double>>& X, vector<vector<double>>& Y, vector<vector<vector<double>>>& SndIn, vector<vector<vector<double>>>& SndOut, int dataSize, vector<vector<vector<double>>>& w, int classNum, int componentNum, int dimention, int H);
void backword(vector<vector<double>> X, vector<vector<double>> Y, vector<vector<vector<double>>> SndOut, vector<vector<double>> Ty, int dataSize, vector<vector<vector<double>>>& w, int H, int classNum, int componentNum);
void wInit(vector<vector<vector<double>>>& w, int H, int classNum, int componentNum);
double callocError(vector<vector<double>> Y, vector<vector<double>> Ty, int dataN, int classNum);
