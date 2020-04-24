#include "header.h"

int main()
{
    int dimention = DIM;
    int classNum = CLASSNUM;
    int componentNum = COMPONENTNUM;
    int dataN = DATANUM;
    int studyrate = STUDYRATE;
    int countmax = COUNTMAX;
    //教師データ読み込み
    vector<vector<double>> Tx(dataN, vector<double>(dimention));  //教師入力x[dataN][dimention] = x[800][2]
    vector<vector<double>> Ty(dataN, vector<double>(classNum, 0));  //教師出力y[dataN][class] = y[800][4]
    vector<vector<double>> Dx(dataN, vector<double>(dimention));  //識別入力x[dataN][dimention] = x[800][2]
    vector<vector<double>> Dy(dataN, vector<double>(classNum, 0));  //識別出力y[dataN][class] = y[800][4]
    vector<vector<vector<double>>> w;   //重み w[h][class][component]

    ifstream ifs("lea_sig.csv");
    ifstream ifsT("lea_T_sig.csv");
    ifstream ifsDis("dis_sig.csv");
    ifstream ifsDisT("dis_T_sig.csv");
    string str;
    int n = 0;
    
    if (ifs.fail()) {
        cout<< "Failed to open lea_sig.csv." << endl;
        return -1;
    }

    if (ifsT.fail()) {
        cout << "Failed to open lea_T_sig.csv." << endl;
        return -1;
    }

    if (ifsDis.fail()) {
        cout << "Failed to open dis_sig.csv." << endl;
        return -1;
    }

    if (ifsDisT.fail()) {
        cout << "Failed to open dis_T_sig.csv." << endl;
        return -1;
    }
    
    //教師入力データ読み込み
    while (getline(ifs, str)) {
        istringstream line(str);
        string s;
        int d = 0;
        while (getline(line, s, ',')) {
            double tmp = stod(s);
            Tx[n][d] = tmp;
            //cout << "Tx(" << n << "," << d << ") :" << Tx[n][d] << endl;
            d++;
        }
        n++;
    }   //読み込み終了

    //教師出力データ読み込み
    n = 0;
    while (getline(ifsT, str)) {
        istringstream line(str);
        string s;
        int d = 0;
        while (getline(line, s, ',')) {
            double tmp = stod(s);
            Ty[n][d] = tmp;
            d++;
        }
        n++;
    }   //読み込み終了

    //識別入力データ読み込み
    n = 0;
    while (getline(ifsDis, str)) {
        istringstream line(str);
        string s;
        int d = 0;
        while (getline(line, s, ',')) {
            double tmp = stod(s);
            Dx[n][d] = tmp;
            //cout << "Dx(" << n << "," << d << ") :" << Dx[n][d] << endl;
            d++;
        }
        n++;
    }   //読み込み終了

    //識別出力データ読み込み
    n = 0;
    while (getline(ifsDisT, str)) {
        istringstream line(str);
        string s;
        int d = 0;
        while (getline(line, s, ',')) {
            double tmp = stod(s);
            Dy[n][d] = tmp;
            //cout << "Dy(" << n << "," << d << ") :" << Dy[n][d] << endl;
            d++;
        }
        n++;
    }   //読み込み終了

    int H = 1 + (dimention * dimention + 3 * dimention) / 2;
    LLGMN(Tx, Ty, w, dataN, classNum, componentNum, H, dimention);

    //学習が終わったら識別用データを通す
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

    forward(Dx, X, Y, SndIn, SndOut, dataN, w, classNum, componentNum, dimention, H);
    double error;
    error = callocError(Y, Dy, dataN, classNum);
    cout << "識別用データ入力結果:" << endl;
    cout << "error: " << error << endl;
    return 0;
}
