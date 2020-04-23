#pragma once
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<random>
#include<fstream>
#include<string>
#include<sstream>

#define DATANUM 800	//���̓f�[�^��
#define DIM 2	//���̓f�[�^����
#define CLASSNUM 4	//�N���X��
#define COMPONENTNUM 2	//�R���|�[�l���g��
#define STUDYRATE 0.01	//�w�K��
#define COUNTMAX 5000	//�ő厎�s��
#define Threshold 0.001	//�������l

using namespace std;

void LLGMN(vector<vector<double>> Tx, vector<vector<double>> Ty, vector<vector<vector<double>>>& w, int dataN, int classNum, int componentNum, int H, int dimention);
void forward(vector<vector<double>> x, vector<vector<double>>& X, vector<vector<double>>& Y, vector<vector<vector<double>>>& SndIn, vector<vector<vector<double>>>& SndOut, int dataSize, vector<vector<vector<double>>>& w, int classNum, int componentNum, int dimention, int H);
void backword(vector<vector<double>> X, vector<vector<double>> Y, vector<vector<vector<double>>> SndOut, vector<vector<double>> Ty, int dataSize, vector<vector<vector<double>>>& w, int H, int classNum, int componentNum);
void wInit(vector<vector<vector<double>>>& w, int H, int classNum, int componentNum);
double callocError(vector<vector<double>> Y, vector<vector<double>> Ty, int dataN, int classNum);
