#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void thinner(Mat &image,Mat &result,int maxitrator=-1)
{
	image.copyTo(result);
	int count=0;//记录迭代次数
	while(1)
	{
		count++;
		if(maxitrator!=-1 && count>maxitrator)
			break;
		vector<Point>flagPoint;//用于标记要删除的点//vector<pair<int,int>>flagPoint
		//进行点标记
		for(int i=0;i<image.rows;i++)
		{
			for(int j=0;j<image.cols;j++)
			{
				//满足以下四个条件，进行标记
				//p9 p2 p3
				//p8 p1 p4
				//p7 p6 p5
				int p1=result.at<uchar>(i,j);
				int p2=(i==0)?0:result.at<uchar>(i-1,j);
				int p3=(i==0 || j==result.cols-1)?0:result.at<uchar>(i-1,j+1);
				int p4=(j==result.cols-1)?0:result.at<uchar>(i,j+1);
				int p5=(i==result.rows-1 || j==image.cols-1)?0:result.at<uchar>(i+1,j+1);
				int p6=(i==result.rows-1)?0:result.at<uchar>(i+1,j);
				int p7=(i==result.rows-1 || j==0)?0:result.at<uchar>(i+1,j-1);
				int p8=(j==0)?0:result.at<uchar>(i,j-1);
				int p9=(i==0 || j==0)?0:result.at<uchar>(i-1,j-1);
				if((p2+p3+p4+p5+p6+p7+p8+p9)>=2 && (p2+p3+p4+p5+p6+p7+p8+p9)<=6)
				{
					int ap=0;
					if(p2==0 && p3==1) ++ap;
					if(p3==0 && p4==1) ++ap;
					if(p4==0 && p5==1) ++ap;
					if(p5==0 && p6==1) ++ap;
					if(p6==0 && p7==1) ++ap;
					if(p7==0 && p8==1) ++ap;
					if(p8==0 && p9==1) ++ap;
					if(p9==0 && p2==1) ++ap;
					if(ap==1)
					{
						if(p2*p4*p6==0)
						{
							if(p4*p6*p8==0)
								//记入标记点
								flagPoint.push_back(Point(i,j));		
						}
					}
				}
			}		
		}
		//将标记点删除
		//vector<Vec2b>::const_iterator it=flagPoint.begin();
		for(vector<Point>::const_iterator it=flagPoint.begin();it!=flagPoint.end();it++)
			result.at<uchar>((*it).x,(*it).y)=0;
		//直到没有点满足，算法结束
		if(flagPoint.size()==0)
			break;
		else
			flagPoint.clear();
		//再对点进行标记
		for(int i=0;i<image.rows;i++)
		{
			for(int j=0;j<image.cols;j++)
			{
				//满足以下四个条件，进行标记
				//p9 p2 p3
				//p8 p1 p4
				//p7 p6 p5
				int p1=result.at<uchar>(i,j);
				if(p1!=1) continue;
				int p2=(i==0)?0:result.at<uchar>(i-1,j);
				int p3=(i==0 || j==result.cols-1)?0:result.at<uchar>(i-1,j+1);
				int p4=(j==result.cols-1)?0:result.at<uchar>(i,j+1);
				int p5=(i==result.rows-1 || j==image.cols-1)?0:result.at<uchar>(i+1,j+1);
				int p6=(i==result.rows-1)?0:result.at<uchar>(i+1,j);
				int p7=(i==result.rows-1 || j==0)?0:result.at<uchar>(i+1,j-1);
				int p8=(j==0)?0:result.at<uchar>(i,j-1);
				int p9=(i==0 || j==0)?0:result.at<uchar>(i-1,j-1);
				if((p2+p3+p4+p5+p6+p7+p8+p9)>=2 && (p2+p3+p4+p5+p6+p7+p8+p9)<=6)
				{
					int ap=0;
					if(p2==0 && p3==1) ++ap;
					if(p3==0 && p4==1) ++ap;
					if(p4==0 && p5==1) ++ap;
					if(p5==0 && p6==1) ++ap;
					if(p6==0 && p7==1) ++ap;
					if(p7==0 && p8==1) ++ap;
					if(p8==0 && p9==1) ++ap;
					if(p9==0 && p2==1) ++ap;
					if(ap==1)
					{
						if(p2*p4*p8==0)
						{
							if(p2*p6*p8==0)
								//记入标记点
								flagPoint.push_back(Point(i,j));		
						}
					}
				}
			}		
		}
		//将标记点删除
		for(vector<Point>::const_iterator it=flagPoint.begin();it!=flagPoint.end();it++)
			result.at<uchar>((*it).x,(*it).y)=0;
		//直到没有点满足，算法结束
		if(flagPoint.size()==0)
			break;
		else
			flagPoint.clear();
	}
	cout<<count<<endl;
}
void main()
{
	Mat result;
	Mat binary;
	Mat opend;
	Mat element(3,3,CV_8UC1,Scalar(1));
	Mat img=imread("StructLight0.JPG",0);//读入灰度图像
	medianBlur(img,img,3);
	threshold(img,binary,128,255,THRESH_BINARY|THRESH_OTSU);
	imshow("binary",binary);
	imwrite("binary.jpg",binary);
	/*medianBlur(binary,opend,3);
	dilate(opend,opend,element);
	imshow("opend",opend);*/
	threshold(binary,binary,128,1,THRESH_BINARY);//这个不是一般的二值，而是根据阈值来设定灰度值
	//imshow("img",img);
	thinner(binary,result);
	for(int i=0;i<result.rows;i++)
	{
		for(int j=0;j<result.cols;j++)
		{
			if(result.at<uchar>(i,j)==1)
				result.at<uchar>(i,j)=255;
		}	
	}
	imshow("result",result);
	imwrite("result.jpg",result);
	waitKey();
}