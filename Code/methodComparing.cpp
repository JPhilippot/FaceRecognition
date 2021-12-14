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
typedef Vector<u_char,Dynamic> ImgLine;
typedef Matrix<ImgLine,Dynamic,Dynamic> FlattenedImages;

unsigned char max(u_char a, u_char b){
    if (a<b) return b;
    else return a;
}

unsigned char min(u_char a, u_char b){
    if (a>b) return b;
    else return a;
}

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

int main(){
    
}
