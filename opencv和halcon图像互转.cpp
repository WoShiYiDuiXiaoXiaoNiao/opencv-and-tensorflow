//	转换函数
HObject Mat2HObject(Mat& image)
{
	HObject Hobj=HObject();
	int hgt=image.rows;
	int wid=image.cols;
	int i;
	//	CV_8UC3
	if(image.type() == CV_8UC3)
	{
		vector<Mat> imgchannel;
		split(image,imgchannel);
		Mat imgB=imgchannel[0];
		Mat imgG=imgchannel[1];
		Mat imgR=imgchannel[2];
		uchar* dataR=new uchar[hgt*wid];
		uchar* dataG=new uchar[hgt*wid];
		uchar* dataB=new uchar[hgt*wid];
		for(i=0;i<hgt;i++)
		{
			memcpy(dataR+wid*i,imgR.data+imgR.step*i,wid);
			memcpy(dataG+wid*i,imgG.data+imgG.step*i,wid);
			memcpy(dataB+wid*i,imgB.data+imgB.step*i,wid);
		}
		GenImage3(&Hobj,"byte",wid,hgt,(Hlong)dataR,(Hlong)dataG,(Hlong)dataB);
		delete []dataR;
		delete []dataG;
		delete []dataB;
		dataR=NULL;
		dataG=NULL;
		dataB=NULL;
	}
	//	CV_8UCU1
	else if(image.type() == CV_8UC1)
	{
		uchar* data=new uchar[hgt*wid];
		for(i=0;i<hgt;i++)
			memcpy(data+wid*i,image.data+image.step*i,wid);
		GenImage1(&Hobj,"byte",wid,hgt,(Hlong)data);
		delete[] data;
		data=NULL;
	}
	return Hobj;
}
 
Mat HObject2Mat(HObject Hobj)
{
	HTuple htCh=HTuple();
	HTuple cType;
	Mat Image;
	ConvertImageType(Hobj,&Hobj,"byte");
	CountChannels(Hobj,&htCh);
	HTuple wid;
	HTuple hgt;
	int W,H;
	if(htCh[0].I()==1)
	{
		HTuple ptr;
		GetImagePointer1(Hobj,&ptr,&cType,&wid,&hgt);
		W=(Hlong)wid;
		H=(Hlong)hgt;
		Image.create(H,W,CV_8UC1);
		uchar* pdata=(uchar*)ptr[0].I();
		memcpy(Image.data,pdata,W*H);
	}
	else if(htCh[0].I()==3)
	{
		HTuple ptrR, ptrG, ptrB;
		GetImagePointer3(Hobj,&ptrR,&ptrG,&ptrB,&cType,&wid,&hgt);
		W=(Hlong)wid;
		H=(Hlong)hgt;
		Image.create(H,W,CV_8UC3);
		vector<Mat> vecM(3);
		vecM[2].create(H,W,CV_8UC1);
		vecM[1].create(H,W,CV_8UC1);
		vecM[0].create(H,W,CV_8UC1);
		uchar* pr=(uchar*)ptrR[0].I();
		uchar* pg=(uchar*)ptrG[0].I();
		uchar* pb=(uchar*)ptrB[0].I();
		memcpy(vecM[2].data,pr,W*H);
		memcpy(vecM[1].data,pg,W*H);
		memcpy(vecM[0].data,pb,W*H);
		merge(vecM,Image);
	}
	return Image;
}
