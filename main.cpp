#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdlib>
#include<iostream>
#include<vector>
#include<list>
using namespace std;
using namespace cv;

/*
 * 
 */
double dist(){
    double res;
    return res;
}
int len(){
    int res;
    return res;
}
bool buscarPts(Point2i v, Point2i tmp){
    for(int y=-1;y<=1;y++){
        for(int x=-1;x<=1;x++){
           if(v-Point2i(tmp.x+x,tmp.y+y)==Point2i(0,0)&&v!=tmp){
                    return true;    
            }
        }
    }    
    return false;
}
int main() {

    Mat img=imread("/home/fredy/imagenes/img/poligono.jpg",0);
    Mat res=Mat::zeros(img.rows,img.cols, CV_8UC1);
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(img.at<uchar>(i,j)<100){
                img.at<uchar>(i,j)=0;
            }else{
                img.at<uchar>(i,j)=255;
            }
        }
    }
     int umbral=255;
    int px2=5555;
    list<Point> ls ;
   
    int v=0;
    for(int y=2;y<img.rows-2;y++){
        for(int x=2;x<img.cols-2;x++){
            int px1=2*img.at<uchar>(y,x)-img.at<uchar>(y,x+1)-img.at<uchar>(y+1,x);
            if(px1>umbral){
                px1=umbral;                
            }
            if(px1==0){
                px2=2*img.at<uchar>(y,x)-img.at<uchar>(y,x-1)-img.at<uchar>(y-1,x);
                if(px2>umbral){
                       px2=umbral;
                }                
            }            
            if((px1==0&&px2==510)||(px1==510&&px2==0))
                res.at<uchar>(y,x)=umbral;
            else if((px1==0&&px2==255)||(px1==255&&px2==0))
                res.at<uchar>(y,x)=umbral;
            else
                res.at<uchar>(y,x)=px1;//mejor con px1 en vez de 0
            //----------------------GUARDAR EN VECTOR-----------------------------------
           
        }
    }
    for(int y=0;y<res.rows;y++)
        for(int x=0;x<res.cols;x++)
            if(res.at<uchar>(y,x)==255)
                ls.push_back(Point2i(x,y));             
    list<Point2i> lsr;
    int tm=ls.size();
    for (int i=0;i<tm;i++){
                if(lsr.size()!=0){                   
                    for(list<Point2i>::iterator it=ls.begin(); it != ls.end(); ++it){                    
                        if(buscarPts(lsr.back(), Point2i((*it).x,(*it).y))){
                            lsr.push_back(Point2i((*it).x,(*it).y));
                            ls.remove(Point2i((*it).x,(*it).y));
                            break;
                        }else if(buscarPts(lsr.front(), Point2i((*it).x,(*it).y))){
                            lsr.push_front(Point2i((*it).x,(*it).y));
                            ls.remove(Point2i((*it).x,(*it).y));
                            break;
                         }
                    }
                }else{
                    lsr.push_back(ls.back());
                    ls.pop_back();
                }
        }
    int n=lsr.size();
    list<Point2i> polAprox;
    vector<Point2i> aproxTpm;
    int t=5;
    int lamda=0;
    for(int i=0;i<n;i++){
        aproxTpm.push_back(lsr.back());lsr.pop_back();
        aproxTpm.push_back(lsr.back());lsr.pop_back();
        aproxTpm.push_back(lsr.back());lsr.pop_back();
        aproxTpm.push_back(lsr.back());lsr.pop_back();
        aproxTpm.push_back(lsr.back());lsr.pop_back();
        for(list<Point2i>::iterator it=lsr.begin();it!=lsr.end();++it){
            lamda=dist(aproxTpm[4],aproxTpm[0],aproxTpm[3])*dist(aproxTpm[5],aproxTpm[0],aproxTpm[2]);
            lamda/=dist(aproxTpm[4],aproxTpm[0],aproxTpm[2])*dist(aproxTpm[5],aproxTpm[0],aproxTpm[3]);
            double d=1-lamda;
            if(d<t){
            
            }else{
                polAprox.push_back(lsr.back());
                //lsr.pop_back();
            }
        }
    
    }
   /* cout<<" ::::-lst->"<<lsr.size()<<", ls: "<<ls.size()<<endl;
    for (std::list<Point2i>::iterator it=lsr.begin(); it != lsr.end(); ++it){
        //std::cout << ' ' << *it<<endl;
        //getchar();
        res.at<uchar>((*it).y,(*it).x)=80;
         namedWindow( "imagenes",CV_WINDOW_KEEPRATIO);
        imshow("imagenes",res);
        
        waitKey(0);
    }*/
       namedWindow( "imagenes",CV_WINDOW_KEEPRATIO);
    imshow("imagenes",res);
    waitKey(0);
  
            
    return 0;
}

