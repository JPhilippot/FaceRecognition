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

auto max(auto a, auto b){
    if (a<b) return b;
    else return a;
}

auto min(auto a, auto b){
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


//db is fileName ; names is labels ; values : values[person][imN][doubleM]
void processDB(string db, vector<string>& names, vector<vector<vector<double>>>& values){
    ifstream file;
    file.open(db);
    values = vector<vector<vector<double>>>();
    string line ;
    while(getline(file, line)){
        size_t exclam = line.find('!');
        if (exclam != std::string::npos){
            names.push_back(line.substr(exclam+1));
            values.push_back(vector<vector<double>>());
        }
        else{
            if (line.size()>1){
                vector<double> res;
                stringstream ss(line);
                
                double num;
                while(ss>>num){
                    res.push_back(num);
                }
                values[values.size()-1].push_back(res);
            }
        }
    }
}

int main(int argc, char* argv[]){

    if (argc<3){
        cout<<"usage : eigenfaces im.pgm"<<endl;
    }

    vector<TempImgLine> eigenfaces;
    int K =42; int nH,nW;

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

    

    OCTET* img; 
    lire_nb_lignes_colonnes_image_pgm(argv[2],&nH,&nW);
    TempImgLine imgLine(nH*nW);            
    allocation_tableau(img,OCTET, nH*nW);
    lire_image_pgm(argv[2],img,nH*nW);
    double sumIm=0.0;
    for (int i=0;i<nH*nW;i++){sumIm+=(double)(img[i]-127)*(double)(img[i]-127);}
    for (int i=0; i<nH*nW;i++){imgLine(i)=(double)(img[i]-127)/sumIm;}

    //registre[countDirs].push_back(projectOnEigenSpace(eigenfaces,imgLine,K));
    vector<double> imProj = projectOnEigenSpace(eigenfaces,imgLine,K);

    vector<string> labels; 
    vector<vector<vector<double>>> values;
    processDB("DBEigen.txt", labels, values);
    double minDist=INFINITY; 
    int closestMatch = -1;
    for (int i=0;i<values.size();i++){
        for (int j=0; j<values[i].size();j++){
            double dist = eigenProjsDistance(imProj, values[i][j]);
            //cout<<labels[i]<<" "<<dist<<endl;
            if (dist<minDist){
                //cout<<dist<<" "<<i<<endl;
                minDist=dist;
                closestMatch=i;
                
            }
        }
    }

     if (closestMatch>=0){cout<<"\n **************** \nCLOSEST MATCH FOUND : "<<labels[closestMatch]<<"\n ****************"<<endl;}
    else {cout<<"No match found !"<<endl;}
    
}