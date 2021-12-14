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
typedef Vector<u_char, Dynamic> ImgLine;
typedef Matrix<ImgLine, Dynamic, Dynamic> FlattenedImages;

unsigned char max(u_char a, u_char b)
{
    if (a < b)
        return b;
    else
        return a;
}

unsigned char min(u_char a, u_char b)
{
    if (a > b)
        return b;
    else
        return a;
}

double max(double a, double b)
{
    if (a < b)
        return b;
    else
        return a;
}

double min(double a, double b)
{
    if (a > b)
        return b;
    else
        return a;
}

vector<double> projectOnEigenSpace(EigenSolver<TempMatrixImg> &solver, TempMatrixImg &img, int K)
{
    vector<double> res;
    for (int i = 0; i < K; i++)
    {
        double test = ((solver.eigenvectors().col(i).real().transpose()) * img)(0);
        res.push_back(test);
    }
    return res;
}

void writeEigenfaces(EigenSolver<TempMatrixImg> &covMat, MatrixImg &A, int nbrFaces)
{
    // covMat.eigenvalues.
}

void makeEigenSpace(std::string dirIn, std::string dirOut)
{
    int nH, nW;
    vector<OCTET *> set = vector<OCTET *>();

    OCTET *meanImg;
    long long *tempMean;

    long totNumberIm = 0;
    for (auto &file : std::filesystem::directory_iterator(dirIn))
    {
        if (totNumberIm == 0)
        {
            cout << file.path().c_str() << endl;
            lire_nb_lignes_colonnes_image_pgm(file.path().c_str(), &nH, &nW);
            allocation_tableau(meanImg, OCTET, nH * nW);
            tempMean = (long long *)calloc(nH * nW, sizeof(long long));
        }
        // OCTET* im; allocation_tableau(im,OCTET,nH*nW);
        OCTET *im;
        set.push_back(im);
        allocation_tableau(set[totNumberIm], OCTET, nH * nW);
        lire_image_pgm(file.path().c_str(), set[totNumberIm], nH * nW);

        for (int i = 0; i < nH * nW; i++)
        {
            tempMean[i] += set[totNumberIm][i];
        }

        totNumberIm++;
    }

    for (int i = 0; i < nH * nW; i++)
    {
        meanImg[i] = (unsigned char)((double)tempMean[i] / ((double)totNumberIm));
    }
    ecrire_image_pgm(string(dirOut + string("/mean.pgm")).c_str(), meanImg, nH, nW);

    TempMatrixImg bigMatNoMean(nH * nW, totNumberIm);
    for (int i = 0; i < totNumberIm; i++)
    {
        for (int pix = 0; pix < nW * nH; pix++)
        {
            bigMatNoMean(pix, i) = set[i][pix] - meanImg[pix] + 128;
        }
    }

    TempMatrixImg cov = bigMatNoMean.transpose() * bigMatNoMean;
    EigenSolver<TempMatrixImg> eigensolver(cov);

    for (int i = 0; i < eigensolver.eigenvectors().cols(); i++)
    {
        OCTET *im;
        allocation_tableau(im, OCTET, nH * nW);
        TempMatrixImg face = bigMatNoMean * eigensolver.eigenvectors().col(i).real();
        double minD = face(0);
        double maxD = face(0);
        for (int pix = 1; pix < nH * nW; pix++)
        {
            minD = min(minD, face(pix));
            maxD = max(maxD, face(pix));
        }
        for (int pix = 0; pix < nH * nW; pix++)
        {
            // cout<<(face(pix)-minD)*(255.0/(maxD-minD))<<endl;
            im[pix] = (unsigned char)((face(pix) - minD) * (255.0 / (maxD - minD))); // comment fit dans 0 255 ?
        }
        char name[50];
        sprintf(name, "/eigenfaces/im%d.pgm", i);
        ecrire_image_pgm(string(dirOut + string(name)).c_str(), im, nH, nW);
    }
}

int main(int argc, char *argv[])
{

    std::string nom = string("in");
    string dirIn = string(argv[1]);
    string dirOut = string(argv[2]);
    makeEigenSpace(dirIn,dirOut);

    //      int nH,nW;
    //
    // //     vector<vector<OCTET*>> imageSet = vector<vector<OCTET*>>();
    // //     for (int i=0; i<40;i++){
    // //         imageSet.push_back(vector<OCTET*>());
    // //         for (int j=0;j<10;j++){
    // //             OCTET* im;
    // //             imageSet[i].push_back(im);
    // //         }
    // //     }

    // //     OCTET* meanImg;
    // //     long long* tempMean ;
    // //     int countDirs = 0;
    // //     long totalNumberOfImages=0;
    // int i=0;int j=0;
    //     for (auto & dir : std::filesystem::directory_iterator(nom)){
    //         int countFile=0;
    //         for (auto & file : std::filesystem::directory_iterator(dir.path())){
    //             OCTET* img;
    //             lire_nb_lignes_colonnes_image_pgm(file.path().c_str(),&nH,&nW);
    //             allocation_tableau(img,OCTET,nH*nW);
    //             lire_image_pgm(file.path().c_str(),img,nH*nW);
    //             char name[50];
    //             sprintf(name,"in/%d%d.pgm",i,j);
    //             ecrire_image_pgm(name,img,nH,nW);

    // allocation_tableau(imageSet[countDirs][countFile],OCTET,nH*nW);
    // for (int pix = 0;pix<nH*nW;pix++){
    //     imageSet[countDirs][countFile][pix]=img[pix];
    // }
    // //imageSet[countDirs].push_back(img);
    // if (countDirs==0 && countFile==0){
    //     allocation_tableau(meanImg,OCTET,nH*nW);
    //     tempMean=(long long*)calloc(nH*nW,sizeof(long long));
    // }

    // for (int i=0;i<nH*nW;i++){
    //     tempMean[i]+=img[i];
    // }

    // totalNumberOfImages++;
    // countFile++;

    // //free(img);
    //         j++;
    //     }
    //     i++;

    // }

    //     allocation_tableau(meanImg,OCTET,nH*nW);
    //     for (int i=0;i<nH*nW;i++){
    //         meanImg[i]=(unsigned char)(tempMean[i]/totalNumberOfImages);
    //     }

    //     ecrire_image_pgm("MEAN.pgm",meanImg,nH,nW);

    //     vector<vector<OCTET*>> imagesSansMean = vector<vector<OCTET*>>();
    //     vector<vector<TempMatrixImg>> matrixNoMean = vector<vector<TempMatrixImg>>();

    //     for (int i=0;i<imageSet.size();i++){
    //         imagesSansMean.push_back(vector<OCTET*>());
    //         matrixNoMean.push_back(vector<TempMatrixImg>());
    //         for (int j=0; j<imageSet[i].size();j++){
    //             OCTET* img;
    //             TempMatrixImg mat(nH,nW);
    //             imagesSansMean[i].push_back(img);
    //             allocation_tableau(imagesSansMean[i][j],OCTET,nH*nW);

    //             allocation_tableau(img,OCTET,nH*nW);
    //             for (int pix=0;pix<nH*nW;pix++){
    //                 img[pix]=min(255,max(0,imageSet[i][j][pix]+128-meanImg[pix]));
    //                 imagesSansMean[i][j][pix]=img[pix];

    //             }
    //             for (int x=0;x<nH;x++){
    //                 for (int y=0; y<nW;y++){
    //                     mat(x,y)=img[x*nW+y];
    //                 }
    //             }
    //             matrixNoMean[i].push_back(mat);

    //             //uncomment to write images

    //             //char name[50];
    //             //sprintf(name,"meanless/im%d_%d",i,j);
    //             //ecrire_image_pgm(name,img,nH,nW);
    //         }
    //     }

    //    TempMatrixImg bigMatNoMean(nH*nW,totalNumberOfImages);
    //     for (int i=0;i<imageSet.size();i++){
    //         for (int j=0; j<imageSet[i].size();j++){
    //             for (int pix=0;pix<nH*nW;pix++){
    //                 bigMatNoMean(pix,i*imageSet[0].size()+j)=(double)imagesSansMean[i][j][pix];
    //             }
    //         }
    //     }

    //     TempMatrixImg cov = bigMatNoMean.transpose()*bigMatNoMean;

    //     EigenSolver<TempMatrixImg> eigensolver(cov);
    //     //cout<<eigensolver.eigenvalues()<<endl;

    //     for (int i=0;i<eigensolver.eigenvectors().cols();i++){
    //         OCTET* im; allocation_tableau(im,OCTET,nH*nW);
    //         TempMatrixImg face = bigMatNoMean*eigensolver.eigenvectors().col(i).real();
    //         double minD=face(0);double maxD=face(0);
    //         for (int pix =1; pix<nH*nW;pix++){minD=min(minD,face(pix));maxD=max(maxD,face(pix));}
    //         for (int pix=0;pix<nH*nW;pix++){
    //             //cout<<(face(pix)-minD)*(255.0/(maxD-minD))<<endl;
    //             im[pix]=(unsigned char)((face(pix)-minD)*(255.0/(maxD-minD)));     // comment fit dans 0 255 ?
    //         }
    //         char name[50];
    //         sprintf(name,"eigenfaces/im%d.pgm",i);
    //         ecrire_image_pgm(name,im,nH,nW);

    //     }

    // OCTET* felix; allocation_tableau(felix,OCTET,nH*nW);
    // lire_image_pgm("felixResized.pgm",felix,nH*nW);

    // TempMatrixImg fix(nH,nW) ;
    // for (int i=0;i<nH;i++){
    //     for (int j=0; j<nW;j++){
    //         fix(i,j)=(double)felix[i*nW+j] - meanImg[i*nW+j];
    //     }
    // }

    // vector<double> proj ; proj = projectOnEigenSpace(eigensolver, fix, 40);
    // for (int i=0; i<40; i++){
    //     cout<<proj[i];
    // }
    // cout<<endl;
}