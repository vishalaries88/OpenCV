#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
using namespace cv;
using namespace std;
int main( void )
{
   double alpha = 0.5; double beta; double input;
   Mat src1, src2, dst;
   cout << " Simple Linear Blender " << endl;
   cout << "-----------------------" << endl;
   cout << "* Enter alpha [0.0-1.0]: ";
   cin >> input;
   // We use the alpha provided by the user if it is between 0 and 1
   if( input >= 0 && input <= 1 )
     { alpha = input; }
   src1 = imread( "images/WindowsLogo.jpg" );
   Size size(200,200);
   Mat dst1,dst2;
   resize(src1,dst1,size);//resize image
// show the image
//    namedWindow("Display Image", WINDOW_AUTOSIZE );
//    imshow("Display Image", src1);
   src2 = imread( "images/LinuxLogo.jpg" );
   if( src1.empty() ) { cout << "Error loading src1" << endl; return -1; }
   if( src2.empty() ) { cout << "Error loading src2" << endl; return -1; }
   resize(src2,dst2,size);//resize image

   beta = ( 1.0 - alpha );
   addWeighted( dst1, alpha, dst2, beta, 0.0, dst);
   imshow( "Linear Blend", dst );
   waitKey(0);
   return 0;
}