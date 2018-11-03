//====Number to String====
char str[8];//enough large
sprintf(str, "%d", t);

//====Variable File Name====
ostringstream fn;
fn<<"./output/xxx_"<<k<<".JPG";
(fn.str()).c_str()
