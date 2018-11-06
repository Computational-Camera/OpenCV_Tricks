//====Number to String====
char str[8];//enough large
sprintf(str, "%d", t);

//====Variable File Name====
ostringstream fn;
fn<<"./output/xxx_"<<k<<".JPG";
(fn.str()).c_str()
  
//=====Time Performance=====
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC_RAW, &start);
//------------------
clock_gettime(CLOCK_MONOTONIC_RAW, &end);
uint64_t delta_us = (end.tv_nsec - start.tv_nsec) / 1000;
