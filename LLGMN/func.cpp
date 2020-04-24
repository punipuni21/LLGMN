#include"header.h"

void LLGMN(vector<vector<double>> Tx, vector<vector<double>> Ty, vector<vector<vector<double>>>& w,int dataN, int classNum, int componentNum, int H, int dimention) {
    vector<vector<double>> X(dataN, vector<double>(H)); //X[dataN][H]
    vector<vector<double>> Y(dataN, vector<double>(classNum)); //Y[dataN][class]

    vector<vector<vector<double>>> SndIn;
    vector<vector<vector<double>>> SndOut;   //��2�w���o��
    //SndIn, Out�̗̈�m��
    SndIn.resize(dataN);    SndOut.resize(dataN);
    for (int n = 0; n < dataN; n++) {
        SndIn[n].resize(classNum);   SndOut[n].resize(classNum);
        for (int k = 0; k < classNum; k++) {
            SndIn[n][k].resize(componentNum);   SndOut[n][k].resize(componentNum);
        }
    }
    //�d�ݏ�����
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
    double deno = 0;    //�o�͌v�Z�̍ۂ̕���

    //���񏉊���
    for (int n = 0; n < dataSize; n++) {
        for (int k = 0; k < classNum; k++) {
            for (int m = 0; m < componentNum; m++) {
                SndIn[n][k][m] = 0; SndOut[n][k][m] = 0;
            }
        }
    }
    for (int dataN = 0; dataN < dataSize; dataN++) {    //dataN:�f�[�^�Z�b�g�ԍ�
        int g;
        for (int l = 0; l < 3; l++) {   //l:�w�̐���\��

            switch (l) {
            case 0: //���͑w
                //����`�ϊ�
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
                }   //����`�ϊ��I��
                //(1)Ij =(1)Oj = Xj
                break;

            case 1: //���ԑw
                //�d��:w[h][class;k][component:m]
                //�d�݂̏�������LLGMN()�ł��łɍs���Ă���

                //����SndIn�̌v�Z
                //for (int k = 0; k < classNum; k++) {
                //    for (int m = 0; m < componentNum; m++) {
                //        SndIn[dataN][k][m] = 0;
                //    }
                //}
                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNum�̓N���X���ƂɈႤ����
                        for (int h = 0; h < H; h++) {
                            SndIn[dataN][k][m] += X[dataN][h] * w[h][k][m];
                        }
                    }
                }
                //�o��SndOut�̌v�Z
                //�o�͌v�Z���̕�����v�Z
                deno = 0;
                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNum�̓N���X���ƂɈႤ����
                        deno += exp(SndIn[dataN][k][m]);
                    }
                }

                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNum�̓N���X���ƂɈႤ����
                        SndOut[dataN][k][m] = exp(SndIn[dataN][k][m]) / deno;
                    }
                }
                break;

            case 2: //�o�͑w
                //���ׂĂ̓��͂���x0�ɂ���
                for (int k = 0; k < classNum; k++) {
                    Y[dataN] [k] = 0;
                }
                //���͂��o�͂̌v�Z
                //�v�C���I
                for (int k = 0; k < classNum; k++) {
                    for (int m = 0; m < componentNum; m++) {    //componentNum�̓N���X���ƂɈႤ����
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

    //�̈�̊m��
    dJdw.resize(H);
    for (int h = 0; h < H; h++) {
        dJdw[h].resize(classNum);
        for (int k = 0; k < classNum; k++) {
            dJdw[h][k].resize(componentNum);
        }
    }

    //dJdw[h][k][m]:dJdw[h][k][m][n]�̑��a(n:�f�[�^��)
    for (int h = 0; h < H; h++) {
        for (int k = 0; k < classNum; k++) {
            for (int m = 0; m < componentNum; m++) {
                dJdw[h][k][m] = 0;  //�܂��l��0�ɏ�����
                for (int n = 0; n < dataSize; n++) {
                    dJdw[h][k][m] += (Y[n][k] - Ty[n][k]) * SndOut[n][k][m] / Y[n][k] * X[n][h];
                }
                w[h][k][m] -= studyrate * dJdw[h][k][m];
            }
        }
    }
    //�ŏI�N���X�ŏI�R���|�[�l���g�ւ̏d�݂͂��ׂă[��
    for (int h = 0; h < H; h++) {
        w[h][classNum - 1][componentNum - 1] = 0;
    }
    return;
}

//�d�ݏ������֐�
void wInit(vector<vector<vector<double>>>& w, int H, int classNum, int componentNum) {
    
    //w�̃T�C�Y�����O�ɒ�`���Ă����K�v������
    w.resize(H);
    for (int h = 0; h < H; h++) {
        w[h].resize(classNum);
        for (int k = 0; k < classNum; k++) {
            w[h][k].resize(componentNum);
        }
    }

    //-1����1�͈̔͂ŏd�݂������_���ɐݒ�
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

    //�ŏI�N���X�ŏI�R���|�[�l���g�ւ̏d�݂͂��ׂă[��
    for (int h = 0; h < H; h++) {
        w[h][classNum - 1][componentNum - 1] = 0;
    }
	return;
}

//�덷�v�Z
double callocError(vector<vector<double>> Y, vector<vector<double>> Ty, int dataN, int classNum) {
    double error = 0;
    for (int n = 0; n < dataN; n++) {
        for (int k = 0; k < classNum; k++) {
            error += -1 * Ty[n][k] * log(Y[n][k]);
        }
    }
    return error;
}