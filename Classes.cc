Point2i p(x,y); //2D point integer
Point3f p(x,y,x); //3D point floating point

Rect r = Rect(x,y,w,h); //or Rect(p1,p2);
r.tl()
r.br()

Matx33f m; //3 by 3 matrix
Vec3f(x0,x1,x2) //vector, Vec6f...

fastMalloc();//loate memory aligned go a 16 byte boundary

setUseOptimized(1);//The function can be used to dynamically turn on and off optimized dispatched code
flag = useOptimized();//return 1 means optmized



