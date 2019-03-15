//https://eigen.tuxfamily.org/dox/group__QuickRefPage.html

Matrix<float,3,3> a;//  Matrix3f
//or
MatrixXd a(3,3);
Matrix<float,4,1> b;//  Vector4f

//construction 
b<<1, 2, 3, 4;
A = Matrix3f :: Random () ;
B = Matrix4d :: Identity () ;
A = Matrix3f :: Zero () ;
A = Matrix3f :: Ones () ;

//element visit
cout<<a(1,1)<<endl;

b.transpose()
b.inverse()
