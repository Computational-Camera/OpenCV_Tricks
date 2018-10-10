//======Basic Defination=====
Vec3w unsigned short
Vec3s short
Vec3b unsigned uchar
Vec3f  float
Vec3d double

#define CV_8U   0 // 8-bit unsigned integers ( 0..255 )
#define CV_8S   1 // 8-bit signed integers ( -128..127 )
#define CV_16U  2 // 16-bit unsigned integers ( 0..65535 )
#define CV_16S  3 // 16-bit signed integers ( -32768..32767 )
#define CV_32S  4 // 32-bit signed integers ( -2147483648..2147483647 )
#define CV_32F  5 // 32-bit floating-point numbers ( -FLT_MAX..FLT_MAX, INF, NAN )
#define CV_64F  6 // 64-bit floating-point numbers ( -DBL_MAX..DBL_MAX, INF, NAN )

//=== New a image ==========
Mat image;
Mat image(320,320, CV_U3C);#w ,h
cout<<" depth "<<image.depth()<<"channels "<<image.channels()<<endl;
or cout<<" type "<<image.type()<<endl;
//=== Image information ====
image.channels()
image.cols
image.rows
image.??? (type)

//=== Load image ===========
Mat img = imread(filedir,flag);
IMREAD_ANYCOLOR  =  4
IMREAD_ANYDEPTH  =  2
IMREAD_COLOR     =  1
IMREAD_GRAYSCALE =  0  
IMREAD_LOAD_GDAL =  8
IMREAD_UNCHANGED = -1
image.data

//=== save image ===========
imwrite("test.jpg", image);

//load videos
VideoCapture cap(INPUT_VIDEO_PATH);
Mat cur;
for (;;){
  cap >> cur;
}
//write vidoes
VideoWriter outputVideo;  //output
Size S = Size(W,H);
int ex = CV_FOURCC('X','V','I','D' );//static_cast<int>(cap.get(CV_CAP_PROP_FOURCC)); 'M','J','P','G' 
outputVideo.open("test.avi", ex, cap.get(CV_CAP_PROP_FPS), S, true);

//====Access Mat Efficiently======
uchar*  img_ptr_temp = img.ptr<uchar>(line_num) + offset;

//=== Convert Type============
img.convertTo(img2, CV_8UC3);

//=== Initialize Mat with 1D array ============
double t[9]={1,0,0, 0,1,0, 0,0,1};
Mat    A1=Mat(3,3,CV_64F,t);
Mat    B(3600,5760,CV_8UC3, Scalar(255,255,255)); //black color image (y,x)

//=== Initialize to zeros or ones or identity ==
Mat src=Mat::zeros(h,w,type)
Mat src=Mat::ones( h,w,type)
Mat src=Mat::eye(   w,w,type)
Mat src=Mat(w,h,type,Scalar() or (,,)) //any constant value

//=== Copy Mat ======
src.copyTo(dst); //real data copy
//basically same as .clone(), a wrapper of copyTo

//=== CvMat ===
//C interface of Mat. Might need memory release instread of Mat (C++ interface)
A[3][3]  
CvMat _A = CvMat(3,3, CV_64F, A)
//=== CvMat to Mat conversion  
CvMat *a;
Mat b = a;//only copy header
Mat b = Mat(a, true);//clone need check if this works in opencv 3.1 or above
//=== Mat to CvMat conversion
Mat a;
CvMat b = a;//only copy header

  
