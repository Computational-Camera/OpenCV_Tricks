  Eigen::Matrix<double, Eigen::Dynamic, 3> V; //vertex
  Eigen::Matrix<int,    Eigen::Dynamic, 3> F; //edge
  Eigen::Matrix<double, Eigen::Dynamic, 3> N; //norm
  
  igl::readSTL("./data/mesh.stl",   V, F, N);

  cout<<V.rows()<<" "<<V.cols()<<" "<<V.size()<<" "<<endl;  
