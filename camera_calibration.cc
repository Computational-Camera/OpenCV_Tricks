
vector<Point2f>  imagePoints;                 // real  pattern point postion     
vector<vector<Point2f> > imagePoints_vec;     // real  pattern points array postion        
vector<vector<Point3f> > objectPoints_vec;    // ideal pattern point array postion
vector<Point3f> objectPoints;                 // ideal pattern point postion
vector<Point2f> objectPoints2d;   

Mat cameraMatrix = Mat::eye(3, 3, CV_64F);    // Camera intrinsic matrix
Mat distCoeffs   = Mat::zeros(8, 1, CV_64F);  // Distortion coefficients

//ideal points
for( int j= 0; j < (DOT_H-1); ++j )
  for( int i = 0; i < (DOT_W-1); ++i )
    objectPoints.push_back(Point3f((i+1)*LCD_DOT_W, (j+1)*LCD_DOT_H, 0));
objectPoints_vec.push_back(objectPoints);		

//pratical points
find= findChessboardCorners(ref, patternSize, imagePoints, CALIB_CB_ADAPTIVE_THRESH |CALIB_CB_FILTER_QUADS);

vector<Mat> rvecs, tvecs;  // store errors vectors
//--Find intrinsic and extrinsic camera parameters objectPoints,
double rms = calibrateCamera(objectPoints_vec, imagePoints_vec, ref.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

//---compensate radial and tangential lens distortion
undistort(ref,tmp,cameraMatrix, distCoeffs);

Mat T=findHomography(imagePoints2, objectPoints,RANSAC, 1 ); //CV_RANSAC for opencv 2.4
Mat dst;
warpPerspective(tmp, dst, T, Size(LCD_W, LCD_H), INTER_LINEAR);
