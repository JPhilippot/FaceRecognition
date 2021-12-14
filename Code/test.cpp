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




int main(){
    vector<TempImgLine> eigenfaces;
    int K =100; int nH;int nW;
    for (int i=0; i<K;i++){
        OCTET* im;
        char name[50];
        sprintf(name,"eigenfaces/im%d.pgm",i);
        lire_nb_lignes_colonnes_image_pgm(name,&nH,&nW);
        allocation_tableau(im,OCTET,nH*nW);
        lire_image_pgm(name,im,nH*nW);
        TempImgLine eigenFace(nH*nW) ;
        double sum=0.0;
        for (int j=0;j<nH*nW;j++){
            eigenFace(j)=im[j]-127;
            sum+=(double)(im[j]-127)*(double)(im[j]-127);
        }
        for (int j=0;j<nH*nW;j++){eigenFace(j)=eigenFace(j)/sqrt(sum);}
        eigenfaces.push_back(eigenFace);
        free(im);
    }

    OCTET* meanIm; allocation_tableau(meanIm,OCTET, nH*nW); lire_image_pgm("MEAN.pgm",meanIm,nH*nW);
    OCTET* felix; allocation_tableau(felix,OCTET,nH*nW); //lire_image_pgm("../Databases/yalefaces/test/pgms/subject01.noglasses.pgm",felix,nH*nW);
    //lire_image_pgm("../Databases/AT&T/s25/3.pgm",felix,nH*nW);
    lire_image_pgm("116.pgm",felix,nH*nW);

    //for (int i=0; i<nH*nW;i++){felix[i]=min(255,max(0,felix[i]-meanIm[i]));}

    //ecrire_image_pgm("meanLessFix.pgm",felix,nH,nW);

    //cout<<eigenfaces[1]<<endl;
    double sumF=0.0;
    for (int i=0; i<nH*nW;i++){sumF+=(double)(felix[i]-127)*(double)(felix[i]-127);}
    TempImgLine fixLine(nH*nW) ;for (int i=0;i<nH*nW;i++){fixLine(i)=(double)(felix[i]-127)/sumF;}
    //cout<<fixLine<<endl;
    //cout<<eigenfaces[0]<<endl;
    //cout<<fixLine.dot(eigenfaces[0])<<endl;
    vector<double> projs = projectOnEigenSpace(eigenfaces,fixLine,K);
    double sum=0.0;
    for (int i=0;i<K;i++){
        //cout<<i<<" : "<<projs[i]<<endl;
        sum+=projs[i];
    }
    //cout<<sum<<endl;

    // for (int i=0;i<K;i++){
    //     OCTET* im;allocation_tableau(im,OCTET,nH*nW);
    //     for (int pix=0;pix<nH*nW;pix++){
    //         im[pix]=eigenfaces[i](pix);

    //     }
    //     char name2[50];
    //     sprintf(name2,"testEigenFaces/im%d.pgm",i);
    //     ecrire_image_pgm(name2,im,nH,nW);
    // }
    
    OCTET* reco; allocation_tableau(reco,OCTET, nH*nW);

    TempImgLine recoTemp(nH*nW); for (int i=0;i<nH*nW;i++){recoTemp(i)=0.0;}

    for (int i=0; i<K;i++){
        for (int pix=0; pix<nH*nW;pix++){
            recoTemp(pix)+=(projs[i]) * (eigenfaces[i])(pix);
        }
        cout<<projs[i]<<endl;
        //cout<<recoTemp<<endl;
    }

    // for (int i=0;i<nH*nW;i++){
    //     recoTemp(i)+=meanIm[i];
    // }


    double minR=recoTemp(0); double maxR=recoTemp(0);
    for (int i=0; i<nH*nW;i++){
        minR=min(minR,recoTemp(i));
        maxR=max(maxR,recoTemp(i));
    }
    cout<<minR<<"  "<<maxR<<endl;

    for (int i=0;i<nH*nW;i++){
        reco[i]=(unsigned char)((1.0/2.0)*(((recoTemp(i)-minR)*(255.0/(maxR-minR)))+(meanIm[i])));
        //reco[i]=min(255,max(0,recoTemp(i)));
        //cout<<(int)reco[i]<<endl;
    }
    

    ecrire_image_pgm("RecoFelix.pgm",reco,nH,nW);



}