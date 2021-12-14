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

int gridX=5,gridY=5;

auto max(auto a, auto b){
    if (a<b) return b;
    else return a;
}

auto min(auto a, auto b){
    if (a>b) return b;
    else return a;
}





//db is fileName ; names is labels ; values : values[person][imN][intM]
void processDB(string db, vector<string>& names, vector<vector<int*>>& values){
    ifstream file;
    file.open(db);
    values = vector<vector<int*>>();
    string line ;
    while(getline(file, line)){
        size_t exclam = line.find('!');
        if (exclam != std::string::npos){
            names.push_back(line.substr(exclam+1));
            values.push_back(vector<int*>());
        }
        else{
            if (line.size()>1){
                int* res = (int*)malloc(256*gridX*gridY*sizeof(int));
                stringstream ss(line);
                
                int num;
                int i=0;
                while(ss>>num){
                    res[i]=num;
                    i++;
                }
                values[values.size()-1].push_back(res);
            }
        }
    }
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

    if (argc<2){
        cout<<"usage : im.pgm"<<endl;
    }

    int nH, nW;
    


    OCTET* img; 
    lire_nb_lignes_colonnes_image_pgm(argv[1],&nH,&nW);       
    allocation_tableau(img,OCTET, nH*nW);
    lire_image_pgm(argv[1],img,nH*nW);   

    OCTET* LBP;
    allocation_tableau(LBP,OCTET, nH*nW);
    for (int i=1;i<nH-1;i++){
        for (int j =1;j<nW-1;j++){
            LBP[i*nW+j]=LBPvalue(img, i, j, nW, nH); 
        }
    }

    int* histo = (int*)calloc(256*gridX*gridY,sizeof(int));

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
                    histo[k]=tempHisto[k%255];                    
                }
                compteur++;
            }
        }

    

    vector<string> labels; 
    vector<vector<int*>> values;
    processDB("DBLBP.txt", labels, values);

    
    double minDist=INFINITY; 
    int closestMatch = -1;
    for (int i=0;i<values.size();i++){
        double moy=0.0;
        for (int j=0; j<values[i].size();j++){
            double dist = divergenceLK(histo,values[i][j],256*gridY*gridX);
            moy+=dist;
            //eigenProjsDistance(histo, values[i][j]);
            // cout<<labels[i]<<" "<<dist<<endl;
            if (dist<minDist){
                //cout<<dist<<" "<<i<<endl;
                minDist=dist;
                closestMatch=i;
                
            }
            
        }
        //cout<<labels[i]<<" dist moy : "<<moy/(double)(values[i].size())<<endl;
    }

    if (closestMatch>=0){cout<<"\n **************** \nCLOSEST MATCH FOUND : "<<labels[closestMatch]<<"\n ****************"<<endl;}
    
}