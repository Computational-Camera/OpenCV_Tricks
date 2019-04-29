#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread("k.jpg");   // Read the file
    
    while(image.cols>1024)
        pyrDown(image, image);


    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );               // Show our image inside it.
    int kp;
    waitKey();                           // Wait for a keystroke in the window

    return 0;
}
