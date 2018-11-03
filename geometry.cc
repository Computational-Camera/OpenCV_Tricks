//=== Convert Point vector to Mat===
vector<Point2f> pt_vec;
Mat pt_mat = Mat(pt_vec).clone();

//==== Convert Mat to vector ===
Point *pts = (const Point*) Mat(contour).data;

//=== Warp image   ====
warpPerspective ( src, dst, T, Size(w,h), INTER_LINEAR, BORDER_CONSTANT);

//=== Points Transformation
perspectiveTransform(p1, p2, T);//for points! src p1, dst p2

//=== Calculate distantce
double cost = norm(Mat(p1), Mat(p2), NORM_L2);

//=== Homography matrix estimation ====
findHomography( dst_pt, src_pt,RANSAC );

//=== Perspective matrix estimation ====
getPerspectiveTransform(const Point2f src[], const Point2f dst[])
