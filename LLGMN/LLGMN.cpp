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
    vector<vector<double>> Tx(dataN,vector<double>(dimention));  //教師入力x[dataN][dimention] = x[800][2]
    vector<vector<double>> Ty(dataN, vector<double>(classNum, 0));  //教師出力y[dataN][class] = y[800][4]
    vector<vector<vector<double>>> w;   //重み w[h][class][component]

    ifstream ifs("lea_sig.csv");
    string str;
    int n = 0;
    
    if (ifs.fail()) {
        cout<< "Failed to open file." << endl;
        return -1;
    }
    
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

    //教師出力データ作成
    //y[][0] = 1 (n<200), y[][1] = 1 (n<400), y[][2] = 1 (n<600), y[][3] = 1 (n<800) 他は0
    for (int n = 0; n < dataN; n++) {
        Ty[n][n / 200] = 1;
    }
    //for (n = 0; n < dataN; n++) {
    //    cout << "#" << n << ": ";
    //    for (int k = 0; k < classNum; k++) {
    //        cout << Ty[n][k] << " ";
    //    }
    //    cout << endl;
    //}

    int H = 1 + (dimention * dimention + 3 * dimention) / 2;
    LLGMN(Tx, Ty, w, dataN, classNum, componentNum, H, dimention);
    return 0;
}
