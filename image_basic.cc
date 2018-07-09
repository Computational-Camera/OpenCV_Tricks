//======Basic Defination=====
Vec3w unsigned short
Vec3s short
Vec3b unsigned uchar
Vec3f  float
Vec3d double

//====New a image====
Mat image;
Mat image(320,320, CV_U3C);

//====Image information===
image.channels()
image.cols
image.rows
image.??? (type)

//load image
Mat img = imread(filedir,flag);
IMREAD_ANYCOLOR  =  4
IMREAD_ANYDEPTH  =  2
IMREAD_COLOR     =  1
IMREAD_GRAYSCALE =  0  
IMREAD_LOAD_GDAL =  8
IMREAD_UNCHANGED = -1
image.data

//save image
imwrite("test.jpg", image);

//load videos
VideoCapture cap(INPUT_VIDEO_PATH);
Mat cur;
for (;;){
  cap >> cur;
}

//=======Initialize Mat with 1D array=================
double t[9]={1,0,0, 0,1,0, 0,0,1};
Mat    A1=Mat(3,3,CV_64F,t);
Mat    B(3600,5760,CV_8UC3, Scalar(255,255,255)); //black color image (y,x)

//=======Initialize to zeros or ones or identity============
Mat src=Mat::zeros(h,w,type)
Mat src=Mat::ones( h,w,type)
Mat src=Mat::eye(   w,w,type)
Mat src=Mat(w,h,type,Scalar() or (,,)) //any constant value

