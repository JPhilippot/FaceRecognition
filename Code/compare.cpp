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

// void loadConcatHist(int** concHist, int size, std::string source){
//     std::ifstream file;
//     std::string line;
//     file.open(source);
    
//     if (file.is_open()){
//         int counter = 0;
//         while (getline (file, line)){
//             *concHist[counter] = (line);
//         }
//     }



//     file.close();
// }

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

// int main(int argc, char* argv[]){
//     char cNomImgLue[250], cNomImgEcrite[250],cNomImgLue2[250];
//     int nH, nW,nH2,nW2, nTaille,nTaille2, gridX, gridY;

//     if (argc!=6){
//         exit(1);
//     }

//     sscanf (argv[1],"%s",cNomImgLue) ;
//     sscanf (argv[2],"%s",cNomImgEcrite);
//     sscanf (argv[3],"%s",cNomImgLue2);
//     sscanf (argv[4],"%d",&gridX);
//     sscanf (argv[5],"%d",&gridY);

//     OCTET *ImgIn, *ImgIn2,*ImgOut, *LBP,*LBP2;
   
//     lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
//     nTaille = nH * nW;
//     lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH2, &nW2);
//     nTaille2 = nH2 * nW2;
  
//     allocation_tableau(ImgIn, OCTET, nTaille);
//     lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
//     allocation_tableau(ImgIn2, OCTET, nTaille2);
//     lire_image_pgm(cNomImgLue2, ImgIn2, nH2 * nW2);
//     allocation_tableau(ImgOut, OCTET, nTaille);
//     allocation_tableau(LBP, OCTET, nTaille);
//     allocation_tableau(LBP2, OCTET, nTaille2);


//     for (int i=1;i<nH-1;i++){
//         for (int j =1;j<nW-1;j++){
//             LBP[i*nW+j]=LBPvalue(ImgIn, i, j, nW, nH);
            
//             //printf("%d\t%d\n",i*nW+j-129,(int)LBP[i*nW+j]);
//         }
//     }
//     for (int i=1;i<nH2-1;i++){
//         for (int j =1;j<nW2-1;j++){
            
//             LBP2[i*nW2+j]=LBPvalue(ImgIn2, i, j, nW2, nH2);
//             //printf("%d\t%d\n",i*nW+j-129,(int)LBP[i*nW+j]);
//         }
//     }
    
//     int *concatHisto = (int*)calloc(256*gridX*gridY,sizeof(int));
//     int compteur=0;
//     for (int x = 0 ; x<gridX;x++){
//         for (int y = 0;y<gridY;y++){
//             int *histo = (int*)calloc(256,sizeof(int));
//             for (int i=y*nH/gridY;i<(y+1)*nH/gridY;i++){
//                 for (int j=x*nW/gridX;j<(x+1)*nW/gridX;j++){
//                     histo[(int)LBP[i*nW+j]]++;
//                 }
//             }
//             //printf(" val histo x:%d y:%d est : %f\n",x,y,minutie(histo,256, nH, nW, gridX, gridY));
//             for (int k = compteur*256;k<(compteur+1)*256;k++){
//                 concatHisto[k]=histo[k%255];
//                 //printf("%d\n",concatHisto[k]);
                
//             }
//             compteur++;
//         }
//     }
//     //printf("\n");
//     //ecrire_image_pgm("histo1.dat", (unsigned char*)concatHisto, 256*gridX*gridY, 1);


//     int *concatHisto2 = (int*)calloc(256*gridX*gridY,sizeof(int));
//     int compteur2=0;
//     for (int x = 0 ; x<gridX;x++){
//         for (int y = 0;y<gridY;y++){
//             int *histo2 = (int*)calloc(256,sizeof(int));
//             for (int i=y*nH2/gridY;i<(y+1)*nH2/gridY;i++){
//                 for (int j=x*nW2/gridX;j<(x+1)*nW2/gridX;j++){
//                     histo2[(int)LBP2[i*nW2+j]]++;
//                 }
//             }
//             //printf(" val histo x:%d y:%d est : %f\n",x,y,minutie(histo,256, nH, nW, gridX, gridY));
//             for (int k = compteur2*256;k<(compteur2+1)*256;k++){
//                 concatHisto2[k]=histo2[k%255];
//                 //printf("%d\t%d\n",k,concatHisto2[k]);
                
//             }
//             compteur2++;
//         }
//     }
//     //ecrire_image_pgm("histo2.dat", (unsigned char*)concatHisto2, 256*gridX*gridY, 1);

//     printf("divergence entre photo 1 et 2 : %f\n",divergenceLK(concatHisto, concatHisto2, 256*gridX*gridY));


//             // on fait cette etape foreach histConcat (nombre images profil). on récupere histmoyen type d'une personne.

//     //ecrire_image_pgm(cNomImgEcrite, LBP,  nH, nW);


//     free(ImgIn);
//     return 1;
// }



int main(int argc, char* argv[]){

    //char nomFolder[255];
    
    //if (argc!=2){cout<<"pas assez d'args";exit(1);}

    //sscanf (argv[1],"%s", nomFolder);
    int nH,nW;

    int gridX = 5;
    int gridY = 5;

    std::string nom = string("../Databases/AT&T/");

    

    vector<vector<int*>> matHistos = vector<vector<int*>>();
    for (int i=0; i<40;i++){
        matHistos.push_back(vector<int*>());
        for (int j=0;j<10;j++){
            matHistos[i].push_back((int*)calloc(gridX*gridY*256,sizeof(int)));
        }
    }

    //cout<<"coucou"<<endl;

    //parcours dirs puis files

    OCTET* img;
    int countDirs = 0;
    //cout<<"coucou2"<<endl;
    for (auto & dir : std::filesystem::directory_iterator(nom)){
        //cout<<"coucou3"<<endl;
        //cout<<dir.path().c_str()<<endl;
        int countFile=0;
        for (auto & file : std::filesystem::directory_iterator(dir.path())){
            
            //image lue
            
            lire_nb_lignes_colonnes_image_pgm(file.path().c_str(),&nH,&nW);
            allocation_tableau(img,OCTET,nH*nW);
            lire_image_pgm(file.path().c_str(),img,nH*nW);     
            
            // LBP application
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
                    int *histo = (int*)calloc(256,sizeof(int));
                    for (int i=y*nH/gridY;i<(y+1)*nH/gridY;i++){
                        for (int j=x*nW/gridX;j<(x+1)*nW/gridX;j++){
                            histo[(int)LBP[i*nW+j]]++;
                        }
                    }
                    //printf(" val histo x:%d y:%d est : %f\n",x,y,minutie(histo,256, nH, nW, gridX, gridY));
                    for (int k = compteur*256;k<(compteur+1)*256;k++){
                        matHistos[countDirs][countFile][k]=histo[k%255];
                        //printf("%d\n",concatHisto[k]);
                        
                    }
                    compteur++;
                }
            }

            free(img);
            free(LBP);
            countFile++;
        }
        countDirs++;        
    }
    

    for (int i=0;i<9;i++){
        for (int j=0; j<9;j++){
            printf("divergence entre photo %d %d et %d %d : %f\n",2,j,1,i,divergenceLK(matHistos[2][j],matHistos[1][i],256*gridX*gridY));
        }
    }
    



    return 1;
}