/*connectComponents实现和Halcon类似的功能，这样不用findContours就可以进行物体选择分割等；

可以进一步根据Component的面积，圆周比，大概位置，等等进行筛选；*/

int ConnectComponents()
{
Mat img_src = imread("d:\\work\\pic\\30.png", CV_LOAD_IMAGE_COLOR);
if (img_src.empty())
return -1;
Mat img_gray,img_bin;
cvtColor(img_src, img_gray,COLOR_RGB2GRAY);
threshold(img_gray, img_bin, 100, 255, THRESH_BINARY);
imshow("threshold",img_bin);
Mat labels,stats,centroids;
int nComps = connectedComponentsWithStats(img_bin,labels,stats,centroids);
//int nComps = connectedComponents(img_bin, labels);
Mat img_color = Mat::zeros(img_src.size(), CV_8UC3);


//方法一：设置每个label产生一个mask，在原图针对mask进行颜色设置；
for (int i = 1; i < nComps; i++)
{
Mat mask = (labels == i);	//先对labels操作，凡是值==i的像素，均重置为255；其他不等于i的像素，设置为0；
Vec3b color(Vec3b(rand() % 256, rand() % 256, rand() % 256));
img_color.setTo(color, mask);	//安装mask，对原图进行着色；

//获取stats的数据，并画出外接框，及其左上角；
Rect outRect;
outRect.x = stats.at<int>(i, 0);
outRect.y = stats.at<int>(i, 1);
outRect.width = stats.at<int>(i, 2);
outRect.height = stats.at<int>(i, 3);
circle(img_color, outRect.tl(), 2, color, 2);	
rectangle(img_color, outRect, color,1);
}
imshow("Method1", img_color);


//方法二：逐点设置颜色
img_color.setTo(0);
vector<Vec3b>colors(nComps + 1);
colors[0] = Vec3b(0, 0, 0);//第0个颜色为黑色，背景色；
for (int i = 1; i <= nComps; i++)
{
colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);
}
for(int y = 0; y<img_color.rows;y++)
for (int x = 0; x < img_color.cols; x++)
{
int lable = labels.at<int>(y, x);
img_color.at<Vec3b>(y, x) = colors[lable]; //对原图进行着色
}
imshow("Method2", img_color);
waitKey();

return 0;
}
--------------------- 
作者：Jaden_CSDN 
来源：CSDN 
原文：https://blog.csdn.net/weixin_41961260/article/details/79854130 
版权声明：本文为博主原创文章，转载请附上博文链接！