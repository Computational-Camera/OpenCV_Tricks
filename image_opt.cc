

//=== Visit Pixel ======
Mat image;
image.isContinous
image.data //byte pointer
image.step //? next line 

image.at<Vec3b>(j,i)[0]  //Pixel
uchar* data_ptr = image.ptr<uchar>(j)    //Row pointer, j is the row number
//for i = i+ 1  or i = i + 3
//  *a++ =      or *a++, *a++, *a++  (colour)

//=== Region of Interest ====
Rect roi=Rect(x,y,len_x,len_y);

//=== Image Blending  =======
image = 0.5* imag1 + 0.5*image2  

//=== Split Image ===========
vector<Mat> rgb;
split(frame, rgb); // result rgb[0]   rgb[1] rgb[2]

//==== Color Converstion ====
cvtColor(img, img_colour, COLOR_BayerRG2BGR);
cvtColor(img, img_gray,    COLOR_BGR2GRAY);

//==== Resize Image =========
pyrDown (src, dst) //half

//==== Reshape ==============
Mat src,dst;
dst=src.reshape(w2,h2);//w2h2=w1*h1

//=== Integral ==============
integral( src,  dst,  CV_32SC3);

