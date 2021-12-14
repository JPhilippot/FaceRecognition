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

// auto max(auto a, auto b){
//     if (a<b) return b;
//     else return a;
// }

// auto min(auto a, auto b){
//     if (a>b) return b;
//     else return a;
// }



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

    //DB file : 
    ofstream outFile;
    outFile.open ("DBEigen.txt");
    


    //recup eigenfaces

    if (argc<3){
        cout<<"usage : eigenfaces dir0 ... dirn\n"<<endl;
    }
    vector<string> directories;
    for (int i=0; i<argc-2;i++){
        string s = string(argv[i+2]);
        int found = s.find_last_of('/');

        directories.push_back(s.substr(found+1));
    }
    
    vector<TempImgLine> eigenfaces;
    int K =42; int nH;int nW;

    for (int i=0; i<K;i++){
        OCTET* im;
        char name[100];
        
        sprintf(name,"/im%d.pgm",i);
        string eigenFolder = string(string(argv[1])+string(name));
        
        lire_nb_lignes_colonnes_image_pgm(eigenFolder.c_str(),&nH,&nW);
        allocation_tableau(im,OCTET,nH*nW);
        lire_image_pgm(eigenFolder.c_str(),im,nH*nW);
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

    
    

    //vector<vector<vector<double>>> registre = vector<vector<vector<double>>>();

    int countDirs =0;
    for (int i=0; i<argc-2;i++){
        int countFile=0;

        outFile<<"!"<<directories[i]<<endl;
        for (auto& file : std::filesystem::directory_iterator(argv[i+2])){
            //registre.push_back(vector<vector<double>>());
            OCTET* img;
            lire_nb_lignes_colonnes_image_pgm(file.path().c_str(),&nH,&nW);
            TempImgLine imgLine(nH*nW);            
            allocation_tableau(img,OCTET, nH*nW);
            lire_image_pgm(file.path().c_str(),img,nH*nW);
            double sumIm=0.0;
            for (int ip=0;ip<nH*nW;ip++){sumIm+=(double)(img[ip]-127)*(double)(img[ip]-127);}
            for (int ip=0; ip<nH*nW;ip++){imgLine(ip)=(double)(img[ip]-127)/sumIm;}

            //registre[countDirs].push_back(projectOnEigenSpace(eigenfaces,imgLine,K));
            vector<double> testPrint = projectOnEigenSpace(eigenfaces,imgLine,K);
            for (int d=0; d<testPrint.size();d++){outFile<<testPrint[d]<<" ";}
            outFile<<endl;

            countFile++;
        }
        outFile<<endl;
        countDirs++;
    }

    outFile.close();
} 