#include <bits/types/FILE.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <tgmath.h>
#include "image_ppm.h"
#include <filesystem>
// #include <Eigen/Dense>

using namespace std;
// using namespace Eigen;

// typedef Matrix<u_char, Dynamic, Dynamic> MatrixImg;
// typedef Matrix<double, Dynamic, Dynamic> TempMatrixImg;
// typedef Vector<u_char,Dynamic> ImgLine;
// typedef Vector<double,Dynamic> TempImgLine;
//typedef Matrix<ImgLine,Dynamic,Dynamic> FlattenedImages;



unsigned char max(u_char a, u_char b){
    if (a<b) return b;
    else return a;
}

unsigned char min(u_char a, u_char b){
    if (a>b) return b;
    else return a;
}


double max(double a, double b){
    if (a<b) return b;
    else return a;
}

double min(double a, double b){
    if (a>b) return b;
    else return a;
}

int max(int a, int b){
    if (a<b) return b;
    else return a;
}

int min(int a, int b){
    if (a>b) return b;
    else return a;
}

// auto max(auto a, auto b){
//     if (a<b) return b;
//     else return a;
// }

// auto min(auto a, auto b){
//     if (a>b) return b;
//     else return a;
// }



// vector<double> projectOnEigenSpace(vector<TempImgLine> eigenfaces, TempImgLine imToProj,int K){
//     vector<double> res = vector<double>();
//     for (int i=0;i<K;i++){
//         res.push_back(eigenfaces[i].dot(imToProj));
//     }
//     return res;
// }

// ImgLine octToVec(OCTET* im, int nH, int nW){
//     ImgLine res(nH*nW);
//     for (int i=0; i<nH*nW;i++){
//         res(i)=im[i];
//     }
//     return res;
// }

// double eigenProjsDistance(vector<double> proj1, vector<double> proj2){

//     double res =0.0;
//     for (int i=0; i<min(proj1.size(),proj2.size());i++){
//         res+=(proj1[i]-proj2[i])*(proj1[i]-proj2[i]);
//     }
//     return res;
// }

unsigned char LBPvalue(OCTET* image, int x, int y, int nW, int nH){
    int count =0;
    int* binaryNumber = new int[8];
    
    for(int i =-1;i<2;i++){
        for (int j=-1;j<2;j++){
            if (i!=0 && j!=0){
                if (image[(x+i)*nW+j+y]<image[x*nW+y]){
                    binaryNumber[count]=0;
                }
                if (image[(x+i)*nW+j+y]>=image[x*nW+y]){
                    binaryNumber[count]=1;
                }
                count++;
            }
        }
    }
    unsigned char res = (unsigned char)(1*binaryNumber[7] + 2*binaryNumber[6] + 4*binaryNumber[5] + 8*binaryNumber[4] + 16*binaryNumber[3] + 32*binaryNumber[2] + 64*binaryNumber[1] + 128*binaryNumber[0]);
    return (res);
}

double minutie(int* hist, int size,int nH, int nW, int gridX, int gridY){
    double res =0.0;
    for (int i =0;i<size;i++){
        res+=i*(double)hist[i]/((double)(nW*nH)/(double)(gridY*gridX));//(double)size;
    }
    return res;
}

double divergenceLK(int* hist1, int* hist2, int size){
    double res = 0.0;
    for (int k=0;k<size;k++){
        res+=(hist1[k]+1.0)*log((hist1[k]+1.0)/(hist2[k]+1.0));
    }
    return res;
}



int main(int argc, char* argv[]){

    //DB file : 
    ofstream outFile;
    outFile.open ("DBLBP.txt");
    
    vector<vector<int*>> histos = vector<vector<int*>>(); 

    int nH, nW;
    int gridX=5,gridY=5;


    if (argc<2){
        // cout<<"usage : dir0 ... dirn\n"<<endl;
    }
    vector<string> directories;
    for (int i=0; i<argc-1;i++){
        string s = string(argv[i+1]);
        int found = s.find_last_of('/');

        directories.push_back(s.substr(found+1));
    }
    

    //vector<vector<vector<double>>> registre = vector<vector<vector<double>>>();

    int countDirs =0;
    for (int i=0; i<argc-1;i++){
        int countFile=0;
        outFile<<"!"<<directories[i]<<endl;
        histos.push_back(vector<int*>());
        for (auto& file : std::filesystem::directory_iterator(argv[i+1])){
            histos[i].push_back((int*)calloc(gridX*gridY*256,sizeof(int)));
            //registre.push_back(vector<vector<double>>());
            OCTET* img;
            lire_nb_lignes_colonnes_image_pgm(file.path().c_str(),&nH,&nW);
            allocation_tableau(img,OCTET, nH*nW);
            lire_image_pgm(file.path().c_str(),img,nH*nW);

            OCTET* LBP;
            allocation_tableau(LBP,OCTET, nH*nW);
            for (int i=1;i<nH-1;i++){
                for (int j =1;j<nW-1;j++){
                    LBP[i*nW+j]=LBPvalue(img, i, j, nW, nH); 
                }
            }

            int compteur=0;
            for (int x = 0 ; x<gridX;x++){
                for (int y = 0;y<gridY;y++){
                    int *tempHisto = (int*)calloc(256,sizeof(int));
                    for (int i=y*nH/gridY;i<(y+1)*nH/gridY;i++){
                        for (int j=x*nW/gridX;j<(x+1)*nW/gridX;j++){
                            tempHisto[(int)LBP[i*nW+j]]++;
                        }
                    }
                    for (int k = compteur*256;k<(compteur+1)*256;k++){
                        histos[i][countFile][k]=tempHisto[k%255];                    
                    }
                    compteur++;
                    delete[] tempHisto;
                }
            }

            //registre[countDirs].push_back(projectOnEigenSpace(eigenfaces,imgLine,K));
            //vector<double> testPrint = projectOnEigenSpace(eigenfaces,imgLine,K);
            for (int d=0; d<256*gridX*gridY;d++){outFile<<histos[i][countFile][d]<<" ";}
            outFile<<endl;

            countFile++;
        }
        outFile<<endl;
        countDirs++;
    }

    outFile.close();
}