#include <bits/types/FILE.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <tgmath.h>
#include "image_ppm.h"
#include <filesystem>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

typedef Matrix<u_char, Dynamic, Dynamic> MatrixImg;
typedef Matrix<double, Dynamic, Dynamic> TempMatrixImg;
typedef Vector<u_char,Dynamic> ImgLine;
typedef Vector<double,Dynamic> TempImgLine;
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



vector<double> projectOnEigenSpace(vector<TempImgLine> eigenfaces, TempImgLine imToProj,int K){
    vector<double> res = vector<double>();
    for (int i=0;i<K;i++){
        res.push_back(eigenfaces[i].dot(imToProj));
    }
    return res;
}

ImgLine octToVec(OCTET* im, int nH, int nW){
    ImgLine res(nH*nW);
    for (int i=0; i<nH*nW;i++){
        res(i)=im[i];
    }
    return res;
}

double eigenProjsDistance(vector<double> proj1, vector<double> proj2){

    double res =0.0;
    for (int i=0; i<min(proj1.size(),proj2.size());i++){
        res+=(proj1[i]-proj2[i])*(proj1[i]-proj2[i]);
    }
    return res;
}



int main(int argc, char* argv[]){


    //recup eigenfaces
    
    vector<TempImgLine> eigenfaces;
    int K =200; int nH;int nW;

    for (int i=0; i<K;i++){
        OCTET* im;
        char name[50];
        sprintf(name,"out/eigenfaces/im%d.pgm",i);
        lire_nb_lignes_colonnes_image_pgm(name,&nH,&nW);
        allocation_tableau(im,OCTET,nH*nW);
        lire_image_pgm(name,im,nH*nW);
        TempImgLine eigenFace(nH*nW) ;
        double sum=0.0;
        for (int j=0;j<nH*nW;j++){
            eigenFace(j)=im[j]-127;
            sum+=(double)(im[j]-127)*(double)(im[j]-127);
        }
        for (int j=0;j<nH*nW;j++){
            eigenFace(j)=eigenFace(j)/sqrt(sum);
            }
        eigenfaces.push_back(eigenFace);
        free(im);
    }

    string dirIn = string(argv[1]);

    //vector<vector<int*>> histos = vector<vector<int*>>(); 

    vector<vector<vector<double>>> registre = vector<vector<vector<double>>>();

    int countDirs =0;
    for (auto & dir : std::filesystem::directory_iterator(dirIn)){
        registre.push_back(vector<vector<double>>());
        int countFile=0;
        for (auto & file : std::filesystem::directory_iterator(dir.path())){
            OCTET* img;
            lire_nb_lignes_colonnes_image_pgm(file.path().c_str(),&nH,&nW);
            TempImgLine imgLine(nH*nW);            
            allocation_tableau(img,OCTET, nH*nW);
            lire_image_pgm(file.path().c_str(),img,nH*nW);
            double sumIm=0.0;
            for (int i=0;i<nH*nW;i++){sumIm+=(double)(img[i]-127)*(double)(img[i]-127);}
            for (int i=0; i<nH*nW;i++){imgLine(i)=(double)(img[i]-127)/sumIm;}
            registre[countDirs].push_back(projectOnEigenSpace(eigenfaces,imgLine,K));

            countFile++;
        }
        countDirs++;
    }

    for (double threshold=0.0;threshold<0.0000005;threshold+=0.00000001){
        int VP=0,VN=0,FP=0,FN=0;

        for (int i=0;i<registre.size();i++){
            for (int j=0; j<registre[i].size();j++){
                for (int ip=0;ip<registre.size();ip++){
                    for (int jp=0;jp<registre[ip].size();jp++){
                        if (!(i==ip && j==jp)){
                            //cout<<eigenProjsDistance(registre[i][j],registre[ip][jp])<<endl;
                            if (eigenProjsDistance(registre[i][j],registre[ip][jp])<threshold){
                                if (i==ip){
                                    VP++;
                                }
                                else{
                                    FP++;
                                }
                            }

                        else{
                                if (i==ip){
                                    FN++;
                                }
                                else{
                                    VN++;
                                }
                            }

                        }
                    }
                }
            }
        }

        double sensitivite = (double)((double)VP/(double)(VP+FN));
        double antispecificite = 1.0-(double)((double)VN/(double)(VN+FP));
        double distance = antispecificite*antispecificite + (1-sensitivite)*(1-sensitivite);
        cout<<antispecificite<<" "<<sensitivite<<" "<<distance<<" "<<threshold<<endl;
    }

}