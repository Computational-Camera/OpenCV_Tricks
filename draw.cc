
line(src, Point(x1,y2), Point(x2, y2), Scalar(r,g,b),2);

circle(src, Point(x,y), 2, Scalar(r,g,b), -1);

rectangle(src, Point(x1,y1), Point(x2,y2), Scalar(r,g,b), 4, 8, 0);

//=== Poly line ===
const Point *pts = (const cv::Point*) Mat(contour).data;
const Point *ppt[1] = {&points_out[0]};
int npt[] = { 4 }; //number of vertex
polylines(  src, ppt, npt,
          1, //closed
          1, //linewidth=1
          Scalar( r, g, b ),
          2);     //lineType=8
}

//===Text===
putText(img, str, imagePoint, FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 2);

//=== Display ===
namedWindow( "display_window", WINDOW_AUTOSIZE );
imshow( "display_window", src);
waitKey();// display window stay until key pressed
