#include <bits/types/FILE.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <tgmath.h>
#include "image_ppm.h"
#include <filesystem>


using namespace std;




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

int main(int argc, char* argv[]){

    //open images (but 1 of each person)

    string dirIn = string(argv[1]);

    vector<vector<int*>> histos = vector<vector<int*>>(); 

    int nH, nW;
    int gridX=5,gridY=5;

    for (int i=0; i<40;i++){
        histos.push_back(vector<int*>());
        for (int j=0;j<10;j++){
            histos[i].push_back((int*)calloc(gridX*gridY*256,sizeof(int)));
        }
    }
    
    int countDirs =0;
    for (auto & dir : std::filesystem::directory_iterator(dirIn)){
        int countFile=0;
        for (auto & file : std::filesystem::directory_iterator(dir.path())){
            OCTET* img;
            lire_nb_lignes_colonnes_image_pgm(file.path().c_str(),&nH,&nW);
            allocation_tableau(img,OCTET, nH*nW);
            lire_image_pgm(file.path().c_str(),img,nH*nW);

            //apply LBP alg

            OCTET* LBP;
            allocation_tableau(LBP,OCTET, nH*nW);
            for (int i=1;i<nH-1;i++){
                for (int j =1;j<nW-1;j++){
                    LBP[i*nW+j]=LBPvalue(img, i, j, nW, nH); 
                }
            }
            char nom[50];
            sprintf(nom,"subLBPout/im%d%d",countDirs,countFile);
            ecrire_image_pgm(nom,LBP,nH,nW);

            //fill histos

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
                        histos[countDirs][countFile][k]=tempHisto[k%255];                    
                    }
                    compteur++;
                }
            }
            countFile++;
        }
        countDirs++;
    }

    // cout<<"div meme personne : "<<divergenceLK(histos[0][0],histos[0][1],256*gridX*gridY)<<endl;
    // cout<<"div diff personnes : "<<divergenceLK(histos[0][0],histos[1][1],256*gridX*gridY)<<endl;

    //make vector of vector -> [i][j] -> histogram j of person i  -> DB

    //scale everything down. depends on haar ... no need rn 

    //for 1 image that's NOT in DB :

    //compute score with != thresholds
    
    for (int threshold=100;threshold<4000; threshold+=200){
        int VP=0,VN=0,FP=0,FN=0;

        for (int i=0; i<histos.size();i++){
            for (int j=0; j<histos[i].size();j++){
                for (int ip=0; ip<histos.size();ip++){
                    for (int jp=0; jp<histos[ip].size();jp++){
                        if (!(i==ip && j==jp)){
                            
                            if (divergenceLK(histos[i][j],histos[ip][jp],256*gridY*gridX)<(float)threshold){

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
        //cout<<"VP : "<<VP<<" FP : "<<FP<<" FN : "<<FN<<" VN : "<<VN<<endl;
        double sensitivite = (double)((double)VP/(double)(VP+FN));
        double antispecificite = 1.0-(double)((double)VN/(double)(VN+FP));
        double distance = antispecificite*antispecificite + (1-sensitivite)*(1-sensitivite);
        cout<<sensitivite<<" "<<antispecificite<<" "<<distance<<" "<<threshold<<endl;
    }
    

    //get false/true positives/negatives -> get ROC -> find best threshold
}
