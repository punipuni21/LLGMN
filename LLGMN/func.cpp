#include"header.h"

void LLGMN(vector<vector<double>> Tx, vector<vector<double>> Ty, vector<vector<vector<double>>>& w,int dataN, int classNum, int componentNum, int H, int dimention) {
    vector<vector<double>> X(dataN, vector<double>(H)); //X[dataN][H]
    vector<vector<double>> Y(dataN, vector<double>(classNum)); //Y[dataN][class]

    vector<vector<vector<double>>> SndIn;
    vector<vector<vector<double>>> SndOut;   //第2層入出力
    //SndIn, Outの領域確保
    SndIn.resize(dataN);    SndOut.resize(dataN);
    for (int n = 0; n < dataN; n++) {
        SndIn[n].resize(classNum);   SndOut[n].resize(classNum);
        for (int k = 0; k < classNum; k++) {
            SndIn[n][k].resize(componentNum);   SndOut[n][k].resize(componentNum);
        }
    }
    //重み初期化
    wInit(w, H, classNum, componentNum);

    for(int count = 0; count < COUNTMAX; count++){
        double error;
        forward(Tx, X, Y, SndIn, SndOut, dataN, w, classNum, componentNum, dimention, H);
        error = callocError(Y, Ty, dataN, classNum);
        cout << "#" << count << ": " << error << endl;
        if (error < Threshold)   break;
        backword(X, Y, SndOut, Ty, dataN, w, H, classNum, componentNum);
    }
    
    return;
}

void forward(vector<vector<double>> x, vector<vector<double>>& X, vector<vector<double>>& Y, vector<vector<vector<double>>>& SndIn, vector<vector<vector<double>>>& SndOut, int dataSize, vector<vector<vector<double>>>& w, int classNum, int componentNum, int dimention, int H) {
    //Snd[dataN][class][component]
    double deno = 0;    //出力計算の際の分母

    //毎回初期化
    for (int n = 0; n < dataSize; n++) {
        for (int k = 0; k < classNum; k++) {
            for (int m = 0; m < componentNum; m++) {
                SndIn[n][k][m] = 0; SndOut[n][k][m] = 0;
            }
        }
    }
    for (int dataN = 0; dataN < dataSize; dataN++) {    //dataN:データセット番号
        int g;
        for (int l = 0; l < 3; l++) {   //l:層の数を表す

            switch (l) {
            case 0: //入力層
                //非線形変換
                g = 0;
                X[dataN][g] = 1;
                g++;
                for (int i = 0; i < dimention; i++) {
                    X[dataN][g] = x[dataN][i];
                    g++;
                }
                for (int i = 0; i < dimention; i++) {
                    for (int j = i; j < dimention; j++) {
                        X[dataN][g] = x[dataN][i] * x[dataN][j];
                        g++;
                        //X[dataN].push_back(x[dataN][i] * x[dataN][j]);
                    }
                }   //非線形変換終了
                //(1)Ij =(1)Oj = Xj
                break;

            case 1: //中間層
                //重み:w[h][class;k][component:m]
                //重みの初期化はLLGMN()ですでに行ってある

                //入力SndInの計算
                //for (int k = 0; k < classNum; k++) {
                //    for (int m = 0; m < componentNum; m++) {
                //        SndIn[dataN][k][m] = 0;
                //    }
                //}
                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNumはクラスごとに違うかも
                        for (int h = 0; h < H; h++) {
                            SndIn[dataN][k][m] += X[dataN][h] * w[h][k][m];
                        }
                    }
                }
                //出力SndOutの計算
                //出力計算式の分母を計算
                deno = 0;
                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNumはクラスごとに違うかも
                        deno += exp(SndIn[dataN][k][m]);
                    }
                }

                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNumはクラスごとに違うかも
                        SndOut[dataN][k][m] = exp(SndIn[dataN][k][m]) / deno;
                    }
                }
                break;

            case 2: //出力層
                //すべての入力を一度0にする
                for (int k = 0; k < classNum; k++) {
                    Y[dataN] [k] = 0;
                }
                //入力かつ出力の計算
                //要修正！
                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNumはクラスごとに違うかも
                        Y[dataN][k] += SndOut[dataN][k][m];
                    }
                }
                
                break;
            }
        }
    }
    return;
}

void backword(vector<vector<double>> X, vector<vector<double>> Y, vector<vector<vector<double>>> SndOut, vector<vector<double>> Ty, int dataSize, vector<vector<vector<double>>>& w, int H, int classNum, int componentNum) {
    double studyrate = STUDYRATE;
    vector<vector<vector<double>>> dJdw;    //dJdw[H][classNum][componentNum]

    //領域の確保
    dJdw.resize(H);
    for (int h = 0; h < H; h++) {
        dJdw[h].resize(classNum);
        for (int k = 0; k < classNum; k++) {
            dJdw[h][k].resize(componentNum);
        }
    }

    //dJdw[h][k][m]:dJdw[h][k][m][n]の総和(n:データ数)
    for (int h = 0; h < H; h++) {
        for (int k = 0; k < classNum; k++) {
            for (int m = 0; m < componentNum; m++) {
                dJdw[h][k][m] = 0;  //まず値を0に初期化
                for (int n = 0; n < dataSize; n++) {
                    dJdw[h][k][m] += (Y[n][k] - Ty[n][k]) * SndOut[n][k][m] / Y[n][k] * X[n][h];
                }
                w[h][k][m] -= studyrate * dJdw[h][k][m];
            }
        }
    }
    //最終クラス最終コンポーネントへの重みはすべてゼロ
    for (int h = 0; h < H; h++) {
        w[h][classNum - 1][componentNum - 1] = 0;
    }
    return;
}

//重み初期化関数
void wInit(vector<vector<vector<double>>>& w, int H, int classNum, int componentNum) {
    
    //wのサイズを事前に定義しておく必要がある
    w.resize(H);
    for (int h = 0; h < H; h++) {
        w[h].resize(classNum);
        for (int k = 0; k < classNum; k++) {
            w[h][k].resize(componentNum);
        }
    }

    //-1から1の範囲で重みをランダムに設定
    random_device rnd;
    mt19937 mt(rnd());
    uniform_real_distribution<> rand1(-1.0, 1.0);
    for (int h = 0; h < H; h++) {
        for (int k = 0; k < classNum; k++) {
            for (int m = 0; m < componentNum; m++) {
                w[h][k][m] = rand1(mt);
            }
        }
    }

    //最終クラス最終コンポーネントへの重みはすべてゼロ
    for (int h = 0; h < H; h++) {
        w[h][classNum - 1][componentNum - 1] = 0;
    }
	return;
}

//誤差計算
double callocError(vector<vector<double>> Y, vector<vector<double>> Ty, int dataN, int classNum) {
    double error = 0;
    for (int n = 0; n < dataN; n++) {
        for (int k = 0; k < classNum; k++) {
            error += -1 * Ty[n][k] * log(Y[n][k]);
        }
    }
    return error;
}