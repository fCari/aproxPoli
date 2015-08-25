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
double calcDis(Point2i p1/*pt*/,Point2i p2,Point2i p3){
    double dis=0.0;
    double m=double(p2.y-p3.y)/double(p2.x-p3.x);    
     dis=abs(double(-m*p1.x)+double(p1.y)+double(m*p2.x)-double(p2.y))/(double)sqrt((double)pow(-m,2.0)+1);
     
    return (double)dis;
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
    Mat img=imread("/home/system/imagenes/img/binary2.jpg",0);
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

  for(list<Point2i>::iterator it=lsr.begin();it!=lsr.end();++it){
      res.at<uchar>((*it).y,(*it).x)=80;
        
  }
    namedWindow( "imagenes1",CV_WINDOW_KEEPRATIO);
    imshow("imagenes1",res);
    waitKey(0);
    cout<<" ,,, "<<lsr.size();
    getchar();
    int n=lsr.size();
    list<Point2i> polAprox;
    Point2i aproxTpm[6];
    double t=0.2;
    double lamda=9.0;
   
        polAprox.push_back(lsr.back());
        aproxTpm[0]=lsr.back();lsr.pop_back();lsr.pop_back();
        aproxTpm[1]=lsr.back();lsr.pop_back();
        aproxTpm[2]=lsr.back();
        int cont=0;
        cout<<" holassas"<<lsr.size()<<endl;
        getchar();
        list<Point2i>::iterator it=lsr.begin();
        while(it!=lsr.end()){
             
            lamda=calcDis(aproxTpm[2],aproxTpm[0],aproxTpm[1]);
          

            cout<<" el valor de  lamda: "<<aproxTpm[2]<<" , "<<aproxTpm[0]<<" , "<<aproxTpm[1]<<" , lamda ::"<<abs(lamda)<<endl;
           
            if(lamda<t){
                ++it;
                aproxTpm[2]=(*it);
                
                
            cout<<" Parte1 : "<<aproxTpm[0]<<" , "<<aproxTpm[1]<<" , "<<aproxTpm[2]<<endl;
          //  getchar();
            }else{// es una esquina por lo tanto 
                --it;
                polAprox.push_back((*it));++it;
                aproxTpm[0]=(*it);++it;++it;
                aproxTpm[1]=(*it);++it;
                aproxTpm[2]=(*it);
                //lsr.pop_back();
                cout<<" Parte2 : "<<aproxTpm[0]<<" , "<<aproxTpm[1]<<" , "<<aproxTpm[2]<<" >>> "<<*(++it)<<endl;
              //  getchar();
            }
            cont++;
            if(cont==lsr.size())
                break;
    }
    cout<<" ::::-lst->"<<polAprox.size()<<", ls: "<<polAprox.size()<<endl;
    for (std::list<Point2i>::iterator it=polAprox.begin(); it != polAprox.end(); ++it){
        //std::cout << ' ' << *it<<endl;
        //getchar();
        res.at<uchar>((*it).y,(*it).x)=255;
        //circle(res,(*it),4,0,-1,8,6);
    }
             namedWindow( "imagenes",CV_WINDOW_KEEPRATIO);
        imshow("imagenes",res);
        
        waitKey(0);
     
  
            
    return 0;
}

